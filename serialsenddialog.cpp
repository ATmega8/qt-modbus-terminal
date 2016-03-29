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

QString SerialSendDialog::writedata(void)
{
    return ui->lineEdit->text();
}

bool SerialSendDialog::hexstatus(void)
{
   return ui->checkBox->isChecked();
}
