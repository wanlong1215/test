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
    }

    auto lst = DatabaseProxy::instance().getOrganizations();
    foreach (auto o1, lst) {
        foreach (auto o2, o1->lst) {
            foreach (auto o3, o2->lst) {
                foreach (auto o4, o3->lst) {
                    foreach (auto o5, o4->lst) {
                        foreach (auto o6, o5->lst) {
                            foreach (auto o7, o6->lst) {
                                if (o7 != _o) {
                                    _map.insert(o7->id, o7->name);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    _map.insert(-1, QStringLiteral("空"));
    ui->cbPreMonitor->addItems(_map.values());
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
        _o->PreMonitorID = _map.key(ui->cbPreMonitor->currentText());

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
