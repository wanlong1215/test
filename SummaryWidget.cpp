#include "SummaryWidget.h"
#include "ui_SummaryWidget.h"
#include "DatabaseProxy.h"
#include "OrganizationTreeWidgetItem.h"

SummaryWidget::SummaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryWidget)
{
    ui->setupUi(this);

    _currentConcentrator = NULL;
    ui->trwOrganization->setHeaderHidden(true);
    ui->trwOrganization->viewport()->installEventFilter(this);

    init();
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

    connect(ui->rbAutoQuery, SIGNAL(toggled(bool)), this, SLOT(onAutoQueryToggled(bool)));
    connect(ui->rbQuickQuery, SIGNAL(toggled(bool)), this, SLOT(onQuickQueryToggled(bool)));
    connect(ui->rbAbsoluteQuery, SIGNAL(toggled(bool)), this, SLOT(onAbsoluteQueryToggled(bool)));
    connect(ui->btnQuery, SIGNAL(clicked(bool)), this, SLOT(onHistoryQuery()));

    ui->dteBegin->setDateTime(QDateTime::currentDateTime().addSecs(-60 * 60));
    ui->dteEnd->setDateTime(QDateTime::currentDateTime());

    ui->rbAutoQuery->setChecked(true);
    onAutoQueryToggled(true);

    // TODO: temp init
    //onRealtimePage();
}
void SummaryWidget::onAutoQueryToggled(bool b)
{
    ui->cbQuickCycle->setEnabled(!b);
    ui->dteBegin->setEnabled(!b);
    ui->dteEnd->setEnabled(!b);

    // clear data, show first concentrator
    proConcentrator *concentrator = DatabaseProxy::instance().firstConcentrator();
}
void SummaryWidget::onQuickQueryToggled(bool b)
{
    ui->cbQuickCycle->setEnabled(b);
    ui->dteBegin->setEnabled(!b);
    ui->dteEnd->setEnabled(!b);

    // clear data
}
void SummaryWidget::onAbsoluteQueryToggled(bool b)
{
    ui->cbQuickCycle->setEnabled(!b);
    ui->dteBegin->setEnabled(b);
    ui->dteEnd->setEnabled(b);

    // clear data
}

void SummaryWidget::onHistoryQuery()
{
    QStringList lstHeader;

    ui->tawHistoryDetail->setColumnCount(8);
    lstHeader << QString("供电分公司") << QString("供电所") << QString("线路") << QString("集中器") << QString("A电流值") << QString("B电流值") << QString("C电流值") << QString("采集时间");
    ui->tawHistoryDetail->setHorizontalHeaderLabels(lstHeader);
    if (NULL == _currentConcentrator)
    {
        return;
    }

    QList<proData> lst = DatabaseProxy::instance().historyData(1, QDateTime(), QDateTime());
    ui->tawHistoryDetail->setRowCount(lst.count());
    ui->tawHistoryDetail->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < lst.count(); i++)
    {
        proData data = lst.at(i);
        ui->tawHistoryDetail->setItem(i, 0, new QTableWidgetItem(_currentConcentrator->parent->parent->parent->name));
        ui->tawHistoryDetail->setItem(i, 1, new QTableWidgetItem(_currentConcentrator->parent->parent->name));
        ui->tawHistoryDetail->setItem(i, 2, new QTableWidgetItem(_currentConcentrator->parent->name));
        ui->tawHistoryDetail->setItem(i, 3, new QTableWidgetItem(_currentConcentrator->name));
        ui->tawHistoryDetail->setItem(i, 4, new QTableWidgetItem(QString::number(data.iValueA)));
        ui->tawHistoryDetail->setItem(i, 5, new QTableWidgetItem(QString::number(data.iValueB)));
        ui->tawHistoryDetail->setItem(i, 6, new QTableWidgetItem(QString::number(data.iValueC)));
        ui->tawHistoryDetail->setItem(i, 7, new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(data.CollectTime).toString("yyyy-MM-dd hh:mm")));
    }
}

void SummaryWidget::onRealtimePage()
{
    ui->wgtGraphics->init(_currentConcentrator);
    ui->scrollAreaWidgetContents->setMinimumWidth(ui->wgtGraphics->width());

    QStringList lstHeader;

    ui->tawRealTimeDetail->setColumnCount(8);
    lstHeader << QString("供电分公司") << QString("供电所") << QString("线路") << QString("集中器") << QString("A电流值") << QString("B电流值") << QString("C电流值") << QString("采集时间");
    ui->tawRealTimeDetail->setHorizontalHeaderLabels(lstHeader);
    if (NULL == _currentConcentrator)
    {
        return;
    }

    QList<proData> lst = DatabaseProxy::instance().historyData(1, QDateTime(), QDateTime());
    ui->tawRealTimeDetail->setRowCount(lst.count());
    ui->tawRealTimeDetail->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < lst.count(); i++)
    {
        proData data = lst.at(i);
        ui->tawRealTimeDetail->setItem(i, 0, new QTableWidgetItem(_currentConcentrator->parent->parent->parent->name));
        ui->tawRealTimeDetail->setItem(i, 1, new QTableWidgetItem(_currentConcentrator->parent->parent->name));
        ui->tawRealTimeDetail->setItem(i, 2, new QTableWidgetItem(_currentConcentrator->parent->name));
        ui->tawRealTimeDetail->setItem(i, 3, new QTableWidgetItem(_currentConcentrator->name));
        ui->tawRealTimeDetail->setItem(i, 4, new QTableWidgetItem(QString::number(data.iValueA)));
        ui->tawRealTimeDetail->setItem(i, 5, new QTableWidgetItem(QString::number(data.iValueB)));
        ui->tawRealTimeDetail->setItem(i, 6, new QTableWidgetItem(QString::number(data.iValueC)));
        ui->tawRealTimeDetail->setItem(i, 7, new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(data.CollectTime).toString("yyyy-MM-dd hh:mm")));
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

            onRealtimePage();
        }
    }

    return QWidget::eventFilter(obj, e);
}

void SummaryWidget::resizeEvent(QResizeEvent*/*e*/)
{
//    int w = ui->tabWidget->width();
//    QTabBar *bar = ui->tabWidget->tabBar();

//    bar->setMinimumSize(w, 50);
//    bar->setMaximumSize(w, 50);
}
