#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modbusSerial = new SerialPort();
    serialDialog = new SerialSettingDialog();
    serialSendDialog = new SerialSendDialog();

    connect(modbusSerial, SIGNAL(readyRead()), this, SLOT(serialRead()));
}

MainWindow::~MainWindow()
{
    delete modbusSerial;
    delete serialDialog;
    delete serialSendDialog;
    delete ui;
}

void MainWindow::serialRead(void)
{

}

void MainWindow::on_actionOpen_triggered()
{
   if( !modbusSerial->open(QIODevice::ReadWrite) )
   {
       ui->statusBar->showMessage( tr("Serial Port %1 Open Error: %2")
                                   .arg(modbusSerial->portName()).arg(modbusSerial->errorString()));
   }
   else
   {
       ui->statusBar->showMessage(tr("Serial Port %1 Open OK").arg((modbusSerial->portName())));
   }
}


void MainWindow::on_actionClose_triggered()
{
    modbusSerial->close();
    ui->statusBar->showMessage(tr("Serial Port %1 Closed").arg((modbusSerial->portName())));
}

void MainWindow::on_actionSetting_triggered()
{
    serialDialog->exec();
    modbusSerial->setting(serialDialog);
}

void MainWindow::on_actionSend_triggered()
{
    QString hexBuff;
    int i = 0;
    char sendData;
    bool ok;

    serialSendDialog->exec();

    if(!serialSendDialog->hexstatus())
    {
        if(modbusSerial->write(serialSendDialog->writedata().toStdString().data(),
                            serialSendDialog->writedata().length()))
        {
            ui->statusBar->showMessage(tr("wirte serial error"));
        }
        else
        {
            ui->statusBar->showMessage(tr("wirte serial succeed"));
        }
     }
    else
    {
        hexBuff = serialSendDialog->writedata();

        for(i = 0; i < hexBuff.length()/2; i++)
        {
            sendData = (QString(hexBuff[2*i]) + QString(hexBuff[2*i+1])).toUInt(&ok, 16);

            if(ok)
                modbusSerial->write(&sendData, 1);
            else
            {
                ui->statusBar->showMessage(tr("convert error"));
                return;
            }
        }
             ui->statusBar->showMessage(tr("wirte serial succeed"));
    }
}
