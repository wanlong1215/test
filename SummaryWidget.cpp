#include "SummaryWidget.h"
#include "ui_SummaryWidget.h"
#include "DatabaseProxy.h"
#include "OrganizationTreeWidgetItem.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include "AppSession.h"
#include <QFileDialog>
#include <xlsxdocument.h>
#include <QtConcurrent>

SummaryWidget::SummaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryWidget)
{
    ui->setupUi(this);

    _currentConcentrator = NULL;
    ui->trwOrganization->setHeaderHidden(true);
    ui->trwOrganization->viewport()->installEventFilter(this);

    _groupButton = new QButtonGroup(this);
    _groupButton->addButton(ui->rbAutoQuery, 0);
    _groupButton->addButton(ui->rbQuickQuery, 1);
    _groupButton->addButton(ui->rbAbsoluteQuery, 2);

    connect(_groupButton, SIGNAL(buttonClicked(int)), SLOT(onRadioButtonClicked(int)));
    connect(ui->btnQuery, &QPushButton::clicked, this, &SummaryWidget::onHistoryQuery);
}

SummaryWidget::~SummaryWidget()
{
    delete ui;
}

void SummaryWidget::init()
{
    static bool first = true;
    if (first) {
        ui->tawHistoryDetail->clear();
        ui->tawRealTimeDetail->clear();
        ui->wgtHistoryGraphics->init(nullptr);
        ui->wgtGraphics->init(nullptr);
    }
    // clear first
    _map.clear();
    ui->trwOrganization->clear();

    auto beginTime = QDateTime::currentDateTime();
    qDebug() << "begin getOrganizations";
    QList<proCompany *> lstCompany = DatabaseProxy::instance().getOrganizations();
    qDebug() << QString("end getOrganizations, cost time: %1 ms").arg(beginTime.msecsTo(QDateTime::currentDateTime()));

    // add company, level=1
    foreach(proCompany * o1, lstCompany) {
        OrganizationTreeWidgetItem *i1 = new OrganizationTreeWidgetItem(o1, ui->trwOrganization);

        ui->trwOrganization->addTopLevelItem(i1);

        // add subcompany, level=2
        QStringList lstConcentrator;
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

                        lstConcentrator.append(o5->name);
                        i4->addChild(i5);
                    }

                    i4->setExpanded(true);
                }

                i3->setExpanded(true);
            }

            i2->setExpanded(true);
            _map.insert(o2->name, lstConcentrator);
        }

        i1->setExpanded(true);
    }

    _timer = new QTimer(this);
    _timer->setInterval(5*1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    ui->dteBegin->setDateTime(QDateTime::currentDateTime().addSecs(-60 * 60));
    ui->dteEnd->setDateTime(QDateTime::currentDateTime());
    ui->rbAutoQuery->setChecked(true);

    QStringList subNames = _map.keys();
    subNames.prepend(QStringLiteral("全部"));
    QStringList conNames;
    foreach (auto child, _map.values()) {
        conNames.append(child);
    }
    conNames.prepend(QStringLiteral("全部"));
    disconnect(ui->cbSubCompany, SIGNAL(currentIndexChanged(int)), this, SLOT(onSubCompayChanged()));
    disconnect(ui->cbConcentrator, SIGNAL(currentIndexChanged(int)), this, SLOT(onHistoryQuery()));
    ui->cbSubCompany->clear();
    ui->cbConcentrator->clear();
    ui->cbSubCompany->addItems(subNames);
    ui->cbConcentrator->addItems(conNames);
    connect(ui->cbSubCompany, SIGNAL(currentIndexChanged(int)), this, SLOT(onSubCompayChanged()));
    connect(ui->cbConcentrator, SIGNAL(currentIndexChanged(int)), this, SLOT(onHistoryQuery()));

    if (1) {
        QStringList lstHeader;
        ui->tawHistoryDetail->setColumnCount(12);
        lstHeader << QStringLiteral("序号") << QStringLiteral("公司") << QStringLiteral("供电分公司") << QStringLiteral("供电所") << QStringLiteral("线路") << QStringLiteral("集中器") << QStringLiteral("线段") << QStringLiteral("监测点") << QStringLiteral("A相电流") << QStringLiteral("B相电流") << QStringLiteral("C相电流") << QStringLiteral("采集时间");
        ui->tawHistoryDetail->setHorizontalHeaderLabels(lstHeader);
        ui->tawHistoryDetail->horizontalHeader()->setStyleSheet("QHeaderView::section{font:20pt '微软雅黑';color: black;};");
        for (int i = 0; i < 11; i++)
        {
            ui->tawHistoryDetail->setColumnWidth(i, 150);
        }
    }

    if (1) {
        QStringList lstHeader;

        ui->tawRealTimeDetail->setColumnCount(11);
        lstHeader << QStringLiteral("选择") << QStringLiteral("公司") << QStringLiteral("供电分公司") << QStringLiteral("供电所") << QStringLiteral("线路") << QStringLiteral("集中器") << QStringLiteral("线段") << QStringLiteral("监测点") << QStringLiteral("A相电流") << QStringLiteral("B相电流") << QStringLiteral("C相电流");// << QStringLiteral("采集时间");
        ui->tawRealTimeDetail->setHorizontalHeaderLabels(lstHeader);
        ui->tawRealTimeDetail->horizontalHeader()->setStyleSheet("QHeaderView::section{font:20pt '微软雅黑';color: black;};");
        for (int i = 0; i < 11; i++)
        {
            ui->tawRealTimeDetail->setColumnWidth(i, i==0?80:150);
        }
    }

    ui->wgtHistoryGraphics->setType(true);
    ui->wgtGraphics->setType(false);
    _currentConcentrator = DatabaseProxy::instance().firstConcentrator();

    if (first) {
        onRadioButtonClicked(0);
        first = false;
    }
}

void SummaryWidget::onRadioButtonClicked(int id)
{
    ui->cbQuickCycle->setEnabled(id == 1);
    ui->dteBegin->setEnabled(id == 2);
    ui->dteEnd->setEnabled(id == 2);

    // clear data, show first concentrator
    while (0 != ui->tawHistoryDetail->rowCount())
    {
        ui->tawHistoryDetail->removeRow(0);
    }

    onHistoryQuery();
}

void SummaryWidget::onHistoryQuery()
{
    QtConcurrent::run(this, &SummaryWidget::onHistoryQueryExec);
}

void SummaryWidget::onRealtimeQuery()
{
    QtConcurrent::run(this, &SummaryWidget::onRealtimeQueryExec);
}

void SummaryWidget::onHistoryQueryExec()
{
    if (NULL == _currentConcentrator)
    {
        ui->scrollAreaWidgetContents_2->setFixedSize(ui->scrollArea_2->size());
        ui->wgtHistoryGraphics->init(_currentConcentrator);
    }
    else
    {
        ui->wgtHistoryGraphics->init(_currentConcentrator);
        ui->scrollAreaWidgetContents_2->setFixedSize(ui->wgtHistoryGraphics->size());
    }

    if (NULL == _currentConcentrator)
    {
        return;
    }

    QList<showData> lst;
    if (ui->rbAutoQuery->isChecked()) {
        auto beginTime = QDateTime::currentDateTime();
        qDebug() << "begin query history data";
        DatabaseProxy::instance().historyDataByTime(lst, _currentConcentrator->concentratorAddr);
        qDebug() << QString("end query history data, cost time: %1 ms").arg(beginTime.msecsTo(QDateTime::currentDateTime()));
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

    int realCount = 0;
    ui->tawHistoryDetail->setRowCount(lst.count());
    ui->tawHistoryDetail->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < lst.count(); i++)
    {
        showData data = lst.at(i);

        if (ui->cbSubCompany->currentIndex() == 0) {
            //
        } else if (ui->cbSubCompany->currentText() != data.subCompany) {
            continue;
        }

        // filter concentrator
        if (ui->cbConcentrator->currentIndex() == 0) {
            //
        } else if (ui->cbConcentrator->currentText() != data.concentrator) {
            continue;
        }

        ui->tawHistoryDetail->setItem(i, 0, new QTableWidgetItem(QString::number(realCount+1)));
        ui->tawHistoryDetail->setItem(i, 1, new QTableWidgetItem(data.company));
        ui->tawHistoryDetail->setItem(i, 2, new QTableWidgetItem(data.subCompany));
        ui->tawHistoryDetail->setItem(i, 3, new QTableWidgetItem(data.amso));
        ui->tawHistoryDetail->setItem(i, 4, new QTableWidgetItem(data.route));
        ui->tawHistoryDetail->setItem(i, 5, new QTableWidgetItem(data.concentrator));
        ui->tawHistoryDetail->setItem(i, 6, new QTableWidgetItem(data.line));
        ui->tawHistoryDetail->setItem(i, 7, new QTableWidgetItem(data.monitor));
        ui->tawHistoryDetail->setItem(i, 8, new QTableWidgetItem(QString::number(data.valueA.iValue)));
        ui->tawHistoryDetail->setItem(i, 9, new QTableWidgetItem(QString::number(data.valueB.iValue)));
        ui->tawHistoryDetail->setItem(i, 10, new QTableWidgetItem(QString::number(data.valueC.iValue)));
        qint64 showTime = data.valueA.CollectTime;
        showTime = (showTime == 0) ? data.valueB.CollectTime : showTime;
        showTime = (showTime == 0) ? data.valueC.CollectTime : showTime;
        ui->tawHistoryDetail->setItem(i, 11, new QTableWidgetItem(showTime == 0 ? "-" : AppSession::instance().toQDateTime(showTime).toString("yyyy-MM-dd hh:mm")));

        if (data.valueA.intRev1 == 1 || data.valueB.intRev1 == 1 || data.valueC.intRev1 == 1) {
            for (int j = 0; j < 10; j++) {
                ui->tawHistoryDetail->item(i, j)->setBackgroundColor(QColor(255, 105, 180));
            }
        }

        realCount++;
    }
    ui->tawHistoryDetail->setRowCount(realCount);
}

void SummaryWidget::onRealtimeQueryExec()
{
    if (NULL == _currentConcentrator)
    {
        ui->scrollAreaWidgetContents->setFixedSize(ui->scrollArea->size());
        ui->wgtGraphics->init(_currentConcentrator);
    }
    else
    {
        ui->wgtGraphics->init(_currentConcentrator);
        ui->scrollAreaWidgetContents->setFixedSize(ui->wgtGraphics->size());
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

            QTableWidgetItem *item = new QTableWidgetItem(QString::number(i+1));
            item->setCheckState(Qt::Unchecked);
            item->setData(10, o->parent->parent->concentratorAddr);
            item->setData(11, o->addr);
            item->setData(12, o->lst.at(0)->addr);
            item->setData(13, o->lst.at(1)->addr);
            item->setData(14, o->lst.at(2)->addr);
            item->setData(15, o->id);
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
                    auto monitor = DatabaseProxy::instance().monitor(item->data(15).toInt());

                    if (item->data(12).toInt() == dt.TerminalAddr) {
                        if (nullptr != monitor) monitor->rtva = QString::number(dt.iValue);
                        ui->tawRealTimeDetail->item(j, 8)->setText(QString::number(dt.iValue));
                    } else if (item->data(13).toInt() == dt.TerminalAddr) {
                        if (nullptr != monitor) monitor->rtvb = QString::number(dt.iValue);
                        ui->tawRealTimeDetail->item(j, 9)->setText(QString::number(dt.iValue));
                    } else if (item->data(14).toInt() == dt.TerminalAddr) {
                        if (nullptr != monitor) monitor->rtvc = QString::number(dt.iValue);
                        ui->tawRealTimeDetail->item(j, 10)->setText(QString::number(dt.iValue));
                    }
                }
            }
        }

        ui->wgtGraphics->updateValue();
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

            if (ui->tabWidget->currentIndex() == 0) {
                onHistoryQuery();
            } else {
                onRealtimeQuery();
            }
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
            command->TerminalAddr = item->data(12).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(13).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(14).toInt();
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
            command->TerminalAddr = item->data(12).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(13).toInt();
            DatabaseProxy::instance().addCommand(command);
            command->TerminalAddr = item->data(14).toInt();
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

void SummaryWidget::on_btnExport_clicked()
{
    if (0 == ui->tawHistoryDetail->rowCount()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("没有数据，无法导出!"));
        return;
    }

    // 获取保存文件路径
    auto fileDlg = new QFileDialog(this);
    fileDlg->setWindowTitle(QStringLiteral("保存文件"));
    fileDlg->setAcceptMode(QFileDialog::AcceptSave);
    fileDlg->selectFile(QStringLiteral("历史数据.xls"));
    fileDlg->setNameFilter("Excel Files(*.xls *.xlsx)");
    fileDlg->setDefaultSuffix("xls");

    if (fileDlg->exec() == QDialog::Accepted)
    {
        auto fileName = fileDlg->selectedFiles().first();

        // 保存文件添加后缀名
        auto fInfo = QFileInfo(fileName);
        if (fInfo.suffix() != "xls" && fInfo.suffix() != "xlsx")
        {
            fileName += ".xls";
        }

        QXlsx::Document xlsx;

        QXlsx::Format forTitle;/*设置该单元的样式*/
        forTitle.setFontColor(QColor(Qt::white));/*文字为红色*/
        forTitle.setPatternBackgroundColor(QColor(25,25,112));/*背景颜色*/
        forTitle.setFontSize(20);/*设置字体大小*/
        forTitle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);/*横向居中*/
        forTitle.setBorderStyle(QXlsx::Format::BorderThin);/*边框样式*/

        QXlsx::Format forContent = forTitle;
        forContent.setFontColor(QColor(Qt::black));/*文字为红色*/
        forContent.setFillPattern(QXlsx::Format::PatternNone);/*背景颜色*/
        forTitle.setFontSize(18);/*设置字体大小*/

        // 设置excel任务标题
        QStringList lstTitle;
        lstTitle << QStringLiteral("公司") << QStringLiteral("供电分公司") << QStringLiteral("供电所") << QStringLiteral("线路") << QStringLiteral("集中器") << QStringLiteral("线段") << QStringLiteral("监测点") << QStringLiteral("A相电流") << QStringLiteral("B相电流") << QStringLiteral("C相电流") << QStringLiteral("采集时间");

        for (int i = 0; i < lstTitle.size(); i++)
        {
            xlsx.write(1, i+1, lstTitle.at(i), forTitle);
            xlsx.setColumnWidth(i+1, (i != lstTitle.size()-1) ? 20 : 40);
        }

        // 获取表格内容设置excel
        for (int i = 0; i < ui->tawHistoryDetail->rowCount(); i++)
        {
            for (int j = 0; j < lstTitle.count(); j++)
            {
                xlsx.write(i+2, j+1 , ui->tawHistoryDetail->item(i, j)->text(), forContent);
            }
        }

        // 保存文件
        if (xlsx.saveAs(fileName)) {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导出成功!"));
        }
    }
}

void SummaryWidget::onSubCompayChanged()
{
    ui->cbConcentrator->clear();
    if (0 == ui->cbSubCompany->currentIndex()) {
        QStringList conNames;
        foreach (auto child, _map.values()) {
            conNames.append(child);
        }
        conNames.prepend(QStringLiteral("全部"));
    } else {
        QStringList conNames = _map.value(ui->cbSubCompany->currentText());
        conNames.prepend(QStringLiteral("全部"));
        ui->cbConcentrator->addItems(conNames);
    }

    onHistoryQuery();
}
