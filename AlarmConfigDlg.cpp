#include "AlarmConfigDlg.h"
#include "ui_AlarmConfigDlg.h"

AlarmConfigDlg::AlarmConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmConfigDlg)
{
    ui->setupUi(this);
}

AlarmConfigDlg::~AlarmConfigDlg()
{
    delete ui;
}
