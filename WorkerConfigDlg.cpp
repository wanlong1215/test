#include "WorkerConfigDlg.h"
#include "ui_WorkerConfigDlg.h"

WorkerConfigDlg::WorkerConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkerConfigDlg)
{
    ui->setupUi(this);
}

WorkerConfigDlg::~WorkerConfigDlg()
{
    delete ui;
}
