#include "DatabaseProxy.h"
#include <QDateTime>

DatabaseProxy::DatabaseProxy()
{
}

DatabaseProxy &DatabaseProxy::instance()
{
    static DatabaseProxy instance;

    return instance;
}

QList<proCompany *> DatabaseProxy::getOrganizations()
{
    // read from db;
    return _lst;
}

bool DatabaseProxy::addCompany(proCompany *o)
{
    // insert into db

    // createId, set parent id
    o->id = createId();
    _lst.append(o);

    return true;
}

bool DatabaseProxy::addSubCompany(proSubCompany *o, int parentid)
{
    proCompany *po = company(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addAmso(proAmso *o, int parentid)
{
    proSubCompany *po = subCompany(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addRoute(proRoute *o, int parentid)
{
    proAmso *po = amso(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addConcentrator(proConcentrator *o, int parentid)
{
    proRoute *po = route(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addLine(proLine *o, int parentid)
{
    proConcentrator *po = concentrator(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addMonitor(proMonitor *o, int parentid)
{
    proLine *po = line(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addTerminal(proTerminal *o, int parentid)
{
    proMonitor *po = monitor(parentid);

    if (NULL != po)
    {
        o->id = createId();
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::delCompany(int id)
{
    foreach(proCompany * o1, _lst) {
        if (o1->id == id)
        {
            if (!o1->lst.isEmpty())
            {
                return false;
            }
            _ids.removeOne(id);
            _lst.removeOne(o1);
            return true;
        }
    }

    return false;
}

bool DatabaseProxy::delSubCompany(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            if (o2->id == id)
            {
                if (!o2->lst.isEmpty())
                {
                    return false;
                }
                _ids.removeOne(id);
                o1->lst.removeOne(o2);
                return true;
            }
        }
    }

    return false;
}

bool DatabaseProxy::delAmso(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                if (o3->id == id)
                {
                    if (!o3->lst.isEmpty())
                    {
                        return false;
                    }
                    _ids.removeOne(id);
                    o2->lst.removeOne(o3);
                    return true;
                }
            }
        }
    }

    return false;
}

bool DatabaseProxy::delRoute(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    if (o4->id == id)
                    {
                        if (!o4->lst.isEmpty())
                        {
                            return false;
                        }
                        _ids.removeOne(id);
                        o3->lst.removeOne(o4);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool DatabaseProxy::delConcentrator(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        if (o5->id == id)
                        {
                            if (!o5->lst.isEmpty())
                            {
                                return false;
                            }
                            _ids.removeOne(id);
                            o4->lst.removeOne(o5);
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool DatabaseProxy::delLine(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        foreach(proLine * o6, o5->lst) {
                            if (o6->id == id)
                            {
                                if (!o6->lst.isEmpty())
                                {
                                    return false;
                                }
                                _ids.removeOne(id);
                                o5->lst.removeOne(o6);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool DatabaseProxy::delTerminal(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        foreach(proLine * o6, o5->lst) {
                            foreach(proMonitor * o7, o6->lst) {
                                foreach(proTerminal * o8, o7->lst) {
                                    if (o8->id == id)
                                    {
                                        _ids.removeOne(id);
                                        o7->lst.removeOne(o8);

                                        // if monitor child num = 0, delete monitor
                                        if (o7->lst.isEmpty())
                                        {
                                            o6->lst.removeOne(o7);
                                        }
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

proCompany *DatabaseProxy::company(int id)
{
    foreach(proCompany * o1, _lst) {
        if (o1->id == id)
        {
            return o1;
        }
    }

    return NULL;
}

proSubCompany *DatabaseProxy::subCompany(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            if (o2->id == id)
            {
                return o2;
            }
        }
    }

    return NULL;
}

proAmso *DatabaseProxy::amso(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                if (o3->id == id)
                {
                    return o3;
                }
            }
        }
    }

    return NULL;
}

proRoute *DatabaseProxy::route(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    if (o4->id == id)
                    {
                        return o4;
                    }
                }
            }
        }
    }

    return NULL;
}

proConcentrator *DatabaseProxy::concentrator(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        if (o5->id == id)
                        {
                            return o5;
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

proLine *DatabaseProxy::line(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        foreach(proLine * o6, o5->lst) {
                            if (o6->id == id)
                            {
                                return o6;
                            }
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

proMonitor *DatabaseProxy::monitor(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        foreach(proLine * o6, o5->lst) {
                            foreach(proMonitor * o7, o6->lst) {
                                if (o7->id == id)
                                {
                                    return o7;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

proTerminal *DatabaseProxy::terminal(int id)
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        foreach(proLine * o6, o5->lst) {
                            foreach(proMonitor * o7, o6->lst) {
                                foreach(proTerminal * o8, o7->lst) {
                                    if (o8->id == id)
                                    {
                                        return o8;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

proConcentrator *DatabaseProxy::firstConcentrator()
{
    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        return o5;
                    }
                }
            }
        }
    }

    return NULL;
}
#include <QDebug>
QList<proData> DatabaseProxy::historyData(int ConcentratorId, QDateTime begin, QDateTime end)
{
    QList<proData> lst;

    for (int i = 0; i < 20; i++)
    {
        qsrand(QTime::currentTime().msecsSinceStartOfDay());

        proData data;
        data.CollectTime = QDateTime::currentDateTime().toSecsSinceEpoch(); // 采集时间
        data.iValueA = 5 + i + 0.01 * (qrand() % 99);//采集电流值
        data.iValueB = 5 + i * 2 + 0.01 * (qrand() % 99);//采集电流值
        data.iValueC = 55 + i + 0.01 * (qrand() % 99);//采集电流值
        lst.append(data);
    }

    return lst;
}

int DatabaseProxy::createId()
{
    for (int i = 0; i < _ids.count() + 1; i++)
    {
        if (_ids.contains(i))
        {
            continue;
        }

        _ids.append(i);
        return i;
    }

    return 0;
}

QList<proLine *> proConcentrator::getSortLine()
{
    // TODO: get sort line
    return lst;
}

QString proMonitor::pressureValueA()
{
    if (!lst.isEmpty())
    {
        return QString::number(lst.first()->highPressureValue);
    }

    return QString("-");
}

QString proMonitor::pressureValueB()
{
    if (lst.count() > 1)
    {
        return QString::number(lst.at(1)->highPressureValue);
    }

    return QString("-");
}

QString proMonitor::pressureValueC()
{
    if (lst.count() > 2)
    {
        return QString::number(lst.at(2)->highPressureValue);
    }

    return QString("-");
}
