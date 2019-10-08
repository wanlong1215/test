#ifndef WORKERCONFIGDLG_H
#define WORKERCONFIGDLG_H

#include <QDialog>

namespace Ui {
class WorkerConfigDlg;
}

class WorkerConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WorkerConfigDlg(QWidget *parent = 0);
    ~WorkerConfigDlg();

private:
    Ui::WorkerConfigDlg *ui;
};

#endif // WORKERCONFIGDLG_H
