#include "ModifyTerminalDlg.h"
#include "ui_ModifyTerminalDlg.h"

ModifyTerminalDlg::ModifyTerminalDlg(proTerminal *o, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyTerminalDlg)
{
    ui->setupUi(this);
    _o = o;

    init();
}

ModifyTerminalDlg::~ModifyTerminalDlg()
{
    delete ui;
}

proTerminal *ModifyTerminalDlg::terminal()
{
    return _o;
}

void ModifyTerminalDlg::init()
{
    if (NULL == _o)
    {
        return;
    }
    ui->groupBox->setVisible(2 == _o->type);

    ui->deDate1->setDate(QDate::fromJulianDay(_o->installTime));
    ui->leAddr1->setText(QString::number(_o->addr));
    ui->lePreAddr1->setText(QString::number(_o->preAddr));
    ui->leNextAddr1->setText(QString::number(_o->nextAddr));
    ui->leRoute11->setText(QString::number(_o->RouteState1));
    ui->leRoute21->setText(QString::number(_o->RouteState2));
    ui->leRoute31->setText(QString::number(_o->RouteState3));
    ui->leRoute41->setText(QString::number(_o->RouteState4));
    ui->leRoute51->setText(QString::number(_o->RouteState5));
    ui->leRoute61->setText(QString::number(_o->RouteState6));
    ui->leHighPressureValue->setText(QString::number(_o->highPressureValue));
    ui->cbHighPressureSymbol->setCurrentIndex(_o->highPressureSymbol);
    ui->leHighPressureOffset->setText(QString::number(_o->highPressureOffset));
    ui->leLowPressureValue->setText(QString::number(_o->lowPressureValue));
}

void ModifyTerminalDlg::on_btnOK_clicked()
{
    _o->installTime = ui->deDate1->date().toJulianDay();
    _o->addr = ui->leAddr1->text().toInt();
    _o->preAddr = ui->lePreAddr1->text().toInt();
    _o->nextAddr = ui->leNextAddr1->text().toInt();
    _o->RouteState1 = ui->leRoute11->text().toInt();
    _o->RouteState2 = ui->leRoute21->text().toInt();
    _o->RouteState3 = ui->leRoute31->text().toInt();
    _o->RouteState4 = ui->leRoute41->text().toInt();
    _o->RouteState5 = ui->leRoute51->text().toInt();
    _o->RouteState6 = ui->leRoute61->text().toInt();

    _o->highPressureValue = ui->leHighPressureValue->text().toInt();
    _o->highPressureSymbol = ui->cbHighPressureSymbol->currentIndex();
    _o->highPressureOffset = ui->leHighPressureOffset->text().toInt();
    _o->lowPressureValue = ui->leLowPressureValue->text().toInt();

    accept();
}

void ModifyTerminalDlg::on_btnCancel_clicked()
{
    reject();
}
