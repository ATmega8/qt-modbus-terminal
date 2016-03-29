#ifndef SERIALSETTINGDIALOG_H
#define SERIALSETTINGDIALOG_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QString>
#include <QSerialPort>

namespace Ui {
class SerialSettingDialog;
}

class SerialSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSettingDialog(QWidget *parent = 0);
    ~SerialSettingDialog();

    qint32 baudrate(void);
    QString device(void);
    QSerialPort::DataBits databits(void);
    QSerialPort::StopBits stopbits(void);
    QSerialPort::Parity parity(void);

private slots:
    void on_comboBox_5_activated(const QString &arg1);

private:
    Ui::SerialSettingDialog *ui;

    void serialInfoUpdate(void);
};

#endif // SERIALSETTINGDIALOG_H
