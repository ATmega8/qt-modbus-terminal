#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qwt_plot.h>
#include <qwt_plot_grid.h>

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

    plotTimer = new QTimer(this);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(updateCaption()));
    plotTimer->start(1000);

    plotCount  = 0.0;

    //Qwt Plot Widget Init
    //Axis
    ui->qwtPlot->setAxisMaxMajor(QwtPlot::xBottom, 5);
    ui->qwtPlot->setAxisMaxMinor(QwtPlot::xBottom, 9);
    ui->qwtPlot->setAxisMaxMajor(QwtPlot::yRight, 5);
    ui->qwtPlot->setAxisMaxMinor(QwtPlot::yRight, 9);

    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "Time");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "Attitude");

    //Grid
    QwtPlotGrid* plotGrid = new QwtPlotGrid;

    plotGrid->enableXMin(true);
    plotGrid->setMajorPen(Qt::black, 0, Qt::DotLine);
    plotGrid->setMinorPen(Qt::gray, 0, Qt::DotLine);
    plotGrid->attach(ui->qwtPlot);

    //Curve
    d_curve1 = new QwtPlotCurve( "Attitude" );
    d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve1->setPen( Qt::red);
    d_curve1->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    d_curve1->setYAxis( QwtPlot::yLeft );
    d_curve1->attach(ui->qwtPlot);

    //Marker
    d_marker1 = new QwtPlotMarker();
    d_marker1->setValue( 0.0, 0.0 );
    d_marker1->setLineStyle( QwtPlotMarker::VLine );
    d_marker1->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker1->setLinePen( Qt::green, 0, Qt::DashDotLine );
    d_marker1->attach(ui->qwtPlot);

    //background
    ui->qwtPlot->setCanvasBackground(QColor("white"));

    ui->qwtPlot->setAutoReplot(true);

    timesample.start();

    connect(modbus, SIGNAL(stateChanged(QModbusDevice::State)),
            this, SLOT(modbusStateChangeHandle(QModbusDevice::State)));

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
    uint i;

    QTableWidget* table = ui->tableWidget;

    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());

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

    reply->deleteLater();
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

}
