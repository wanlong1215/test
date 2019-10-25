#include "LineEditDlg.h"
#include "ui_LineEditDlg.h"
#include <QDebug>

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
    _o->preAddr = QString::number(_mapLine.key(ui->cbPreLine->currentText()));
    _o->workerID = _map.key(ui->cbWorker->currentText());
    _o->Ratio = ui->leRatio->text().toDouble();

    qDebug() << _mapLine << ui->cbPreLine->currentText() << _mapLine.key(ui->cbPreLine->currentText());
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

    auto lst = DatabaseProxy::instance().getOrganizations();
    foreach (auto o1, lst) {
        foreach (auto o2, o1->lst) {
            foreach (auto o3, o2->lst) {
                foreach (auto o4, o3->lst) {
                    foreach (auto o5, o4->lst) {
                        if (nullptr == _o) {
                            if (o5->id != _parentId) {
                                continue;
                            }
                        } else {
                            if (o5->id != _o->parent->id) {
                                continue;
                            }
                        }
                        foreach (auto o6, o5->lst) {
                            if (o6 != _o) {
                                _mapLine.insert(o6->id, o6->name);
                            }
                        }
                    }
                }
            }
        }
    }

    _mapLine.insert(-1, QStringLiteral("空"));
    qDebug() << _mapLine;
    ui->cbPreLine->addItems(_mapLine.values());

    if (NULL != _o)
    {
        ui->leName->setText(_o->name);
        ui->cbType->setCurrentIndex(_o->type);
        ui->cbPreLine->setCurrentText(_mapLine.value(_o->preAddr.toInt()));
        ui->cbWorker->setCurrentText(_map.value(_o->workerID));
        ui->leRatio->setText(QString::number(_o->Ratio));
    }
    else
    {
        ui->leName->setText(QStringLiteral("线段"));
        on_cbType_currentIndexChanged(0);
    }
}

void LineEditDlg::on_cbType_currentIndexChanged(int index)
{
    ui->lbRatio->setVisible(2 == index);
    ui->leRatio->setVisible(2 == index);
    ui->lbRatio1->setVisible(2 == index);
}
