#include "ModifyMonitorDlg.h"
#include "ui_ModifyMonitorDlg.h"

ModifyMonitorDlg::ModifyMonitorDlg(proMonitor *o, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyMonitorDlg)
{
    ui->setupUi(this);

    _o = o;
    if (nullptr != _o)
    {
        ui->leMonitorName->setText(_o->name);
        ui->leMonitorAddr->setText(_o->addr);
        ui->lePreMonitorAddr->setText(_o->preAddr);
        ui->leNextMonitorAddr->setText(_o->nextAddr);
    }
}

ModifyMonitorDlg::~ModifyMonitorDlg()
{
    delete ui;
}

void ModifyMonitorDlg::on_btnOK_clicked()
{
    if (nullptr != _o)
    {
        _o->name = ui->leMonitorName->text();
        _o->addr = ui->leMonitorAddr->text();
        _o->preAddr =  ui->lePreMonitorAddr->text();
        _o->nextAddr = ui->leNextMonitorAddr->text();

        if (DatabaseProxy::instance().modifyMonitor(_o))
        {
            accept();
        }
    }
}

void ModifyMonitorDlg::on_btnCancel_clicked()
{
    reject();
}
