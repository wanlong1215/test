#include "CenterWidget.h"
#include "ui_CenterWidget.h"
#include <QMenu>
#include <QDebug>
#include <QContextMenuEvent>
#include "OrganizationTreeWidgetItem.h"
#include "DatabaseProxy.h"
#include "BasicEditDlg.h"
#include "ConcentratorEditDlg.h"
#include "LineEditDlg.h"
#include "AddTerminalDlg.h"
#include "ModifyTerminalDlg.h"

CenterWidget::CenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CenterWidget)
{
	ui->setupUi(this);

	ui->trwOrganization->setHeaderHidden(true);
	ui->trwDetail->setHeaderHidden(true);
    ui->trwDetail->setColumnCount(8);

	ui->trwOrganization->viewport()->installEventFilter(this);
	ui->trwDetail->viewport()->installEventFilter(this);
}

CenterWidget::~CenterWidget()
{
	delete ui;
}

void CenterWidget::init()
{
    // clear first
    ui->trwDetail->clear();
    ui->trwOrganization->clear();

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

                        // add line, level=6
                        foreach(proLine * o6, o5->lst) {
                            OrganizationTreeWidgetItem *i6 = new OrganizationTreeWidgetItem(o6, i5);

                            i5->addChild(i6);
                        }

                        i5->setExpanded(true);
                    }

                    i4->setExpanded(true);
                }

                i3->setExpanded(true);
            }

            i2->setExpanded(true);
        }

        i1->setExpanded(true);
    }
}

bool CenterWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->trwOrganization->viewport())
    {
        if (e->type() == QEvent::ContextMenu)
        {
            QContextMenuEvent *menuEv = dynamic_cast<QContextMenuEvent *>(e);
			QTreeWidgetItem *clickItem = ui->trwOrganization->itemAt(menuEv->pos());
			QMenu menu;
			QAction *addAc = NULL;
            QAction *modifyAc = NULL;
			QAction *delAc = NULL;
			QAction *selAc = NULL;
			OrganizationTreeWidgetItem *itemO = NULL;
            if (NULL == clickItem)
            {
                //addAc = menu.addAction(QString::fromLocal8Bit("添加公司"));
                addAc = menu.addAction(QString("添加公司"));
            }
            else
            {
				itemO = dynamic_cast<OrganizationTreeWidgetItem *>(clickItem);
				if (NULL != itemO)
				{
                    if (itemO->canAddChild())
                    {
                        addAc = menu.addAction(itemO->addMenuText());
					}

                    modifyAc = menu.addAction(itemO->modifyMenuText());
                    delAc = menu.addAction(itemO->delMenuText());
				}
			}

            selAc = menu.exec(QCursor::pos());
            if (NULL != selAc)
            {
                if (addAc == selAc)
                {
                    addItem(clickItem);
                }
                else if (modifyAc == selAc)
                {
                    modifyItem(clickItem);
                }
                else if (delAc == selAc)
                {
                    delItem(clickItem);
                }
            }
		}
    }
    else if (obj == ui->trwDetail->viewport())
    {
        if (e->type() == QEvent::ContextMenu)
        {
            QContextMenuEvent *menuEv = dynamic_cast<QContextMenuEvent *>(e);
            QTreeWidgetItem *clickItem = ui->trwDetail->itemAt(menuEv->pos());
            OrganizationTreeWidgetItem *itemO = dynamic_cast<OrganizationTreeWidgetItem *>(clickItem);
            QMenu menu;
            QAction *delAc = NULL;
            QAction *modifyAc = NULL;
            QAction *selAc = NULL;
            if (NULL != itemO)
            {
                modifyAc = menu.addAction(itemO->modifyMenuText());
                delAc = menu.addAction(itemO->delMenuText());
                QAction *read = menu.addAction("读取集中器时间");
                QAction *readPara = menu.addAction("读取集中器参数");
                QAction *write = menu.addAction("修改集中器时间");
                selAc = menu.exec(QCursor::pos());
                if (NULL != selAc)
                {
                    if (delAc == selAc)
                    {
                        delItem(clickItem);
                    }
                    else if (modifyAc == selAc)
                    {
                        modifyItem(clickItem);
                    }
                }
			}
		}
	}

	return QWidget::eventFilter(obj, e);
}

void CenterWidget::addItem(QTreeWidgetItem *clickItem)
{
    OrganizationTreeWidgetItem *clickItemO = dynamic_cast<OrganizationTreeWidgetItem *>(clickItem);

    int dlgType = (NULL == clickItemO) ? 1 : clickItemO->_level + 1;
    int pId = (NULL == clickItemO) ? -1 : clickItemO->id();

    OrganizationTreeWidgetItem *item = NULL;

    if (dlgType <= 4)
    {
        BasicEditDlg *dlg = new BasicEditDlg(dlgType, pId, this);

        if (dlg->exec() == QDialog::Accepted)
        {
            if (NULL == clickItem)
            {
                proCompany *proCom = dlg->company();
                if (DatabaseProxy::instance().addCompany(proCom))
                {
                    item = new OrganizationTreeWidgetItem(proCom, ui->trwOrganization);
                    ui->trwOrganization->addTopLevelItem(item);
                }
            }
            else
            {
                switch (clickItemO->_level)
                {
                case 1: {
                    proSubCompany *proSub = dlg->subCompany();
                    if (DatabaseProxy::instance().addSubCompany(proSub, clickItemO->id()))
                    {
                        item = new OrganizationTreeWidgetItem(proSub, clickItemO);
                    }
                } break;
                case 2: {
                    proAmso *proAmso = dlg->amso();
                    if (DatabaseProxy::instance().addAmso(proAmso, clickItemO->id()))
                    {
                        item = new OrganizationTreeWidgetItem(proAmso, clickItemO);
                    }
                } break;
                case 3: {
                    proRoute *proRoute = dlg->route();
                    if (DatabaseProxy::instance().addRoute(proRoute, clickItemO->id()))
                    {
                        item = new OrganizationTreeWidgetItem(proRoute, clickItemO);
                    }
                } break;
                default:
                    break;
                }

                if (NULL != item)
                {
                    clickItemO->addChild(item);
                    clickItemO->setExpanded(true);
                }
            }
        }
    }
    else if (5 == dlgType)
    {
        ConcentratorEditDlg *dlg = new ConcentratorEditDlg(NULL, pId, this);
        if (dlg->exec() == QDialog::Accepted)
        {
            proConcentrator *proCon = dlg->concentrator();
            if (DatabaseProxy::instance().addConcentrator(proCon, pId))
            {
                item = new OrganizationTreeWidgetItem(proCon, clickItemO);
                clickItemO->addChild(item);
                clickItemO->setExpanded(true);
            }
        }
    }
    else if (6 == dlgType)
    {
        LineEditDlg *dlg = new LineEditDlg(NULL, pId, this);
        if (dlg->exec() == QDialog::Accepted)
        {
            proLine *proL = dlg->line();
            if (DatabaseProxy::instance().addLine(proL, pId))
            {
                item = new OrganizationTreeWidgetItem(proL, clickItemO);
                clickItemO->addChild(item);
                clickItemO->setExpanded(true);
            }
        }
    }
    else if (7 == dlgType)
    {
        AddTerminalDlg *dlg = new AddTerminalDlg(pId, this);

        if (dlg->exec() == QDialog::Accepted)
        {
            proTerminal *proT1 = dlg->terminal1();
            if (DatabaseProxy::instance().addTerminal(proT1, proT1->parent->id))
            {
                OrganizationTreeWidgetItem *itemT = new OrganizationTreeWidgetItem(proT1, ui->trwDetail);
                ui->trwDetail->addTopLevelItem(itemT);
            }
            proTerminal *proT2 = dlg->terminal2();
            if (DatabaseProxy::instance().addTerminal(proT2, proT2->parent->id))
            {
                OrganizationTreeWidgetItem *itemT = new OrganizationTreeWidgetItem(proT2, ui->trwDetail);
                ui->trwDetail->addTopLevelItem(itemT);
            }
            proTerminal *proT3 = dlg->terminal3();
            if (DatabaseProxy::instance().addTerminal(proT3, proT3->parent->id))
            {
                OrganizationTreeWidgetItem *itemT = new OrganizationTreeWidgetItem(proT3, ui->trwDetail);
                ui->trwDetail->addTopLevelItem(itemT);
            }
        }
    }

    // left treeWidget, not contains terminal item
    if (NULL != item)
    {
        ui->trwOrganization->clearSelection();
        item->setSelected(true);
    }
}

void CenterWidget::modifyItem(QTreeWidgetItem * clickItem)
{
    OrganizationTreeWidgetItem *clickItemO = dynamic_cast<OrganizationTreeWidgetItem *>(clickItem);

    if (NULL == clickItemO)
    {
        return;
    }

    if (clickItemO->_level <= 4)
    {
        BasicEditDlg *dlg = NULL;
        switch (clickItemO->_level)
        {
        case 1:
            dlg = new BasicEditDlg(clickItemO->_o1, this);
            break;
        case 2:
            dlg = new BasicEditDlg(clickItemO->_o2, this);
            break;
        case 3:
            dlg = new BasicEditDlg(clickItemO->_o3, this);
            break;
        case 4:
            dlg = new BasicEditDlg(clickItemO->_o4, this);
            break;
        default:
            break;
        }
        if (NULL != dlg)
        {
            if (dlg->exec() == QDialog::Accepted)
            {
                clickItemO->setText(0, dlg->name());
            }
        }
    }
    else if (5 == clickItemO->_level)
    {
        ConcentratorEditDlg *dlg = new ConcentratorEditDlg(clickItemO->_o5, -1, this);
        if (dlg->exec() == QDialog::Accepted)
        {
            clickItemO->setText(0, dlg->concentrator()->name);
        }
    }
    else if (6 == clickItemO->_level)
    {
        LineEditDlg *dlg = new LineEditDlg(clickItemO->_o6, -1, this);
        if (dlg->exec() == QDialog::Accepted)
        {
            clickItemO->setText(0, dlg->line()->name);
        }
    }
    else if (7 == clickItemO->_level)
    {
        ModifyTerminalDlg *dlg = new ModifyTerminalDlg(clickItemO->_o7, this);
        if (dlg->exec() == QDialog::Accepted)
        {
            clickItemO->setText(6, dlg->terminal()->name);
        }
    }
}

void CenterWidget::delItem(QTreeWidgetItem * item)
{
    OrganizationTreeWidgetItem *itemO = dynamic_cast<OrganizationTreeWidgetItem *>(item);

    if (NULL == itemO)
    {
        return;
    }

    if (NULL == itemO->parent())
    {
        if (1 == itemO->_level)
        {
            if (DatabaseProxy::instance().delCompany(itemO->id()))
            {
                ui->trwOrganization->takeTopLevelItem(ui->trwOrganization->indexOfTopLevelItem(itemO));
            }
        }
        if (7 == itemO->_level)
        {
            if (DatabaseProxy::instance().delTerminal(itemO->id()))
            {
                ui->trwDetail->takeTopLevelItem(ui->trwDetail->indexOfTopLevelItem(itemO));
            }
        }
    }
    else
    {
        switch (itemO->_level)
        {
        case 2: {
            if (DatabaseProxy::instance().delSubCompany(itemO->id()))
            {
                itemO->parent()->removeChild(itemO);
            }
        }
        break;
        case 3: {
            if (DatabaseProxy::instance().delAmso(itemO->id()))
            {
                itemO->parent()->removeChild(itemO);
            }
        }
        break;
        case 4: {
            if (DatabaseProxy::instance().delRoute(itemO->id()))
            {
                itemO->parent()->removeChild(itemO);
            }
        }
        break;
        case 5: {
            if (DatabaseProxy::instance().delConcentrator(itemO->id()))
            {
                itemO->parent()->removeChild(itemO);
            }
        }
        break;
        case 6: {
            if (DatabaseProxy::instance().delLine(itemO->id()))
            {
                itemO->parent()->removeChild(itemO);
            }
        }
        break;
        default:
            break;
        }

        ui->trwOrganization->clearSelection();
    }
}
