﻿#include "Elec.h"
#include "DatabaseConfigDlg.h"
#include "WorkerConfigDlg.h"
#include "UserConfigDlg.h"
#include "ServerConfigDlg.h"
#include "PopupInformation.h"
#include <QPropertyAnimation>
#include <QFile>

Elec::Elec(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

    _lstMenuButton << ui.btnSummary << ui.btnConfigure;
    on_btnSummary_clicked();
}

Elec::~Elec()
{
    while (!_lstPopupWidget.isEmpty())
    {
        delete _lstPopupWidget.takeFirst();
    }
}

void Elec::on_btnSummary_clicked()
{
    updateCheckStatus(ui.btnSummary);
    ui.stackedWidget->setCurrentIndex(0);
    ui.wgtSummary->init();
}

void Elec::on_btnConfigure_clicked()
{
    updateCheckStatus(ui.btnConfigure);
    ui.stackedWidget->setCurrentIndex(1);
    ui.wgtConfigure->init();
}

void Elec::on_btnDatabase_clicked()
{
    DatabaseConfigDlg *dlg = new DatabaseConfigDlg(this);

    dlg->exec();
}

void Elec::on_benServer_clicked()
{
    ServerConfigDlg *dlg = new ServerConfigDlg(this);

    dlg->exec();
}

void Elec::on_btnWorker_clicked()
{
    WorkerConfigDlg *dlg = new WorkerConfigDlg(this);

    dlg->exec();
}

void Elec::on_btnUser_clicked()
{
    UserConfigDlg *dlg = new UserConfigDlg(this);

    dlg->exec();
}

void Elec::on_btnQuit_clicked()
{
    this->close();
    deleteLater();
}

void Elec::on_pushButton_clicked()
{
    PopupInformation *w = new PopupInformation();

    connect(w, SIGNAL(beforeClose(QWidget*)), this, SLOT(onRemovePopupWidget(QWidget*)));

    _lstPopupWidget.append(w);

    w->show();

    QPoint ptRB = this->mapToGlobal(this->rect().bottomRight());

    QPropertyAnimation* animation = new QPropertyAnimation(w, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(ptRB.x() - 300, ptRB.y() - 100, 300, 200));
    animation->setEndValue(QRect(ptRB.x() - 300, ptRB.y() - 300, 300, 200));
    animation->setEasingCurve(QEasingCurve::CosineCurve);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Elec::onRemovePopupWidget(QWidget *obj)
{
    if (_lstPopupWidget.contains(obj))
    {
        _lstPopupWidget.removeOne(obj);
    }
}

void Elec::updateCheckStatus(QPushButton *btn)
{
    for (int i = 0; i < _lstMenuButton.count(); i++)
    {
        _lstMenuButton.at(i)->setChecked(_lstMenuButton.at(i) == btn);
    }
}
