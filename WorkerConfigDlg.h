#ifndef WORKERCONFIGDLG_H
#define WORKERCONFIGDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class WorkerConfigDlg;
}

class WorkerConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WorkerConfigDlg(QWidget *parent = 0);
    ~WorkerConfigDlg();

private slots:
    void on_btnAdd_clicked();

    void on_btnModify_clicked();

    void on_btnDelete_clicked();

    void on_leFilter_textChanged(const QString &arg1);

    void onSubCompayChanged();
    void onAsmoChanged();

    void onQuery();

    void on_btnExport_clicked();

private:
    void init();

private:
    Ui::WorkerConfigDlg *ui;

    QList<proCompany *> _company; // 临时记录，增加速度
};

#endif // WORKERCONFIGDLG_H
