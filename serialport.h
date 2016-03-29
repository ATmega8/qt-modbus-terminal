#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

#include "serialsettingdialog.h"


class SerialPort : public QSerialPort
{
public:
    SerialPort();
    bool setting(SerialSettingDialog* setting);

private:
};

#endif // SERIALPORT_H
