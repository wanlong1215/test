#include "SummaryWidget.h"
#include "ui_SummaryWidget.h"
#include "DatabaseProxy.h"
#include "OrganizationTreeWidgetItem.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include "AppSession.h"

SummaryWidget::SummaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryWidget)
{
    ui->setupUi(this);

    _currentConcentrator = NULL;
    ui->trwOrganization->setHeaderHidden(true);
    ui->trwOrganization->viewport()->installEventFilter(this);
}

SummaryWidget::~SummaryWidget()
{
    delete ui;
}

void SummaryWidget::init()
{
    // clear first
    ui->trwOrganization->clear();
    ui->tawHistoryDetail->clear();
    ui->tawRealTimeDetail->clear();

    QList<proCompany *> lstCompany = DatabaseProxy::instance().getOrganizations();

    // add company, level=1
    foreach(proCompany * o1, lstCompany) {
        OrganizationTreeWidgetItem *i1 = new OrganizationTreeWidgetItem(o1, ui->trwOrganization);

        ui->trwOrganization->addTopLevelItem(i1);

        // add subcompany, level=2
        foreach(proSubCompany * o2, o1->lst) {
            OrganizationTreeWidgetItem *i2 = new OrganizationTreeWidgetItem(o2, i1);

            i1->addChild(i2);

            // add amso, level=3
            foreach(proAmso * o3, o2->lst) {
                OrganizationTreeWidgetItem *i3 = new OrganizationTreeWidgetItem(o3, i2);

                i2->addChild(i3);

                // add route, level=4
                foreach(proRoute * o4, o3->lst) {
                    OrganizationTreeWidgetItem *i4 = new OrganizationTreeWidgetItem(o4, i3);

                    i3->addChild(i4);

                    // add concentrator, level=5
                    foreach(proConcentrator * o5, o4->lst) {
                        OrganizationTreeWidgetItem *i5 = new OrganizationTreeWidgetItem(o5, i4);

                        i4->addChild(i5);
                    }

                    i4->setExpanded(true);
                }

                i3->setExpanded(true);
            }

            i2->setExpanded(true);
        }

        i1->setExpanded(true);
    }

    _timer = new QTimer(this);
    _timer->setInterval(30*1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(ui->rbAutoQuery, SIGNAL(toggled(bool)), this, SLOT(onAutoQueryToggled(bool)));
    connect(ui->rbQuickQuery, SIGNAL(toggled(bool)), this, SLOT(onQuickQueryToggled(bool)));
    connect(ui->rbAbsoluteQuery, SIGNAL(toggled(bool)), this, SLOT(onAbsoluteQueryToggled(bool)));
    connect(ui->btnQuery, &QPushButton::clicked, this, &SummaryWidget::onHistoryQuery);

    ui->dteBegin->setDateTime(QDateTime::currentDateTime().addSecs(-60 * 60));
    ui->dteEnd->setDateTime(QDateTime::currentDateTime());
    ui->rbAutoQuery->setChecked(true);

    _currentConcentrator = DatabaseProxy::instance().firstConcentrator();

    onAutoQueryToggled(true);
}
void SummaryWidget::onAutoQueryToggled(bool b)
{
    ui->cbQuickCycle->setEnabled(!b);
    ui->dteBegin->setEnabled(!b);
    ui->dteEnd->setEnabled(!b);

    // clear data, show first concentrator
    while (0 != ui->tawHistoryDetail->rowCount())
    {
        ui->tawHistoryDetail->removeRow(0);
    }

    onHistoryQuery();
}
void SummaryWidget::onQuickQueryToggled(bool b)
{
    ui->cbQuickCycle->setEnabled(b);
    ui->dteBegin->setEnabled(!b);
    ui->dteEnd->setEnabled(!b);

    // clear data
    while (0 != ui->tawHistoryDetail->rowCount())
    {
        ui->tawHistoryDetail->removeRow(0);
    }

    onHistoryQuery();
}
void SummaryWidget::onAbsoluteQueryToggled(bool b)
{
    ui->cbQuickCycle->setEnabled(!b);
    ui->dteBegin->setEnabled(b);
    ui->dteEnd->setEnabled(b);

    // clear data
    while (0 != ui->tawHistoryDetail->rowCount())
    {
        ui->tawHistoryDetail->removeRow(0);
    }

    onHistoryQuery();
}

void SummaryWidget::onHistoryQuery()
{
    if (NULL == _currentConcentrator)
    {
        ui->scrollAreaWidgetContents_2->setFixedWidth(ui->scrollArea_2->width());
        ui->wgtHistoryGraphics->init(_currentConcentrator);
    }
    else
    {
        ui->wgtHistoryGraphics->init(_currentConcentrator);
        ui->scrollAreaWidgetContents_2->setFixedWidth(ui->wgtHistoryGraphics->width());
    }

    QStringList lstHeader;

    ui->tawHistoryDetail->setColumnCount(10);
    lstHeader << QStringLiteral("公司") << QStringLiteral("供电分公司") << QStringLiteral("供电所") << QStringLiteral("线路") << QStringLiteral("集中器") << QStringLiteral("线段") << QStringLiteral("监测点") << QStringLiteral("A相电流") << QStringLiteral("B相电流") << QStringLiteral("C相电流");// << QStringLiteral("采集时间");
    ui->tawHistoryDetail->setHorizontalHeaderLabels(lstHeader);
    ui->tawHistoryDetail->horizontalHeader()->setStyleSheet("QHeaderView::section{font:20pt '微软雅黑';color: black;};");
    for (int i = 0; i < 10; i++)
    {
        ui->tawHistoryDetail->setColumnWidth(i, 150);
    }

    if (NULL == _currentConcentrator)
    {
        return;
    }

    QList<showData> lst;
    if (ui->rbAutoQuery->isChecked()) {
        DatabaseProxy::instance().historyDataByTime(lst, _currentConcentrator->concentratorAddr);
    } else if (ui->rbQuickQuery->isChecked()) {
        qint64 beginTime = 0;
        qint64 endTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime());
        switch (ui->cbQuickCycle->currentIndex())
        {
        case 0: // 3 hours
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addSecs(-1*3*60*60));
            break;
        case 1: // 1 day
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addDays(-1*1));
            break;
        case 2: // 3 days
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addDays(-1*3));
            break;
        case 3: // 7 days
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addDays(-1*7));
            break;
        case 4: // 1 mounth
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addMonths(-1*1));
            break;
        case 5: // 3 mounths
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addMonths(-1*3));
            break;
        case 6: // 6 mounths
            beginTime = AppSession::instance().toInt64Time(QDateTime::currentDateTime().addMonths(-1*6));
            break;
        default:
            break;
        }

        DatabaseProxy::instance().historyDataByTime(lst, _currentConcentrator->concentratorAddr, beginTime, endTime);
    } else if (ui->rbAbsoluteQuery->isChecked()) {
        qint64 beginTime = AppSession::instance().toInt64Time(ui->dteBegin->dateTime());
        qint64 endTime = AppSession::instance().toInt64Time(ui->dteEnd->dateTime());

        DatabaseProxy::instance().historyDataByTime(lst, _currentConcentrator->concentratorAddr, beginTime, endTime);
    }

    ui->tawHistoryDetail->setRowCount(lst.count());
    ui->tawHistoryDetail->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < lst.count(); i++)
    {
        showData data = lst.at(i);
        ui->tawHistoryDetail->setItem(i, 0, new QTableWidgetItem(data.company));
        ui->tawHistoryDetail->setItem(i, 1, new QTableWidgetItem(data.subCompany));
        ui->tawHistoryDetail->setItem(i, 2, new QTableWidgetItem(data.amso));
        ui->tawHistoryDetail->setItem(i, 3, new QTableWidgetItem(data.route));
        ui->tawHistoryDetail->setItem(i, 4, new QTableWidgetItem(data.concentrator));
        ui->tawHistoryDetail->setItem(i, 5, new QTableWidgetItem(data.line));
        ui->tawHistoryDetail->setItem(i, 6, new QTableWidgetItem(data.monitor));
        ui->tawHistoryDetail->setItem(i, 7, new QTableWidgetItem(QString::number(data.valueA.iValue)));
        ui->tawHistoryDetail->setItem(i, 8, new QTableWidgetItem(QString::number(data.valueB.iValue)));
        ui->tawHistoryDetail->setItem(i, 9, new QTableWidgetItem(QString::number(data.valueC.iValue)));
    }
}

void SummaryWidget::onRealtimeQuery()
{
    if (NULL == _currentConcentrator)
    {
        ui->scrollAreaWidgetContents->setFixedWidth(ui->scrollArea->width());
        ui->wgtGraphics->init(_currentConcentrator);
    }
    else
    {
        ui->wgtGraphics->init(_currentConcentrator);
        ui->scrollAreaWidgetContents->setFixedWidth(ui->wgtGraphics->width());
    }

    QStringList lstHeader;

    ui->tawRealTimeDetail->setColumnCount(11);
    lstHeader << QStringLiteral("选择") << QStringLiteral("公司") << QStringLiteral("供电分公司") << QStringLiteral("供电所") << QStringLiteral("线路") << QStringLiteral("集中器") << QStringLiteral("线段") << QStringLiteral("监测点") << QStringLiteral("A相电流") << QStringLiteral("B相电流") << QStringLiteral("C相电流");// << QStringLiteral("采集时间");
    ui->tawRealTimeDetail->setHorizontalHeaderLabels(lstHeader);
    ui->tawRealTimeDetail->horizontalHeader()->setStyleSheet("QHeaderView::section{font:20pt '微软雅黑';color: black;};");
    for (int i = 0; i < 11; i++)
    {
        ui->tawRealTimeDetail->setColumnWidth(i, i==0?80:150);
    }
    if (NULL == _currentConcentrator)
    {
        return;
    }

    int routCount = 0;
    foreach(auto o, _currentConcentrator->lst) {
        routCount += o->lst.count();
    }

    ui->tawRealTimeDetail->setRowCount(routCount);
    ui->tawRealTimeDetail->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int i = 0;
    foreach(auto o6, _currentConcentrator->lst) {
        foreach (auto o, o6->lst) {
            if (o->lst.count() != 3) {
                continue;
            }

            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setCheckState(Qt::Unchecked);
            item->setData(10, o->parent->parent->concentratorAddr);
            item->setData(11, o->lst.at(0)->addr);
            item->setData(12, o->lst.at(1)->addr);
            item->setData(13, o->lst.at(2)->addr);
           ui->tawRealTimeDetail->setItem(i, 0, item);
            ui->tawRealTimeDetail->setItem(i, 1, new QTableWidgetItem(o6->parent->parent->parent->parent->parent->name));
            ui->tawRealTimeDetail->setItem(i, 2, new QTableWidgetItem(o6->parent->parent->parent->parent->name));
            ui->tawRealTimeDetail->setItem(i, 3, new QTableWidgetItem(o6->parent->parent->parent->name));
            ui->tawRealTimeDetail->setItem(i, 4, new QTableWidgetItem(o6->parent->parent->name));
            ui->tawRealTimeDetail->setItem(i, 5, new QTableWidgetItem(o6->parent->name));
            ui->tawRealTimeDetail->setItem(i, 6, new QTableWidgetItem(o6->name));
            ui->tawRealTimeDetail->setItem(i, 7, new QTableWidgetItem(o->name));
            ui->tawRealTimeDetail->setItem(i, 8, new QTableWidgetItem(QString("")));
            ui->tawRealTimeDetail->setItem(i, 9, new QTableWidgetItem(QString("")));
            ui->tawRealTimeDetail->setItem(i, 10, new QTableWidgetItem(QString("")));
            i++;
        }
    }
}

void SummaryWidget::onTimeout()
{
    vector<DATA> datas;
    if (DatabaseProxy::instance().realTimeData(datas))
    {
        for (int i = 0; i < datas.size(); i++)
        {
            DATA dt = datas[i];

            for (int j = 0; j < ui->tawRealTimeDetail->rowCount(); j++) {
                auto item = ui->tawRealTimeDetail->item(j, 0);
                if (item->checkState() == Qt::Checked && item->data(10).toInt() == dt.ConcentratorAddr) {
                    if (item->data(11).toInt() == dt.TerminalAddr) {
                        ui->tawRealTimeDetail->item(j, 8)->setText(QString::number(dt.iValue));
                    } else if (item->data(12).toInt() == dt.TerminalAddr) {
                        ui->tawRealTimeDetail->item(j, 9)->setText(QString::number(dt.iValue));
                    } else if (item->data(13).toInt() == dt.TerminalAddr) {
                        ui->tawRealTimeDetail->item(j, 10)->setText(QString::number(dt.iValue));
                    }
                }
            }
        }

    }
}

bool SummaryWidget::eventFilter(QObject*obj, QEvent*e)
{
    if (obj == ui->trwOrganization->viewport())
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent*mouseEv = dynamic_cast<QMouseEvent*>(e);
            QTreeWidgetItem*item = ui->trwOrganization->itemAt(mouseEv->pos());
            OrganizationTreeWidgetItem*itemO = dynamic_cast<OrganizationTreeWidgetItem*>(item);
            proConcentrator*con = NULL;
            if (NULL != itemO)
            {
                if (5 == itemO->_level)
                {
                    con = itemO->_o5;
                }
                else
                {
                    con = NULL;
                }
            }
            else
            {
                con = NULL;
            }

            if (NULL == con)
            {
                // clear data
                _currentConcentrator = con;
            }
            else
            {
                if (_currentConcentrator == con)
                {
                    // nothing
                }
                else
                {
                    _currentConcentrator = con;
                }
            }

            onHistoryQuery();
            onRealtimeQuery();
        }
    }

    return QWidget::eventFilter(obj, e);
}

void SummaryWidget::on_btnReadRealtime_clicked()
{
    // insert read command
    int commandNum = 0;
    for (int i = 0; i < ui->tawRealTimeDetail->rowCount(); i++) {
        auto item = ui->tawRealTimeDetail->item(i, 0);
        if (item->checkState() == Qt::Checked) {
            commandNum++;
            auto command = new proCommand;
            command->Commandtype = 1;
            command->UserID = AppSession::instance().user.id;
            command->ConcentratorAddr = item->data(10).toInt();
            command->TerminalAddr = item->data(11).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(12).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(13).toInt();
            DatabaseProxy::instance().addCommand(command);
        }
    }

    if (commandNum > 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("读取指令已下发!"));
    } else if (commandNum == 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请勾选需要读取的监测点!"));
    }

    _timer->start();
}

void SummaryWidget::on_btnStopRead_clicked()
{
    // insert stop command
    int commandNum = 0;
    for (int i = 0; i < ui->tawRealTimeDetail->rowCount(); i++) {
        auto item = ui->tawRealTimeDetail->item(i, 0);
        if (item->checkState() == Qt::Checked) {
            commandNum++;
            auto command = new proCommand;
            command->Commandtype = 0;
            command->UserID = AppSession::instance().user.id;
            command->ConcentratorAddr = item->data(10).toInt();
            command->TerminalAddr = item->data(11).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(12).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(13).toInt();
            DatabaseProxy::instance().addCommand(command);
        }
    }

    if (commandNum > 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("停止读取指令已下发!"));
    } else if (commandNum == 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请勾选需要停止读取的监测点!"));
    }

    _timer->stop();
}
