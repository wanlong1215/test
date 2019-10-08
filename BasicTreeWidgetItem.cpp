#include "BasicTreeWidgetItem.h"
#include <QDebug>

BasicTreeWidgetItem::BasicTreeWidgetItem(int level, QTreeWidget *parent)
    : QTreeWidgetItem(parent)
{
    _level = level;
}

BasicTreeWidgetItem::BasicTreeWidgetItem(int level, QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{
    _level = level;
}

int BasicTreeWidgetItem::id()
{
    switch (_level)
    {
    case 1:
        return _o1->id;
    case 2:
        return _o2->id;
    case 3:
        return _o3->id;
    case 4:
        return _o4->id;
    case 5:
        return _o5->id;
    case 6:
        return _o6->id;
    case 7:
        return _o7->id;
    default:
        break;
	}

	return 0;
}

QString BasicTreeWidgetItem::name()
{
    switch (_level)
    {
    case 1:
        return _o1->name;
    case 2:
        return _o2->name;
    case 3:
        return _o3->name;
    case 4:
        return _o4->name;
    case 5:
        return _o5->name;
    case 6:
        return _o6->name;
    case 7:
        return _o7->name;
    default:
        break;
	}

	return "";
}
