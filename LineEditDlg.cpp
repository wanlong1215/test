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
    if (NULL == _o)
    {
        _o = new proLine(DatabaseProxy::instance().concentrator(_parentId));
    }

    _o->name = ui->leName->text();
    _o->type = ui->cbType->currentIndex();
    _o->addr = ui->leAddr->text();
    _o->preAddr = ui->lePreId->text();
    _o->nextAddr = ui->leNextId->text();
    _o->workerID = ui->leWorkId->text().toInt();

    accept();
}

void LineEditDlg::on_btnCancel_clicked()
{
    reject();
}

void LineEditDlg::init()
{
    if (NULL != _o)
    {
        ui->leName->setText(_o->name);
        ui->cbType->setCurrentIndex(_o->type);
        ui->leAddr->setText(_o->addr);
        ui->lePreId->setText(_o->preAddr);
        ui->leNextId->setText(_o->nextAddr);
        ui->leWorkId->setText(QString::number(_o->workerID));
    }
    else
    {
        ui->leName->setText(QStringLiteral("线段"));
    }
}
