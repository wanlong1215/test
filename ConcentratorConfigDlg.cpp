#include "ConcentratorConfigDlg.h"
#include "ui_ConcentratorConfigDlg.h"

ConcentratorConfigDlg::ConcentratorConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConcentratorConfigDlg)
{
    ui->setupUi(this);
}

ConcentratorConfigDlg::~ConcentratorConfigDlg()
{
    delete ui;
}
