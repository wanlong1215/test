#include "DatabaseConfigDlg.h"
#include "ui_DatabaseConfigDlg.h"

DatabaseConfigDlg::DatabaseConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseConfigDlg)
{
    ui->setupUi(this);
}

DatabaseConfigDlg::~DatabaseConfigDlg()
{
    delete ui;
}
