#include "plotsettingdialog.h"
#include "ui_plotsettingdialog.h"

PlotSettingDialog::PlotSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotSettingDialog)
{
    ui->setupUi(this);
}

int PlotSettingDialog::plotCurveCount(void)
{
    return ui->lineEdit_5->text().toInt();
}

PlotSettingDialog::~PlotSettingDialog()
{
    delete ui;
}
