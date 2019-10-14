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

private slots:
    void on_btnAdd_clicked();

    void on_btnModify_clicked();

    void on_btnDelete_clicked();

private:
    void init();

private:
    Ui::WorkerConfigDlg *ui;
};

#endif // WORKERCONFIGDLG_H
