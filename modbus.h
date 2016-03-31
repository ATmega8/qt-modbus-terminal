#ifndef MODBUS_H
#define MODBUS_H

#include <QtSerialBus>
#include <QIODevice>

#include "serialsettingdialog.h"

class Modbus: public QModbusRtuSerialMaster
{
public:
    Modbus();
    void setting(SerialSettingDialog* setting);

private:
    QIODevice* device;
};

#endif // MODBUS_H
