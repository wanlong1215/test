#ifndef ADDWORKERDLG_H
#define ADDWORKERDLG_H

#include <QDialog>
#include "DatabaseProxy.h"

namespace Ui {
class AddWorkerDlg;
}

class AddWorkerDlg : public QDialog
{
    Q_OBJECT

public:
    AddWorkerDlg(QWidget *parent = 0);
    AddWorkerDlg(proWorker *u, QWidget *parent = 0);
    ~AddWorkerDlg();

    proWorker *worker();

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    Ui::AddWorkerDlg *ui;

    QMap<int, QString> _map;
    proWorker *_u;
};

#endif // ADDWORKERDLG_H
