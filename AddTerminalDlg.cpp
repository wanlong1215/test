#include "AddTerminalDlg.h"
#include "ui_AddTerminalDlg.h"

AddTerminalDlg::AddTerminalDlg(int parentId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTerminalDlg)
{
    ui->setupUi(this);

    _parentId = parentId;
    _o1 = NULL;
    _o2 = NULL;
    _o3 = NULL;

    init();
}

AddTerminalDlg::~AddTerminalDlg()
{
    delete ui;
}

proTerminal *AddTerminalDlg::terminal1()
{
    return _o1;
}

proTerminal *AddTerminalDlg::terminal2()
{
    return _o2;
}

proTerminal *AddTerminalDlg::terminal3()
{
    return _o3;
}

void AddTerminalDlg::init()
{
    ui->leName1->setText("终端A");
    ui->deDate1->setDate(QDate::currentDate());
    ui->leName2->setText("终端B");
    ui->deDate2->setDate(QDate::currentDate());
    ui->leName3->setText("终端C");
    ui->deDate3->setDate(QDate::currentDate());
}

void AddTerminalDlg::on_btnCancel_clicked()
{
    this->reject();
}

void AddTerminalDlg::on_btnOK_clicked()
{
    proLine *line = DatabaseProxy::instance().line(_parentId);

    proMonitor *po = new proMonitor(line);

    po->name = "监视器";

    DatabaseProxy::instance().addMonitor(po, _parentId);

    _o1 = new proTerminal(po);
    _o2 = new proTerminal(po);
    _o3 = new proTerminal(po);

    _o1->name = ui->leName1->text();
    _o1->type = 0;
    _o1->index = ui->sbIndex1->value();
    _o1->installTime = ui->deDate1->date().toJulianDay();
    _o1->addr = ui->leAddr1->text().toInt();
    _o1->preAddr = ui->lePreAddr1->text().toInt();
    _o1->nextAddr = ui->leNextAddr1->text().toInt();
    _o1->RouteState1 = ui->leRoute11->text().toInt();
    _o1->RouteState2 = ui->leRoute21->text().toInt();
    _o1->RouteState3 = ui->leRoute31->text().toInt();
    _o1->RouteState4 = ui->leRoute41->text().toInt();
    _o1->RouteState5 = ui->leRoute51->text().toInt();
    _o1->RouteState6 = ui->leRoute61->text().toInt();

    _o2->name = ui->leName2->text();
    _o2->type = 1;
    _o2->index = ui->sbIndex2->value();
    _o2->installTime = ui->deDate2->date().toJulianDay();
    _o2->addr = ui->leAddr2->text().toInt();
    _o2->preAddr = ui->lePreAddr2->text().toInt();
    _o2->nextAddr = ui->leNextAddr2->text().toInt();
    _o2->RouteState1 = ui->leRoute12->text().toInt();
    _o2->RouteState2 = ui->leRoute22->text().toInt();
    _o2->RouteState3 = ui->leRoute32->text().toInt();
    _o2->RouteState4 = ui->leRoute42->text().toInt();
    _o2->RouteState5 = ui->leRoute52->text().toInt();
    _o2->RouteState6 = ui->leRoute62->text().toInt();

    _o3->name = ui->leName3->text();
    _o3->type = 2;
    _o3->index = ui->sbIndex3->value();
    _o3->installTime = ui->deDate3->date().toJulianDay();
    _o3->addr = ui->leAddr3->text().toInt();
    _o3->preAddr = ui->lePreAddr3->text().toInt();
    _o3->nextAddr = ui->leNextAddr3->text().toInt();
    _o3->RouteState1 = ui->leRoute13->text().toInt();
    _o3->RouteState2 = ui->leRoute23->text().toInt();
    _o3->RouteState3 = ui->leRoute33->text().toInt();
    _o3->RouteState4 = ui->leRoute43->text().toInt();
    _o3->RouteState5 = ui->leRoute53->text().toInt();
    _o3->RouteState6 = ui->leRoute63->text().toInt();
    _o3->highPressureValue = ui->leHighPressureValue->text().toInt();
    _o3->highPressureSymbol = ui->cbHighPressureSymbol->currentIndex();
    _o3->highPressureOffset = ui->leHighPressureOffset->text().toInt();
    _o3->lowPressureValue = ui->leLowPressureValue->text().toInt();

    accept();
}
