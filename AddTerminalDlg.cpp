#include "AddTerminalDlg.h"
#include "ui_AddTerminalDlg.h"
#include "AppSession.h"

AddTerminalDlg::AddTerminalDlg(QList<proCompany *> lst, int parentId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTerminalDlg)
{
    ui->setupUi(this);

    _com = lst;
    _parentId = parentId;
    _monitor = nullptr;
    _o1 = NULL;
    _o2 = NULL;
    _o3 = NULL;

    init();
}

AddTerminalDlg::~AddTerminalDlg()
{
    delete ui;
}

proMonitor *AddTerminalDlg::monitor()
{
    return _monitor;
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
    ui->leMonitorName->setText(QStringLiteral("监测点"));

    QStringList lstName;
    lstName.append(QStringLiteral("空"));
    auto lst = _com;
    foreach (auto o1, lst) {
        foreach (auto o2, o1->lst) {
            foreach (auto o3, o2->lst) {
                foreach (auto o4, o3->lst) {
                    foreach (auto o5, o4->lst) {
                        foreach (auto o6, o5->lst) {
                            foreach (auto o7, o6->lst) {
                                lstName.append(o7->name);
                                _map.insert(o7->id, o7->name);
                            }
                        }
                    }
                }
            }
        }
    }

    ui->cbPreMonitor->addItems(lstName);
}

void AddTerminalDlg::on_btnCancel_clicked()
{
    this->reject();
}

void AddTerminalDlg::on_btnOK_clicked()
{
    proLine *line = DatabaseProxy::instance().line(_parentId);

    _monitor = new proMonitor(line);

    _monitor->name = ui->leMonitorName->text();
    _monitor->addr = ui->leMonitorAddr->text().toInt();
    _monitor->PreMonitorID = _map.key(ui->cbPreMonitor->currentText());

    DatabaseProxy::instance().addMonitor(_monitor, _parentId);

    _o1 = new proTerminal(_monitor);
    _o2 = new proTerminal(_monitor);
    _o3 = new proTerminal(_monitor);

    _o1->name = QStringLiteral("A相");
    _o1->type = "A";
    _o1->ConcentratorAddr = line->parent->concentratorAddr;
    _o1->installTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime());
    _o1->addr = ui->leAddr1->text().toInt();
    _o1->preAddr = 0;
    _o1->nextAddr = 0;
    _o1->RouteState1 = ui->leRoute11->text().toInt();
    _o1->RouteState2 = ui->leRoute21->text().toInt();
    _o1->RouteState3 = ui->leRoute31->text().toInt();
    _o1->RouteState4 = ui->leRoute41->text().toInt();
    _o1->RouteState5 = ui->leRoute51->text().toInt();
    _o1->RouteState6 = ui->leRoute61->text().toInt();
    _o1->index=ui->leTimes1->text().toInt();
    _o1->highPressureValue = ui->leHighPressureValue1->text().toInt();
    _o1->highPressureOffset = ui->leHighPressureOffset1->text().toInt();
    _o1->TerminalCurrentTime=_o1->installTime;
    _o1->highPressureSymbol = 0;
    _o1->lowPressureValue = 0;

    _o2->name = QStringLiteral("B相");
    _o2->type = "B";
    _o2->ConcentratorAddr = line->parent->concentratorAddr;
    _o2->installTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime());
    _o2->addr = ui->leAddr2->text().toInt();
    _o2->preAddr = 0;
    _o2->nextAddr = 0;
    _o2->RouteState1 = ui->leRoute12->text().toInt();
    _o2->RouteState2 = ui->leRoute22->text().toInt();
    _o2->RouteState3 = ui->leRoute32->text().toInt();
    _o2->RouteState4 = ui->leRoute42->text().toInt();
    _o2->RouteState5 = ui->leRoute52->text().toInt();
    _o2->RouteState6 = ui->leRoute62->text().toInt();
    _o2->index = ui->leTimes2->text().toInt();
    _o2->highPressureValue = ui->leHighPressureValue2->text().toInt();
    _o2->highPressureOffset = ui->leHighPressureOffset2->text().toInt();
    _o2->TerminalCurrentTime=_o2->installTime;
    _o2->highPressureSymbol = 0;
    _o2->lowPressureValue = 0;

    _o3->name = QStringLiteral("C相");
    _o3->type = "C";
    _o3->ConcentratorAddr = line->parent->concentratorAddr;
    _o3->installTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime());
    _o3->addr = ui->leAddr3->text().toInt();
    _o3->preAddr = 0;
    _o3->nextAddr = 0;
    _o3->RouteState1 = ui->leRoute31->text().toInt();
    _o3->RouteState2 = ui->leRoute32->text().toInt();
    _o3->RouteState3 = ui->leRoute33->text().toInt();
    _o3->RouteState4 = ui->leRoute43->text().toInt();
    _o3->RouteState5 = ui->leRoute53->text().toInt();
    _o3->RouteState6 = ui->leRoute63->text().toInt();
    _o3->index = ui->leTimes3->text().toInt();
    _o3->highPressureValue = ui->leHighPressureValue3->text().toInt();
    _o3->highPressureOffset = ui->leHighPressureOffset3->text().toInt();
    _o3->TerminalCurrentTime=_o3->installTime;
    _o3->highPressureSymbol = 0;
    _o3->lowPressureValue = 0;

    accept();
}
