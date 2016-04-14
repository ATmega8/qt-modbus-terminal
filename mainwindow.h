#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QThread>
#include <QVector>

#include "serialsettingdialog.h"
#include "serialsenddialog.h"
#include "modbus.h"

#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"

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

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSetting_triggered();
    void on_actionSend_triggered();
    void updateCaption(void);
    void modbusSendFinishedHandle(void);

private:

    Ui::MainWindow *ui;
    SerialSettingDialog* serialDialog;
    SerialSendDialog* serialSendDialog;
    Modbus* modbus;

    QMessageBox* msgBox;

    QTime timesample;
    QTimer *plotTimer;

    QwtPlotCurve* d_curve1;
    QwtPlotMarker* d_marker1;

    double plotCount;

    QVector<double> x_data;
    QVector<double> y_data;

    void updateTable(QTableWidget* table, QString from);
};

#endif // MAINWINDOW_H
