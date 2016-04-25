#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->tableWidget->setColumnCount(5);

    modbus = new Modbus();
    serialDialog = new SerialSettingDialog();
    serialSendDialog = new SerialSendDialog();
    plotSettingDialog = new PlotSettingDialog();

    ploter = new Plot(ui->qwtPlot);

    plotCount = 0.0;

    msgBox = new QMessageBox();

    plotTimer = new QTimer(this);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(updateCaption()));
    plotTimer->start(20);

    timesample.start();

    connect(modbus,SIGNAL(finished()),
            this, SLOT(modbusSendFinishedHandle()));
}

MainWindow::~MainWindow()
{
    if(modbus->state() == QModbusDevice::ConnectedState)
        modbus->disconnectDevice();

    delete msgBox;
    delete modbus;
    delete serialDialog;
    delete serialSendDialog;
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
   if( !modbus->connectDevice() )
   {
       ui->statusBar->showMessage( tr("Serial Port %1 Open Error: %2")
                                   .arg(modbus->connectionParameter(QModbusDevice::SerialPortNameParameter).toString())
                                   .arg(modbus->errorString()));
   }
   else
   {
       ui->statusBar->showMessage(tr("Serial Port %1 Open OK")
                                  .arg(modbus->connectionParameter(QModbusDevice::SerialPortNameParameter).toString()));
   }
}


void MainWindow::on_actionClose_triggered()
{
    modbus->disconnectDevice();
    ui->statusBar->showMessage(tr("Serial Port %1 Closed")
                               .arg((modbus->connectionParameter(QModbusDevice::SerialPortNameParameter).toString())));
}

void MainWindow::on_actionSetting_triggered()
{
    if(serialDialog->exec() == 1)
        modbus->setting(serialDialog);
}

void MainWindow::on_actionSend_triggered()
{
    if(serialSendDialog->exec() == 1)
    {
        if(modbus->sendRequest(serialSendDialog) == true)
        {
            ui->statusBar->showMessage("Send data ok");
            updateTable(ui->tableWidget, "Master");
        }
        else
        {
            ui->statusBar->showMessage("Send data fault");
        }
    }
}

void MainWindow::modbusSendFinishedHandle(void)
{
    int rowCount;
    QString itemText;
    uint i;

    QTableWidget* table = ui->tableWidget;

    rowCount = table->rowCount();

    rowCount++;

    table->setRowCount(rowCount);

    itemText = QString().number(timesample.elapsed());

    table->setItem(rowCount-1, 0, new QTableWidgetItem(itemText));

    if(modbus->replyErrorString().isEmpty())
    {

        ui->statusBar->showMessage(tr("Finished, OK"));

        itemText = QString().number(modbus->readRegister->slaveAddress());

        table->setItem(rowCount-1, 1, new QTableWidgetItem(itemText));

        table->setItem(rowCount-1, 2, new QTableWidgetItem(QString("Master")));

        if(modbus->functionCode() == QModbusPdu::ReadHoldingRegisters)
        {
            itemText = "Read Holding Register (0x03) ";
            table->setItem(rowCount-1, 3, new QTableWidgetItem(itemText));

            itemText =  tr("Return Data Length(Half Word): %1, Data:")
                .arg(QString().number(modbus->readRegister->valueCount()));

            for( i = 0; i < modbus->readRegister->valueCount(); i++)
            {
                itemText.append(tr(" 0x%1")
                                .arg(QString()
                                     .number(modbus->readRegister->value(i), 16).toUpper()));
            }

            //更新绘图
            ploter->plotPoint(((qint16)modbus->readRegister->value(4)), plotCount, 0);
            ploter->plotPoint(((qint16)modbus->readRegister->value(5)), plotCount, 1);
            ploter->plotPoint(((qint16)modbus->readRegister->value(6)), plotCount, 2);
            ploter->plotPoint(((qint16)modbus->readRegister->value(0)), plotCount, 3);
            ploter->plotPoint(((qint16)modbus->readRegister->value(1)), plotCount, 4);
            ploter->plotPoint(((qint16)modbus->readRegister->value(2)), plotCount++, 5);

            //更新表格
            table->setItem(rowCount-1, 4, new QTableWidgetItem(itemText));
        }
        else if(modbus->functionCode() == QModbusPdu::WriteSingleRegister)
        {
            itemText = "Write Single Register (0x06) ";
            table->setItem(rowCount-1, 3, new QTableWidgetItem(itemText));

            itemText =  tr("Register Address: 0x%1, Write Data:")
                                .arg(QString().number(modbus->writeRegister->startAddress(), 16).toUpper());

            itemText.append(tr(" 0x%1").arg(QString()
                                            .number(modbus->writeRegister->value(), 16).toUpper()));

            table->setItem(rowCount-1, 4, new QTableWidgetItem(itemText));
        }
    }
    else
    {
        itemText = modbus->replyErrorString();
        QTableWidgetItem* newItem = new QTableWidgetItem(itemText);
        newItem->setTextColor(Qt::red);

        table->setItem(rowCount-1, 4, newItem);
        ui->statusBar->showMessage(tr("Finished, %1").arg(modbus->replyErrorString()));
    }
}

void MainWindow::updateTable(QTableWidget* table, QString from)
{
    int rowCount;
    QString itemText;

    rowCount = table->rowCount();

    rowCount++;

    table->setRowCount(rowCount);

    itemText = QString().number(timesample.elapsed());
    table->setItem(rowCount-1, 0, new QTableWidgetItem(itemText));

    table->setItem(rowCount-1, 1, new QTableWidgetItem(from));

    itemText = QString().number(serialSendDialog->slaveAddress(), 16);
    table->setItem(rowCount-1, 2, new QTableWidgetItem(itemText));

    if(serialSendDialog->functionCode() == 0x03)
    {
        itemText = "Read Holding Register (0x03) ";
        table->setItem(rowCount-1, 3, new QTableWidgetItem(itemText));

        itemText = tr("Start Register Address(hex): %1, Read Length %2")
                .arg(QString().number(serialSendDialog->registerAddress(), 16))
                .arg(serialSendDialog->data());

        table->setItem(rowCount-1, 4, new QTableWidgetItem(itemText));
    }
    else if(serialSendDialog->functionCode() == 0x06)
    {
        itemText = "Write Single Register (0x06) ";
        table->setItem(rowCount-1, 3, new QTableWidgetItem(itemText));

        itemText = tr("Register Address: 0x%1, Write Data: 0x%2")
                .arg(QString().number(serialSendDialog->registerAddress(), 16).toUpper())
                .arg(QString().number(serialSendDialog->data(), 16).toUpper());

        table->setItem(rowCount-1, 4, new QTableWidgetItem(itemText));
    }
}

void MainWindow::updateCaption(void)
{
    if(modbus->state() == QModbusDevice::ConnectedState)
    {
        //请求数据
        modbus->sendRequest(serialSendDialog);
    }
}

void MainWindow::on_actionPlot_Setting_triggered()
{
    if(plotSettingDialog->exec() == 1)
    {
        ploter->setupPloter(plotSettingDialog->plotCurveCount());
    }
}

void MainWindow::on_actionExport_triggered()
{
    QString receiveFileName;
    QString lineData;
    int i, j;

    QVector<double> data;

    /*打开文件*/
    receiveFileName = QFileDialog::getSaveFileName(this,
        tr("Open Receive File"), "/home/life", tr("Any files(*)"));

    receiveFile.setFileName(receiveFileName);

    if(!receiveFile.open(QIODevice::ReadWrite))
    {
        ui->statusBar->showMessage(tr("Open file fault"));
        return;
    }
    else
    {
        ui->statusBar->showMessage(tr("Open file ok"));
    }

    for(i = 0; i < ploter->plotDataCount(); i++)
    {
        lineData.clear();

        QVector<double> data = ploter->plotDataValue(i);

        for(j = 0; j <data.length(); j++)
        {
            if( j < (data.length() - 1))
            {
                lineData += QString("%1,").arg(data[j]);
            }
            else
            {
                lineData += QString("%1").arg(data[j]);
            }
        }

        lineData += QString("\n");

        receiveFile.write(lineData.toStdString().data());
    }

    receiveFile.close();
}
