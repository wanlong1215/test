#include "ServerConfigDlg.h"
#include "ui_ServerConfigDlg.h"

ServerConfigDlg::ServerConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerConfigDlg)
{
    ui->setupUi(this);
}

ServerConfigDlg::~ServerConfigDlg()
{
    delete ui;
}
