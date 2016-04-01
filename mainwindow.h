#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateTime>

#include "serialsettingdialog.h"
#include "serialsenddialog.h"
#include "modbus.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void modbusStateChangeHandle(QModbusDevice::State state);
    void modbusSendFinishedHandle(void);

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSetting_triggered();
    void on_actionSend_triggered();

private:
    Ui::MainWindow *ui;
    SerialSettingDialog* serialDialog;
    SerialSendDialog* serialSendDialog;
    Modbus* modbus;

    QModbusReply* reply;

    QTime timesample;

    void updateTable(QTableWidget* table, QString from);
};

#endif // MAINWINDOW_H
