#include "modbus.h"

#include <QMessageBox>

Modbus::Modbus()
{
    readRegister = new ModbusReadHoldRegister;
    writeRegister = new ModbusWriteSingleRegister;
}

void Modbus::setting(SerialSettingDialog* setting)
{
    this->setConnectionParameter(QModbusDevice::SerialPortNameParameter, setting->device());
    this->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, setting->baudrate());
    this->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, setting->databits());
    this->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, setting->stopbits());
    this->setConnectionParameter(QModbusDevice::SerialParityParameter, setting->parity());
}

bool Modbus::sendRequest(SerialSendDialog* sendDialog)
{
    if(this->state() == QModbusDevice::ConnectedState)
    {
        QModbusDataUnit unit =
            QModbusDataUnit(QModbusDataUnit::HoldingRegisters,
                sendDialog->registerAddress(),
                sendDialog->data());

        if(sendDialog->functionCode() == 0x03)
        {
            funcCode = QModbusPdu::ReadHoldingRegisters;

            reply = this->sendReadRequest(unit,
            sendDialog->slaveAddress());
        }
        else if(sendDialog->functionCode() == 0x06)
        {
            QModbusRequest request(QModbusRequest::WriteSingleRegister,
                sendDialog->registerAddress(),
                sendDialog->data());

            funcCode = QModbusPdu::WriteSingleRegister;

            reply = this->sendRawRequest(request, sendDialog->slaveAddress());
         }

         //等待回复
         if(!reply->isFinished())
         {
            connect(reply, &QModbusReply::finished, this, &Modbus::requestFinishedHandle);
         }
         else //广播情况下无回复
         {
            delete reply;
         }

    }
    else
    {
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setText("Serial NOT Open");
        msgBox->exec();
        delete msgBox;

        /*Serial Port Not Open*/
        return false;
    }

    /*Send OK*/
    return true;
}

QModbusPdu::FunctionCode Modbus::functionCode(void)
{
    return funcCode;
}

void Modbus::requestFinishedHandle(void)
{
    if(reply->error() == QModbusDevice::NoError)
    {
        //获得回复类型
        if(funcCode == QModbusPdu::ReadHoldingRegisters)
        {
            readRegister->processReadHoldRegisterReply(reply);
        }
        else if(funcCode == QModbusPdu::WriteSingleRegister)
        {
            writeRegister->processWriteSingleRegister(reply);
        }
    }
    else
    {
        replyErrString = reply->errorString();
    }

    reply->deleteLater();
    emit finished();
}


QString Modbus::replyErrorString(void)
{
    return replyErrString;
}
