#include "modbus.h"

Modbus::Modbus()
{

}

void Modbus::setting(SerialSettingDialog* setting)
{
    this->setConnectionParameter(QModbusDevice::SerialPortNameParameter, setting->device());
    this->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, setting->baudrate());
    this->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, setting->databits());
    this->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, setting->stopbits());
    this->setConnectionParameter(QModbusDevice::SerialParityParameter, setting->parity());
}
