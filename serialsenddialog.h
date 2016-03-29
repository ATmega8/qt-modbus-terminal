#ifndef SERIALSENDDIALOG_H
#define SERIALSENDDIALOG_H

#include <QDialog>

namespace Ui {
class SerialSendDialog;
}

class SerialSendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSendDialog(QWidget *parent = 0);
    ~SerialSendDialog();

    bool hexstatus(void);
    QString writedata(void);

private slots:

private:
    Ui::SerialSendDialog *ui;
};

#endif // SERIALSENDDIALOG_H
