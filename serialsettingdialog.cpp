#include "serialsettingdialog.h"
#include "ui_serialsettingdialog.h"

SerialSettingDialog::SerialSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSettingDialog)
{
    ui->setupUi(this);

    this->serialInfoUpdate();
}

SerialSettingDialog::~SerialSettingDialog()
{
    delete ui;
}

void SerialSettingDialog::on_comboBox_5_activated(const QString &arg1)
{
    (void)arg1;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(ui->comboBox_5->findText(info.systemLocation()) == -1)
        {
            ui->comboBox_5->insertItem(0, info.systemLocation());
            ui->comboBox_5->setCurrentIndex(0);
        }
    }
}


void SerialSettingDialog::serialInfoUpdate(void)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_5->insertItem(0, info.systemLocation());
        ui->comboBox_5->setCurrentIndex(0);
    }
}

qint32 SerialSettingDialog::baudrate(void)
{
    return ui->comboBox->currentText().toInt();
}

QString SerialSettingDialog::device(void)
{
    return ui->comboBox_5->currentText();
}

QSerialPort::DataBits SerialSettingDialog::databits(void)
{
    QSerialPort::DataBits dataBits;

    switch(ui->comboBox_2->currentText().toInt())
    {
        case 7:
            dataBits = QSerialPort::Data7;
            break;

        case 8:
            dataBits = QSerialPort::Data8;
            break;

        case 5:
            dataBits = QSerialPort::Data5;
            break;

        case 6:
            dataBits = QSerialPort::Data6;
            break;

        default:
            dataBits = QSerialPort::Data8;
            break;
    }

    return dataBits;
}

QSerialPort::StopBits SerialSettingDialog::stopbits(void)
{
     QSerialPort::StopBits stopBits;

    switch(ui->comboBox_4->currentText().toInt())
    {
        case 1:
            stopBits = QSerialPort::OneStop;
            break;

        case 2:
            stopBits = QSerialPort::TwoStop;
            break;

        case 15:
            stopBits = QSerialPort::OneAndHalfStop;
            break;

        default:
            stopBits = QSerialPort::OneStop;
            break;
    }

    return stopBits;
}

QSerialPort::Parity SerialSettingDialog::parity(void)
{
   return QSerialPort::NoParity;
}

