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

private:
    Ui::AlarmConfigDlg *ui;
};

#endif // ALARMCONFIGDLG_H
