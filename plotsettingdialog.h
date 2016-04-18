#ifndef PLOTSETTINGDIALOG_H
#define PLOTSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class PlotSettingDialog;
}

class PlotSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotSettingDialog(QWidget *parent = 0);
    ~PlotSettingDialog();

    int plotCurveCount(void);

private:
    Ui::PlotSettingDialog *ui;
};

#endif // PLOTSETTINGDIALOG_H
