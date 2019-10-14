#ifndef BASICTREEWIDGETITEM_H
#define BASICTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "DatabaseProxy.h"

class BasicTreeWidgetItem : public QTreeWidgetItem
{
public:
    BasicTreeWidgetItem(int level, QTreeWidget *parent);
	BasicTreeWidgetItem(int level, QTreeWidgetItem *parent);

	int id();
    QString name();

    virtual bool canAddChild() = 0;
    virtual QString addMenuText() = 0;
    virtual QString modifyMenuText() = 0;
    virtual QString delMenuText() = 0;

public:
    int _level;

    proTerminal *_o8;
    proMonitor *_o7;
    proLine *_o6;
    proConcentrator *_o5;
    proRoute *_o4;
    proAmso *_o3;
    proSubCompany *_o2;
    proCompany *_o1;
};

#endif // BASICTREEWIDGETITEM_H
