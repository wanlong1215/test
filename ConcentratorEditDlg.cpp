#include "ConcentratorEditDlg.h"
#include "ui_ConcentratorEditDlg.h"

ConcentratorEditDlg::ConcentratorEditDlg(proConcentrator *o, int parentId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConcentratorEditDlg)
{
    ui->setupUi(this);

    _o = o;
    _parentId = parentId;
    init();
}

ConcentratorEditDlg::~ConcentratorEditDlg()
{
    delete ui;
}

proConcentrator *ConcentratorEditDlg::concentrator()
{
    return _o;
}

void ConcentratorEditDlg::init()
{
    if (NULL != _o)
    {
        ui->leName->setText(_o->name);
        ui->leDestIP->setText(_o->destIp);
        ui->leDestPort->setText(_o->destPort);
        ui->leType->setText(_o->type);
        ui->leInstallAddr->setText(_o->installAddr);
        ui->leApName->setText(_o->apName);
        ui->leApProtocol->setText(_o->apProtocol);
        ui->leSimNumber->setText(_o->strSimCard);
        ui->sbTerminalTimer->setValue(_o->terminalTimer);
        ui->sbConTimer->setValue(_o->concentratorTimer);
        ui->sbHeartTimer->setValue(_o->heartTimer);
        ui->sbGprsReConTimer->setValue(_o->gprsReConnectTimer);
        ui->sbGprsSigStrength->setValue(_o->gprsSignalStrength);
        ui->sbSaveTimer->setValue(_o->saveTimer);
        ui->sbLoopTimer->setValue(_o->wirelessSearchTimer);
        ui->sbConAddr->setValue(_o->concentratorAddr);
        ui->leTime->setDateTime(QDateTime::fromMSecsSinceEpoch(_o->ConcentratorCurrentTime));
        if (_o->SelfReportOnOff == 0 || _o->SelfReportOnOff == 1) {
            ui->cbPower->setCurrentIndex(_o->SelfReportOnOff);
        }
    }
    else
    {
        ui->leName->setText(QStringLiteral("集中器"));
        ui->leTime->setDateTime(QDateTime::currentDateTime());
    }
}

void ConcentratorEditDlg::on_btnOK_clicked()
{
    bool insert = false;
    if (NULL == _o)
    {
        insert = true;
        _o = new proConcentrator(DatabaseProxy::instance().route(_parentId));
    }

    _o->name = ui->leName->text();
    _o->destIp = ui->leDestIP->text();
    _o->destPort = ui->leDestPort->text();
    _o->type = ui->leType->text();
    _o->installAddr = ui->leInstallAddr->text();
    _o->apName = ui->leApName->text();
    _o->apProtocol = ui->leApProtocol->text();
    _o->strSimCard = ui->leSimNumber->text();
    _o->terminalTimer = ui->sbTerminalTimer->value();
    _o->concentratorTimer = ui->sbConTimer->value();
    _o->heartTimer = ui->sbHeartTimer->value();
    _o->gprsReConnectTimer = ui->sbGprsReConTimer->value();
    _o->gprsSignalStrength = ui->sbGprsSigStrength->value();
    _o->saveTimer = ui->sbSaveTimer->value();
    _o->wirelessSearchTimer = ui->sbLoopTimer->value();
    _o->concentratorAddr = ui->sbConAddr->value();
    _o->ConcentratorCurrentTime = ui->leTime->dateTime().toMSecsSinceEpoch();
    _o->SelfReportOnOff = ui->cbPower->currentIndex();

    if (insert) {
        //
    } else {
        if (DatabaseProxy::instance().modifyConcentrator(_o) ) {
            foreach (auto line, _o->lst) {
                foreach (auto con, line->lst) {
                    foreach (auto ter, con->lst) {
                        ter->ConcentratorAddr = _o->concentratorAddr;
                        DatabaseProxy::instance().modifyTerminal(ter);
                    }
                }
            }
        }
    }
    accept();
}

void ConcentratorEditDlg::on_btnCancel_clicked()
{
    reject();
}
