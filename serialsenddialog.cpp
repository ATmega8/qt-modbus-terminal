#include "serialsenddialog.h"
#include "ui_serialsenddialog.h"

SerialSendDialog::SerialSendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSendDialog)
{
    ui->setupUi(this);
}

SerialSendDialog::~SerialSendDialog()
{
    delete ui;
}

quint16 SerialSendDialog::data(void)
{
    bool ok;

    return ui->lineEdit->text().toInt(&ok, 16);

}

quint16 SerialSendDialog::slaveAddress(void)
{
    bool ok;

    return ui->lineEdit_3->text().toInt(&ok, 16);
}

quint16 SerialSendDialog::registerAddress(void)
{
    bool ok;

    return ui->lineEdit_2->text().toInt(&ok, 16);
}

QModbusRequest::FunctionCode SerialSendDialog::functionCode(void)
{
    if(ui->comboBox->currentIndex() == 0)
    {
        return QModbusRequest::ReadHoldingRegisters;
    }
    else if (ui->comboBox->currentIndex() == 1)
    {
        return QModbusRequest::WriteSingleRegister;
    }
    else
    {
        return QModbusRequest::Invalid;
    }
}

