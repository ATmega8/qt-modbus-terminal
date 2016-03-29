#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serialport.h>
#include <serialsettingdialog.h>
#include <serialsenddialog.h>

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
    void serialRead(void);

private slots:
    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionSetting_triggered();

    void on_actionSend_triggered();

private:
    Ui::MainWindow *ui;
    SerialPort* modbusSerial;
    SerialSettingDialog* serialDialog;
    SerialSendDialog* serialSendDialog;

};

#endif // MAINWINDOW_H
