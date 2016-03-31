#ifndef SERIALSENDDIALOG_H
#define SERIALSENDDIALOG_H

#include <QDialog>
#include <QModbusRequest>
#include <QString>

namespace Ui {
class SerialSendDialog;
}

class SerialSendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSendDialog(QWidget *parent = 0);
    ~SerialSendDialog();

    bool hexstatus(void);
    QString writedata(void);

    quint16 data(void);
    quint16 slaveAddress(void);
    quint16 registerAddress(void);
    QModbusRequest::FunctionCode functionCode(void);

private slots:

private:
    Ui::SerialSendDialog *ui;
};

#endif // SERIALSENDDIALOG_H
