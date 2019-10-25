#include "OrganizationTreeWidgetItem.h"

OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proCompany *o, QTreeWidget *parent) : BasicTreeWidgetItem(1, parent)
{
	_o1 = o;
    setText(0, o->name);
}

OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proSubCompany *o, QTreeWidgetItem *parent) : BasicTreeWidgetItem(2, parent)
{
	_o2 = o;
    setText(0, o->name);
}
OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proAmso *o, QTreeWidgetItem *parent) : BasicTreeWidgetItem(3, parent)
{
	_o3 = o;
    setText(0, o->name);
}
OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proRoute *o, QTreeWidgetItem *parent) : BasicTreeWidgetItem(4, parent)
{
	_o4 = o;
    setText(0, o->name);
}
OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proConcentrator *o, QTreeWidgetItem *parent) : BasicTreeWidgetItem(5, parent)
{
    _o5 = o;
    setText(0, o->name);
}
OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proLine *o, QTreeWidgetItem *parent) : BasicTreeWidgetItem(6, parent)
{
    _o6 = o;
    setText(0, o->name);
}

OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proMonitor *o, QTreeWidgetItem *parent) : BasicTreeWidgetItem(7, parent)
{
    _o7 = o;
    setText(0, o->name);
}

OrganizationTreeWidgetItem::OrganizationTreeWidgetItem(proTerminal *o, QTreeWidget *parent) : BasicTreeWidgetItem(8, parent)
{
    _o8 = o;
    updateTerminalText();
//    setText(0, _o8->parent->name);
//    setText(1, _o8->name);
//    setText(2, QString::number(_o8->addr));
//    setText(3, QString::number(_o8->RouteState1));
//    setText(4, QString::number(_o8->RouteState2));
//    setText(5, QString::number(_o8->RouteState3));
//    setText(6, QString::number(_o8->RouteState4));
//    setText(7, QString::number(_o8->RouteState5));
//    setText(8, QString::number(_o8->RouteState6));
//    setText(9, QString::number(_o8->highPressureValue));
//    setText(10, QString::number(_o8->highPressureOffset));
}

void OrganizationTreeWidgetItem::updateTerminalText()
{
    if (8 == _level) {
        setText(0, _o8->parent->name);
        setText(1, _o8->name);
        setText(2, QString::number(_o8->addr));
        setText(3, QString::number(_o8->RouteState1));
        setText(4, QString::number(_o8->RouteState2));
        setText(5, QString::number(_o8->RouteState3));
        setText(6, QString::number(_o8->RouteState4));
        setText(7, QString::number(_o8->RouteState5));
        setText(8, QString::number(_o8->RouteState6));
        setText(9, QString::number(_o8->highPressureValue));
        setText(10, QString::number(_o8->highPressureOffset));
    }
}

bool OrganizationTreeWidgetItem::canAddChild()
{
    return _level < 7;
}

QString OrganizationTreeWidgetItem::delMenuText()
{
    QStringList lst;

    //lst << QString::fromLocal8Bit("删除公司") << QString::fromLocal8Bit("删除分公司") << QString::fromLocal8Bit("删除供电所") << QString::fromLocal8Bit("删除线路") << QString::fromLocal8Bit("删除集中器") << QString::fromLocal8Bit("删除线段");
    lst << QStringLiteral("删除公司") << QStringLiteral("删除分公司") << QStringLiteral("删除供电所") << QStringLiteral("删除线路") << QStringLiteral("删除集中器") << QStringLiteral("删除线段") << QStringLiteral("删除监测点") << QStringLiteral("删除终端");

    int l = _level - 1;
    if (l >= 0 && l < lst.count())
    {
        return lst.at(l);
    }
    else
    {
        return "";
    }
}

QString OrganizationTreeWidgetItem::addMenuText()
{
	QStringList lst;

    //lst << QString::fromLocal8Bit("添加公司") << QString::fromLocal8Bit("添加分公司") << QString::fromLocal8Bit("添加供电所") << QString::fromLocal8Bit("添加线路") << QString::fromLocal8Bit("添加集中器") << QString::fromLocal8Bit("添加线段");
    lst << QStringLiteral("添加公司") << QStringLiteral("添加分公司") << QStringLiteral("添加供电所") << QStringLiteral("添加线路") << QStringLiteral("添加集中器") << QStringLiteral("添加线段") << QStringLiteral("添加终端");

    if (_level > 0 && _level < lst.count())
    {
        return lst.at(_level);
    }
    else
    {
        return "";
    }
}

QString OrganizationTreeWidgetItem::modifyMenuText()
{
    QStringList lst;

    //lst << QString::fromLocal8Bit("修改公司") << QString::fromLocal8Bit("修改分公司") << QString::fromLocal8Bit("修改供电所") << QString::fromLocal8Bit("修改线路") << QString::fromLocal8Bit("修改集中器") << QString::fromLocal8Bit("修改线段");
    lst << QStringLiteral("修改公司") << QStringLiteral("修改分公司") << QStringLiteral("修改供电所") << QStringLiteral("修改线路") << QStringLiteral("修改集中器") << QStringLiteral("修改线段") << QStringLiteral("修改监测点") << QStringLiteral("修改终端");

    int l = _level - 1;
    if (l >= 0 && l < lst.count())
    {
        return lst.at(l);
    }
    else
    {
        return "";
    }
}
