#ifndef MODBUSREADHOLDREGISTER_H
#define MODBUSREADHOLDREGISTER_H

#include <QObject>
#include <QtSerialBus>

class ModbusReadHoldRegister
{
public:
    ModbusReadHoldRegister();

    void processReadHoldRegisterReply(QModbusReply* reply);

    quint16 value(int index);
    int     startAddress(void);
    uint    valueCount(void);
    int     slaveAddress(void);

private:
    QVector<quint16> data;
    uint             dataCount;
    int serverAddress;
    int startAddr;
};

#endif // MODBUSREADHOLDREGISTER_H
