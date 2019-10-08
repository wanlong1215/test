#include "UserConfigDlg.h"
#include "ui_UserConfigDlg.h"

UserConfigDlg::UserConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserConfigDlg)
{
    ui->setupUi(this);
}

UserConfigDlg::~UserConfigDlg()
{
    delete ui;
}
