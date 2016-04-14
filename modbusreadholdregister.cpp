#include "modbusreadholdregister.h"
#include "modbus.h"

ModbusReadHoldRegister::ModbusReadHoldRegister()
{

}

void ModbusReadHoldRegister::processReadHoldRegisterReply(QModbusReply* reply)
{
    serverAddress = reply->serverAddress();
    data = reply->result().values();
    dataCount = reply->result().valueCount();
    startAddr = reply->result().startAddress();
}

quint16 ModbusReadHoldRegister::value(int index)
{
    return data[index];
}

int ModbusReadHoldRegister::startAddress(void)
{
    return startAddr;
}

uint ModbusReadHoldRegister::valueCount(void)
{
    return dataCount;
}

int ModbusReadHoldRegister::slaveAddress(void)
{
    return serverAddress;
}
