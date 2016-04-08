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

    msgBox = new QMessageBox();

   timesample.start();

    connect(modbus, SIGNAL(stateChanged(QModbusDevice::State)),
            this, SLOT(modbusStateChangeHandle(QModbusDevice::State)));
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
        if(modbus->state() == QModbusDevice::ConnectedState)
        {
            QModbusRequest request(serialSendDialog->functionCode(),
                                serialSendDialog->registerAddress(),
                                serialSendDialog->data());

            reply = modbus->sendRawRequest(request, serialSendDialog->slaveAddress());
            ui->statusBar->showMessage("Start send data");
            connect(reply, SIGNAL(finished()), this, SLOT(modbusSendFinishedHandle()));

            /*等待完成*/
            updateTable(ui->tableWidget, "Master");
        }
        else
        {
            msgBox->setText("Serial NOT Open");
            msgBox->exec();
        }
    }
}

void MainWindow::modbusStateChangeHandle(QModbusDevice::State state)
{
    switch(state)
    {
        case QModbusDevice::UnconnectedState:
            ui->statusBar->showMessage("Unconnected");
        break;

        case QModbusDevice::ConnectingState:
            ui->statusBar->showMessage("Connecting");
            break;

         case QModbusDevice::ConnectedState:
            ui->statusBar->showMessage("Connected");
            break;

        default:
            break;
    }
}

void MainWindow::modbusSendFinishedHandle(void)
{
    int rowCount;
    QString itemText;
    int i;
    uint16_t res;

    QTableWidget* table = ui->tableWidget;

    rowCount = table->rowCount();

    rowCount++;

    table->setRowCount(rowCount);

    itemText = QString().number(timesample.elapsed());
    table->setItem(rowCount-1, 0, new QTableWidgetItem(itemText));

    if( reply->error() == QModbusDevice::NoError )
    {

        itemText = QString().number(reply->serverAddress());
        table->setItem(rowCount-1, 1, new QTableWidgetItem(itemText));

        table->setItem(rowCount-1, 2, new QTableWidgetItem(QString("Master")));

        ui->statusBar->showMessage(tr("Finished, OK"));

        if(serialSendDialog->functionCode() == 0x03)
        {
            itemText = "Read Holding Register (0x03) ";
            table->setItem(rowCount-1, 3, new QTableWidgetItem(itemText));

            if( reply->rawResult().isValid())
            {
                for( i = 0; i < (reply->rawResult().dataSize() - 1)/2 + 1; i++)
                {
                    if( i == 0)
                    {
                        itemText =  tr("Return Data Length(Byte): %1, Data:")
                           .arg(QString().number(reply->rawResult().data()[0]));
                    }
                    else
                    {
                        res = reply->rawResult().data()[2*i-1];
                        res <<= 8;
                        res |= (0x00FF & reply->rawResult().data()[2*i]);
                        itemText.append(tr(" 0x%1").arg(QString().number(res, 16).toUpper()));

                    }
                }
            }
            else
            {
                itemText = "Data is Invalid";
            }

            table->setItem(rowCount-1, 4, new QTableWidgetItem(itemText));
        }
        else if(serialSendDialog->functionCode() == 0x06)
        {
            itemText = "Write Single Register (0x06) ";
            table->setItem(rowCount-1, 3, new QTableWidgetItem(itemText));

            if( reply->rawResult().isValid())
            {
                    res = reply->rawResult().data()[0];
                    res <<= 8;
                    res |= (0x00FF & reply->rawResult().data()[1]);
                    itemText =  tr("Register Address: 0x%1, Write Data:")
                                .arg(QString().number(res, 16).toUpper());

                    res = reply->rawResult().data()[2];
                    res <<= 8;
                    res |= (0x00FF & reply->rawResult().data()[3]);
                    itemText.append(tr(" 0x%1").arg(QString().number(res, 16).toUpper()));
            }
            else
            {
                itemText = "Data is Invalid";
            }

            table->setItem(rowCount-1, 4, new QTableWidgetItem(itemText));
        }
    }
    else
    {
        itemText = reply->errorString();
        QTableWidgetItem* newItem = new QTableWidgetItem(itemText);
        newItem->setTextColor(Qt::red);

        table->setItem(rowCount-1, 4, newItem);
        ui->statusBar->showMessage(tr("Finished, %1").arg(reply->errorString()));
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
