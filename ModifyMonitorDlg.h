#ifndef MODIFYMONITORDLG_H
#define MODIFYMONITORDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class ModifyMonitorDlg;
}

class ModifyMonitorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyMonitorDlg(proMonitor *o, QWidget *parent = 0);
    ~ModifyMonitorDlg();

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::ModifyMonitorDlg *ui;

    proMonitor *_o;
};

#endif // MODIFYMONITORDLG_H
