#include "AddWorkerDlg.h"
#include "ui_AddWorkerDlg.h"
#include "DatabaseProxy.h"

AddWorkerDlg::AddWorkerDlg(QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddWorkerDlg)
{
    ui->setupUi(this);

    _u = nullptr;
    auto amsos = DatabaseProxy::instance().amsos();
    foreach (auto o, amsos) {
        _map.insert(o->id, o->name);
    }

    ui->cbAsmo->addItems(_map.values());
}

AddWorkerDlg::AddWorkerDlg(proWorker *u, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWorkerDlg)
{
    ui->setupUi(this);

    _u = u;
    auto amsos = DatabaseProxy::instance().amsos();
    foreach (auto o, amsos) {
        _map.insert(o->id, o->name);
    }

    ui->cbAsmo->addItems(_map.values());

    ui->leName->setText(_u->name);
    ui->lePhone->setText(_u->phone);
    ui->leRemarks->setText(_u->remarks);
    ui->cbAsmo->setCurrentText(_map.value(_u->amsoId));
}

AddWorkerDlg::~AddWorkerDlg()
{
    delete ui;
}

proWorker *AddWorkerDlg::worker()
{
    return _u;
}

void AddWorkerDlg::on_btnOK_clicked()
{
    bool isAdd = false;
    if (nullptr == _u)
    {
        isAdd = true;
        _u = new proWorker;
    }

    _u->name = ui->leName->text();
    _u->amsoId = _map.key(ui->cbAsmo->currentText());
    _u->phone = ui->lePhone->text();
    _u->remarks = ui->leRemarks->text();

    if (isAdd)
    {
        if (-1 != DatabaseProxy::instance().addWorker(_u))
        {
            accept();
        }
    }
    else
    {
        if ( DatabaseProxy::instance().modifyWorker(_u))
        {
            accept();
        }
    }
}

void AddWorkerDlg::on_btnCancel_clicked()
{
    reject();
}
