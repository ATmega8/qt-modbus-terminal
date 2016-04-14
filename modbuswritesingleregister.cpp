#include "modbuswritesingleregister.h"

ModbusWriteSingleRegister::ModbusWriteSingleRegister()
{

}

void ModbusWriteSingleRegister::processWriteSingleRegister(QModbusReply* reply)
{
    quint16 res;

    serverAddress = reply->serverAddress();

    res = reply->rawResult().data()[0];
    res <<= 8;
    res |= (0x00FF & reply->rawResult().data()[1]);

    startAddr = res;


    res = reply->rawResult().data()[2];
    res <<= 8;
    res |= (0x00FF & reply->rawResult().data()[3]);

    data = res;
}

quint16 ModbusWriteSingleRegister::value(void)
{
    return data;
}

int ModbusWriteSingleRegister::startAddress(void)
{
    return startAddr;
}

int ModbusWriteSingleRegister::slaveAddress(void)
{
    return serverAddress;
}
