#ifndef ALARMCONFIGDLG_H
#define ALARMCONFIGDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

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
    void onSubCompayChanged();
    void onAsmoChanged();
    void onLineChanged();
    void onWorkerChanged();
    void onQuery();

    void on_btnExport_clicked();

private:
    void init();

    bool getName(int lineId, QString &subCompany, QString &asmo, QString &lineName);

private:
    Ui::AlarmConfigDlg *ui;

    QList<proCompany *> _company; // 临时记录，增加速度
};

#endif // ALARMCONFIGDLG_H
