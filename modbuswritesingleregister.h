#ifndef MODBUSWRITESINGLEREGISTER_H
#define MODBUSWRITESINGLEREGISTER_H

#include <QObject>
#include <QtSerialBus>

class ModbusWriteSingleRegister
{
public:
    ModbusWriteSingleRegister();

    void processWriteSingleRegister(QModbusReply* reply);

    quint16 value(void);
    int     startAddress(void);
    int     slaveAddress(void);

private:
    quint16 data;
    int serverAddress;
    int startAddr;
};

#endif // MODBUSWRITESINGLEREGISTER_H
