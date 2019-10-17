#include "LineEditDlg.h"
#include "ui_LineEditDlg.h"

LineEditDlg::LineEditDlg(proLine *o, int parentId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineEditDlg)
{
    ui->setupUi(this);

    _o = o;
    _parentId = parentId;
    init();
}

LineEditDlg::~LineEditDlg()
{
    delete ui;
}

proLine *LineEditDlg::line()
{
    return _o;
}

void LineEditDlg::on_btnOK_clicked()
{
    bool insert = false;
    if (NULL == _o)
    {
        insert = true;
        _o = new proLine(DatabaseProxy::instance().concentrator(_parentId));
    }

    _o->name = ui->leName->text();
    _o->type = ui->cbType->currentIndex();
    _o->addr = ui->leAddr->text();
    _o->preAddr = ui->lePreId->text();
    _o->nextAddr = ui->leNextId->text();
    _o->workerID = _map.key(ui->cbWorker->currentText());

    if (insert) {
        //DatabaseProxy::instance().addLine(_o, _parentId);
    } else {
        DatabaseProxy::instance().modifyLine(_o);
    }
    accept();
}

void LineEditDlg::on_btnCancel_clicked()
{
    reject();
}

void LineEditDlg::init()
{
    auto workers = DatabaseProxy::instance().workers();
    foreach (auto worker, workers) {
        _map.insert(worker.id, worker.name);
    }

    ui->cbWorker->addItems(_map.values());

    if (NULL != _o)
    {
        ui->leName->setText(_o->name);
        ui->cbType->setCurrentIndex(_o->type);
        ui->leAddr->setText(_o->addr);
        ui->lePreId->setText(_o->preAddr);
        ui->leNextId->setText(_o->nextAddr);
        ui->cbWorker->setCurrentText(_map.value(_o->workerID));
    }
    else
    {
        ui->leName->setText(QStringLiteral("线段"));
    }
}
