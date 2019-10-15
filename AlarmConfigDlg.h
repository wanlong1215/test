#ifndef ALARMCONFIGDLG_H
#define ALARMCONFIGDLG_H

#include <QDialog>

namespace Ui {
class AlarmConfigDlg;
}

class AlarmConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AlarmConfigDlg(QWidget *parent = 0);
    ~AlarmConfigDlg();

private slots:
    void on_leFilter_textChanged(const QString &arg1);

private:
    void init();

private:
    Ui::AlarmConfigDlg *ui;
};

#endif // ALARMCONFIGDLG_H
