#ifndef MODBUS_H
#define MODBUS_H

#include <QtSerialBus>
#include <QIODevice>
#include <QByteArray>

#include "serialsettingdialog.h"
#include "serialsenddialog.h"
#include "modbusreadholdregister.h"
#include "modbuswritesingleregister.h"

class Modbus: public QModbusRtuSerialMaster
{
    Q_OBJECT

public:
    Modbus();
    void setting(SerialSettingDialog* setting);
    bool sendRequest(SerialSendDialog* sendDialog);
    QModbusPdu::FunctionCode functionCode(void);

    ModbusReadHoldRegister* readRegister;
    ModbusWriteSingleRegister* writeRegister;
    QString replyErrorString(void);

private:

    QModbusReply* reply;

    QModbusPdu::FunctionCode funcCode;
    QModbusDataUnit unit;
    QVector<quint16> data;
    uint             dataCount;
    int serverAddress;
    QString replyErrString;

private slots:
    void requestFinishedHandle(void);

signals:
    void finished(void);

};

#endif // MODBUS_H
