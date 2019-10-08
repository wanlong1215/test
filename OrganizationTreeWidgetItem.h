#ifndef ORGANIZATIONTREEWIDGETITEM_H
#define ORGANIZATIONTREEWIDGETITEM_H

#include "BasicTreeWidgetItem.h"

class OrganizationTreeWidgetItem : public BasicTreeWidgetItem
{
public:
    OrganizationTreeWidgetItem(proCompany *o, QTreeWidget *parent);
    OrganizationTreeWidgetItem(proSubCompany *o, QTreeWidgetItem *parent);
    OrganizationTreeWidgetItem(proAmso *o, QTreeWidgetItem *parent);
    OrganizationTreeWidgetItem(proRoute *o, QTreeWidgetItem *parent);
    OrganizationTreeWidgetItem(proConcentrator *o, QTreeWidgetItem *parent);
    OrganizationTreeWidgetItem(proLine *o, QTreeWidgetItem *parent);
    OrganizationTreeWidgetItem(proTerminal *o, QTreeWidget *parent);

    bool canAddChild() override;
    QString addMenuText() override;
    QString modifyMenuText() override;
    QString delMenuText() override;
};

#endif // ORGANIZATIONTREEWIDGETITEM_H
