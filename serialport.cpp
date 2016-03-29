#include "serialport.h"

SerialPort::SerialPort()
{

}

bool SerialPort::setting(SerialSettingDialog* setting)
{
    bool ok = true;

    this->setPortName(setting->device());
    ok &= this->setBaudRate(setting->baudrate());
    ok &= this->setDataBits(setting->databits());
    ok &= this->setStopBits(setting->stopbits());
    ok &= this->setParity(setting->parity());

    return ok;
}
