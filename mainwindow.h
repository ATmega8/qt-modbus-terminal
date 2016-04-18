#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QThread>
#include <QVector>
#include <QFile>
#include <QFileDialog>

#include "serialsettingdialog.h"
#include "serialsenddialog.h"
#include "plotsettingdialog.h"
#include "modbus.h"
#include "plot.h"

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

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSetting_triggered();
    void on_actionSend_triggered();
    void updateCaption(void);
    void modbusSendFinishedHandle(void);

    void on_actionPlot_Setting_triggered();

    void on_actionExport_triggered();

private:

    Ui::MainWindow *ui;
    SerialSettingDialog* serialDialog;
    SerialSendDialog* serialSendDialog;
    PlotSettingDialog* plotSettingDialog;
    Modbus* modbus;
    Plot* ploter;

    double plotCount;

    QMessageBox* msgBox;

    QTime timesample;
    QTimer *plotTimer;

    QFile receiveFile;

    void updateTable(QTableWidget* table, QString from);
};


#endif // MAINWINDOW_H
