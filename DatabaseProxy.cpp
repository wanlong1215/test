#include "DatabaseProxy.h"
#include <QDateTime>

DatabaseProxy::DatabaseProxy()
{
	m_connectState = false;
}

DatabaseProxy &DatabaseProxy::instance()
{
    static DatabaseProxy instance;

    return instance;
}

/***************************************************************************************************************/
// TODO: 胖子，这块是你的
bool DatabaseProxy::connectDB(const QString &ip, const QString &usr, const QString &pwd)
{
    string strIP = ip.toStdString();
	string strUsr = usr.toStdString();
	string strPwd = pwd.toStdString();

	m_connectState = m_db2.DB2Connect(strIP, strUsr, strPwd);
    return m_connectState;
}

bool DatabaseProxy::isDBConnected()
{
    return m_connectState;
}

bool DatabaseProxy::testDB(const QString &ip, const QString &usr, const QString &pwd)
{
    return true;
}

int DatabaseProxy::userId(const QString &usr, const QString &pwd)
{
    return m_db2.GetUserID(usr.toStdString(), pwd.toStdString());
}

int DatabaseProxy::userLevel(int id)
{
    return m_db2.GetUserLever(id);
}

int DatabaseProxy::addUser(const QString &usr, const QString &pwd, int level)
{
    return m_db2.InsertUser(usr.toStdString(), pwd.toStdString(), level);
}

bool DatabaseProxy::modifyUser(int id, const QString &usr, const QString &pwd, int level)
{
	m_db2.ModifyUser(id, usr.toStdString(), pwd.toStdString(), level);
    return true;
}

bool DatabaseProxy::delUser(int id)
{
	m_db2.DelUser(id);
    return true;
}
/***************************************************************************************************************/

void DatabaseProxy::clearOrganizations()
{
    while (!_lst.isEmpty())
        delete _lst.takeFirst();
}

QList<proCompany *> DatabaseProxy::getOrganizations()
{
    // read from db;
	clearOrganizations();

	vector<COMPANY> company; 
	m_db2.GetAllCompanyID(company);
	for (int i = 0; i < company.size(); i++)
	{
		proCompany *pCompany = new proCompany;
		pCompany->id = company[i].companyID;
		pCompany->name = QString::fromStdString(company[i].strName);
		pCompany->desc = QString::fromStdString(company[i].strDescribe);

		vector<SUBCOMPANY> subCompany;
		m_db2.GetAllSubCompanyByID(subCompany, company[i].companyID);
		for (int i = 0; i < subCompany.size(); i++)
		{
			proSubCompany *pSubCompany = new proSubCompany(pCompany);
			pSubCompany->id = subCompany[i].subCompanyID;
			pSubCompany->name = QString::fromStdString(subCompany[i].strName);
			pSubCompany->desc = QString::fromStdString(subCompany[i].strDescribe);
			
			vector<AMSO> amso;
			m_db2.GetAllAMSOByID(amso, subCompany[i].subCompanyID);
			for (int i = 0; i < amso.size(); i++)
			{
				proAmso *pAmso = new proAmso(pSubCompany);
				pAmso->id = amso[i].AMSOID;
				pAmso->name = QString::fromStdString(amso[i].strName);
				pAmso->desc = QString::fromStdString(amso[i].strDescribe);

				vector<ROUTE> route;
				m_db2.GetAllRouteByID(route, amso[i].AMSOID);
				for (int i = 0; i < route.size(); i++)
				{
					proRoute *pRoute = new proRoute(pAmso);
					pRoute->id = route[i].routeID;
					pRoute->name = QString::fromStdString(route[i].strName);
					pRoute->desc = QString::fromStdString(route[i].strDescribe);

					vector<CONCENTRATOR> concentrator;
					m_db2.GetAllConcentratorByID(concentrator, route[i].routeID);
					for (int i = 0; i < concentrator.size(); i++)
					{
						proConcentrator *pConcentrator = new proConcentrator(pRoute);
						pConcentrator->id = concentrator[i].ConcentratorID;
						pConcentrator->name = QString::fromStdString(concentrator[i].strName);
						pConcentrator->destIp = QString::fromStdString(concentrator[i].strDestIP);
						pConcentrator->destPort = QString::fromStdString(concentrator[i].strDestPort);
						pConcentrator->type = QString::fromStdString(concentrator[i].strConnectType);
						pConcentrator->installAddr = QString::fromStdString(concentrator[i].strInstallPlace);
						pConcentrator->apName = QString::fromStdString(concentrator[i].strAPName);
						pConcentrator->apProtocol = QString::fromStdString(concentrator[i].strAPProtocol);
						pConcentrator->terminalTimer = concentrator[i].TerminalTimer;
						pConcentrator->concentratorTimer = concentrator[i].ConcentratorTimer;
						pConcentrator->heartTimer = concentrator[i].HeartTimer;
						pConcentrator->strSimCard = QString::fromStdString(concentrator[i].strSimCard);
						pConcentrator->gprsReConnectTimer = concentrator[i].GPRSReConnectTimer;
						pConcentrator->gprsSignalStrength = concentrator[i].GPRSSignalStrength;
						pConcentrator->saveTimer = concentrator[i].SaveTimer;
						pConcentrator->wirelessSearchTimer = concentrator[i].wirelessSearchTimer;
						pConcentrator->concentratorAddr = concentrator[i].ConcentratorAddr;
						pConcentrator->ConcentratorCurrentTime = concentrator[i].ConcentratorCurrentTime;

						vector<LINE> line;
						m_db2.GetAllLineByID(line, concentrator[i].ConcentratorID);
						for (int i = 0; i < line.size(); i++)
						{
							proLine *pLine = new proLine(pConcentrator);
							pLine->id = line[i].lineID;
							pLine->name = QString::fromStdString(line[i].strName);
                            pLine->type = line[i].strType;
							pLine->addr = QString::fromStdString(line[i].strAddr);
							pLine->preAddr = QString::fromStdString(line[i].strPreAddr);
							pLine->nextAddr = QString::fromStdString(line[i].strNextAddr);
							pLine->workerID = line[i].workerID;

							vector<MONITOR> monitor;
							m_db2.GetAllMonitorByID(monitor, line[i].lineID);
							for (int i = 0; i < monitor.size(); i++)
							{
								proMonitor *pMonitor = new proMonitor(pLine);
								pMonitor->id = monitor[i].MonitorID;
								pMonitor->name = QString::fromStdString(monitor[i].strName);

								vector<TERMINAL> terminal;
								m_db2.GetAllTerminalByID(terminal, monitor[i].MonitorID);
								for (int i = 0; i < terminal.size(); i++)
								{
									proTerminal *pTerminal = new proTerminal(pMonitor);
									pTerminal->id = terminal[i].TerminalID;
									pTerminal->name = QString::fromStdString(terminal[i].strName);
									pTerminal->type = atoi(terminal[i].strType.c_str());
									pTerminal->index = terminal[i].index;
									pTerminal->installTime = terminal[i].installTime;
									pTerminal->addr = terminal[i].addr;
									pTerminal->preAddr = terminal[i].preAddr;
									pTerminal->nextAddr = terminal[i].nextAddr;
									pTerminal->ConcentratorAddr = terminal[i].ConcentratorAddr;
									pTerminal->TerminalCurrentTime = terminal[i].TerminalCurrentTime;
									pTerminal->RouteState1 = terminal[i].RouteState1;
									pTerminal->RouteState2 = terminal[i].RouteState2;
									pTerminal->RouteState3 = terminal[i].RouteState3;
									pTerminal->RouteState4 = terminal[i].RouteState4;
									pTerminal->RouteState5 = terminal[i].RouteState5;
									pTerminal->RouteState6 = terminal[i].RouteState6;
									pTerminal->highPressureValue = terminal[i].HighValue;
									pTerminal->highPressureOffset = terminal[i].HighOffset;
									pTerminal->highPressureSymbol = terminal[i].HighSymbol;
									pTerminal->lowPressureValue = terminal[i].LowValue;

									pMonitor->lst<<pTerminal;
								}
								pLine->lst<<pMonitor;
							}
							pConcentrator->lst<<pLine;
						}
						pRoute->lst<<pConcentrator;
					}
					pAmso->lst<<pRoute;
				}
				pSubCompany->lst<<pAmso;
			}
			pCompany->lst<<pSubCompany;
		}
		_lst<<pCompany;
	}
	

    return _lst;
}

bool DatabaseProxy::addCompany(proCompany *o)
{
    // insert into db
	COMPANY company;
	company.companyID = o->id;
	company.strName = o->name.toStdString();
	company.strDescribe = o->desc.toStdString();
    o->id = m_db2.InsertCompany(company);
    _lst.append(o);

    return true;
}

bool DatabaseProxy::addSubCompany(proSubCompany *o, int parentid)
{
	SUBCOMPANY subCompany;
	subCompany.companyID = parentid;
	subCompany.strName = o->name.toStdString();
	subCompany.strDescribe = o->desc.toStdString();
	o->id = m_db2.InsertSubCompany(subCompany, parentid);
    proCompany *po = company(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addAmso(proAmso *o, int parentid)
{
	AMSO amso;
	amso.subCompanyID = parentid;
	amso.strName = o->name.toStdString();
	amso.strDescribe = o->desc.toStdString();
	o->id = m_db2.InsertAMSO(amso, parentid);

    proSubCompany *po = subCompany(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addRoute(proRoute *o, int parentid)
{
	ROUTE route;
	route.AMSOID = parentid;
	route.strName = o->name.toStdString();
	route.strDescribe = o->desc.toStdString();
	o->id = m_db2.InsertRoute(route, parentid);
    proAmso *po = amso(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}


bool DatabaseProxy::addConcentrator(proConcentrator *o, int parentid)
{
	CONCENTRATOR concentrator;
	concentrator.routeID = parentid;
	concentrator.strName = o->name.toStdString();
	concentrator.strDestIP = o->destIp.toStdString();
	concentrator.strDestPort = o->destPort.toStdString();
	concentrator.strConnectType = o->type.toStdString();
	concentrator.strInstallPlace = o->installAddr.toStdString();
	concentrator.strAPName = o->apName.toStdString();
	concentrator.strAPProtocol = o->apProtocol.toStdString();
	concentrator.TerminalTimer = o->terminalTimer;
	concentrator.HeartTimer = o->heartTimer;
	concentrator.strSimCard = o->strSimCard.toStdString();
	concentrator.GPRSReConnectTimer = o->gprsReConnectTimer;
	concentrator.GPRSSignalStrength = o->gprsSignalStrength;
	concentrator.SaveTimer = o->saveTimer;
	concentrator.wirelessSearchTimer = o->wirelessSearchTimer;
	concentrator.ConcentratorAddr = o->concentratorAddr;
	concentrator.ConcentratorCurrentTime = o->ConcentratorCurrentTime;
	concentrator.SelfReportOnOff = o->SelfReportOnOff;
    concentrator.ConcentratorTimer = o->concentratorTimer;
	o->id = m_db2.InsertConcentrator(concentrator, parentid);


    proRoute *po = route(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addLine(proLine *o, int parentid)
{
	LINE line;
	line.ConcentratorID = parentid;
	line.strName = o->name.toStdString();
    line.strType = o->type;
	line.strAddr = o->addr.toStdString();
	line.strPreAddr = o->preAddr.toStdString();
	line.strNextAddr = o->nextAddr.toStdString();
	line.workerID = o->workerID;
	o->id = m_db2.InsertLine(line, parentid);

    proConcentrator *po = concentrator(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}

bool DatabaseProxy::addMonitor(proMonitor *o, int parentid)
{
	MONITOR monitor;
	monitor.lineID = parentid;
	monitor.strName = o->name.toStdString();
	o->id = m_db2.InsertMonitor(monitor,parentid);

    proLine *po = line(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}
bool DatabaseProxy::addTerminal(proTerminal *o, int parentid)
{
	TERMINAL terminal;
	terminal.MonitorID = parentid;
	terminal.strName = o->name.toStdString();
	char chType[10] = {0};
	itoa(o->type,chType,10);
	terminal.strType = string(chType);
	terminal.index = o->index;
	terminal.installTime = o->installTime;
	terminal.addr = o->addr;
	terminal.preAddr = o->preAddr;
	terminal.nextAddr = o->nextAddr;
	terminal.ConcentratorAddr = o->ConcentratorAddr;
	terminal.TerminalCurrentTime = o->TerminalCurrentTime;
	terminal.RouteState1 = o->RouteState1;
	terminal.RouteState2 = o->RouteState2;
	terminal.RouteState3 = o->RouteState3;
	terminal.RouteState4 = o->RouteState4;
	terminal.RouteState5 = o->RouteState5;
	terminal.RouteState6 = o->RouteState6;
	terminal.HighValue = o->highPressureValue;
	terminal.HighOffset = o->highPressureOffset;
	terminal.HighSymbol = o->highPressureSymbol;
	terminal.LowValue = o->lowPressureValue;
	o->id = m_db2.InsertTerminal(terminal, parentid);

    proMonitor *po = monitor(parentid);

    if (NULL != po)
    {
        o->parent = po;
        po->lst.append(o);
    }

    return true;
}
bool DatabaseProxy::addData(proData *o)
{
	DATA d;
	d.TerminalAddr = o->TerminalAddr;
	d.ConcentratorAddr = o->ConcentratorAddr;
	d.CollectTime = o->CollectTime;
	d.relaycnt = o->relaycnt;
	d.relayPosition = o->relayPosition;
	d.GetherUnitAddr = o->GetherUnitAddr;
	d.vValue = o->vValue;
	d.vAngValue = o->vAngValue;
	d.iValue = o->iValue;
	d.iAngValue = o->iAngValue;
	d.intRev1 = o->intRev1;
	d.intRev2 = o->intRev2;
	d.intRev3 = o->intRev3;
	d.intRev4 = o->intRev4;
	d.intRev5 = o->intRev5;
	d.intRev6 = o->intRev6;
	o->DataID = m_db2.InsertData(d);
	return true;
}

bool DatabaseProxy::addWarning(proWarning *o)
{
	WARNING w;
	w.WarningTime = o->WarningTime;
	w.WarningLine = o->WarningLine;
	w.MonitorAddr1 = o->MonitorAddr1;
	w.MonitorAddr2 = o->MonitorAddr2;
	w.Type = o->Type;
	w.iValue1 = o->iValue1;
	w.iValue2 = o->iValue2;
	w.WorkerName = o->WorkerName.toStdString();
	w.WarningInfo = o->WarningInfo.toStdString();
	w.SendTime = o->SendTime;
	w.SendState = o->SendState;
	m_db2.InsertWarning(w);
	return true;
}


bool DatabaseProxy::delCompany(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		if (pCompany->id == id)
//		{
//			for (int i = 0; i < pCompany->lst.size(); i++)
//			{
//				proSubCompany *pSubCompany = pCompany->lst.at(i);
//				for (int i = 0; pSubCompany->lst.size(); i++)
//				{
//					proAmso *pAmso = pSubCompany->lst.at(i);
//					for (int i = 0; i < pAmso->lst.size(); i++)
//					{
//						proRoute *pRoute = pAmso->lst.at(i);
//						for (int i = 0; i < pRoute->lst.size(); i++)
//						{
//							proConcentrator *pConcentrator = pRoute->lst.at(i);
//							for (int i = 0; i < pConcentrator->lst.size(); i++)
//							{
//								proLine *pLine = pConcentrator->lst.at(i);
//								for (int i = 0; i < pLine->lst.size(); i++)
//								{
//									proMonitor *pMonitor = pLine->lst.at(i);
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//								if (m_db2.DelLine(pLine->id))
//								{
//									pConcentrator->lst.removeOne(pLine);
//									delete pLine;
//								}
//							}
//							if (m_db2.DelConcentrator(pConcentrator->id))
//							{
//								pRoute->lst.removeOne(pConcentrator);
//								delete pConcentrator;
//							}
//						}
//						if (m_db2.DelRoute(pRoute->id))
//						{
//							pAmso->lst.removeOne(pRoute);
//							delete pRoute;
//						}
//					}
//					if (m_db2.DelAMSO(pAmso->id))
//					{
//						pSubCompany->lst.removeOne(pAmso);
//						delete pAmso;
//					}
//				}
//				if (m_db2.DelSubCompany(pSubCompany->id))
//				{
//					pCompany->lst.removeOne(pSubCompany);
//					delete pSubCompany;
//				}
//			}
//			if (m_db2.DelCompany(pCompany->id))
//			{
//				_lst.removeOne(pCompany);
//				delete pCompany;
//			}
//		}
		
//	}
	
    foreach(proCompany * o1, _lst) {
        if (o1->id == id)
        {
            if (!o1->lst.isEmpty())
            {
                return false;
            }
            if (m_db2.DelCompany(id))
            {
                _lst.removeOne(o1);
                delete o1;

                return true;
            }
        }
    }

    return false;
}

bool DatabaseProxy::delSubCompany(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			if (pSubCompany->id == id)
//			{
//				for (int i = 0; pSubCompany->lst.size(); i++)
//				{
//					proAmso *pAmso = pSubCompany->lst.at(i);
//					for (int i = 0; i < pAmso->lst.size(); i++)
//					{
//						proRoute *pRoute = pAmso->lst.at(i);
//						for (int i = 0; i < pRoute->lst.size(); i++)
//						{
//							proConcentrator *pConcentrator = pRoute->lst.at(i);
//							for (int i = 0; i < pConcentrator->lst.size(); i++)
//							{
//								proLine *pLine = pConcentrator->lst.at(i);
//								for (int i = 0; i < pLine->lst.size(); i++)
//								{
//									proMonitor *pMonitor = pLine->lst.at(i);
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//								if (m_db2.DelLine(pLine->id))
//								{
//									pConcentrator->lst.removeOne(pLine);
//									delete pLine;
//								}
//							}
//							if (m_db2.DelConcentrator(pConcentrator->id))
//							{
//								pRoute->lst.removeOne(pConcentrator);
//								delete pConcentrator;
//							}
//						}
//						if (m_db2.DelRoute(pRoute->id))
//						{
//							pAmso->lst.removeOne(pRoute);
//							delete pRoute;
//						}
//					}
//					if (m_db2.DelAMSO(pAmso->id))
//					{
//						pSubCompany->lst.removeOne(pAmso);
//						delete pAmso;
//					}
//				}
//				if (m_db2.DelSubCompany(pSubCompany->id))
//				{
//					pCompany->lst.removeOne(pSubCompany);
//					delete pSubCompany;
//				}
//			}
//		}
//	}

    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            if (o2->id == id)
            {
                if (!o2->lst.isEmpty())
                {
                    return false;
                }

                if (m_db2.DelSubCompany(id))
                {
                    o1->lst.removeOne(o2);
                    delete o2;

                    return true;
                }
            }
        }
    }

	return false;
}

bool DatabaseProxy::delAmso(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			for (int i = 0; pSubCompany->lst.size(); i++)
//			{
//				proAmso *pAmso = pSubCompany->lst.at(i);
//				if (pAmso->id == id)
//				{
//					for (int i = 0; i < pAmso->lst.size(); i++)
//					{
//						proRoute *pRoute = pAmso->lst.at(i);
//						for (int i = 0; i < pRoute->lst.size(); i++)
//						{
//							proConcentrator *pConcentrator = pRoute->lst.at(i);
//							for (int i = 0; i < pConcentrator->lst.size(); i++)
//							{
//								proLine *pLine = pConcentrator->lst.at(i);
//								for (int i = 0; i < pLine->lst.size(); i++)
//								{
//									proMonitor *pMonitor = pLine->lst.at(i);
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//								if (m_db2.DelLine(pLine->id))
//								{
//									pConcentrator->lst.removeOne(pLine);
//									delete pLine;
//								}
//							}
//							if (m_db2.DelConcentrator(pConcentrator->id))
//							{
//								pRoute->lst.removeOne(pConcentrator);
//								delete pConcentrator;
//							}
//						}
//						if (m_db2.DelRoute(pRoute->id))
//						{
//							pAmso->lst.removeOne(pRoute);
//							delete pRoute;
//						}
//					}
//					if (m_db2.DelAMSO(pAmso->id))
//					{
//						pSubCompany->lst.removeOne(pAmso);
//						delete pAmso;
//					}
//				}
//			}
//		}
//	}

    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                if (o3->id == id)
                {
                    if (!o3->lst.isEmpty())
                    {
                        return false;
                    }

                    if (m_db2.DelAMSO(id))
                    {
                        o2->lst.removeOne(o3);
                        delete o3;

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool DatabaseProxy::delRoute(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			for (int i = 0; pSubCompany->lst.size(); i++)
//			{
//				proAmso *pAmso = pSubCompany->lst.at(i);
//				for (int i = 0; i < pAmso->lst.size(); i++)
//				{
//					proRoute *pRoute = pAmso->lst.at(i);
//					if (pRoute->id == id)
//					{
//						for (int i = 0; i < pRoute->lst.size(); i++)
//						{
//							proConcentrator *pConcentrator = pRoute->lst.at(i);
//							for (int i = 0; i < pConcentrator->lst.size(); i++)
//							{
//								proLine *pLine = pConcentrator->lst.at(i);
//								for (int i = 0; i < pLine->lst.size(); i++)
//								{
//									proMonitor *pMonitor = pLine->lst.at(i);
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//								if (m_db2.DelLine(pLine->id))
//								{
//									pConcentrator->lst.removeOne(pLine);
//									delete pLine;
//								}
//							}
//							if (m_db2.DelConcentrator(pConcentrator->id))
//							{
//								pRoute->lst.removeOne(pConcentrator);
//								delete pConcentrator;
//							}
//						}
//						if (m_db2.DelRoute(pRoute->id))
//						{
//							pAmso->lst.removeOne(pRoute);
//							delete pRoute;
//						}
//					}
//				}
//			}
//		}
//	}

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

                        if (m_db2.DelRoute(id))
                        {
                            o3->lst.removeOne(o4);
                            delete o4;

                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool DatabaseProxy::delConcentrator(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			for (int i = 0; pSubCompany->lst.size(); i++)
//			{
//				proAmso *pAmso = pSubCompany->lst.at(i);
//				for (int i = 0; i < pAmso->lst.size(); i++)
//				{
//					proRoute *pRoute = pAmso->lst.at(i);
//					for (int i = 0; i < pRoute->lst.size(); i++)
//					{
//						proConcentrator *pConcentrator = pRoute->lst.at(i);
//						if (pConcentrator->id == id)
//						{
//							for (int i = 0; i < pConcentrator->lst.size(); i++)
//							{
//								proLine *pLine = pConcentrator->lst.at(i);
//								for (int i = 0; i < pLine->lst.size(); i++)
//								{
//									proMonitor *pMonitor = pLine->lst.at(i);
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//								if (m_db2.DelLine(pLine->id))
//								{
//									pConcentrator->lst.removeOne(pLine);
//									delete pLine;
//								}
//							}
//							if (m_db2.DelConcentrator(pConcentrator->id))
//							{
//								pRoute->lst.removeOne(pConcentrator);
//								delete pConcentrator;
//							}
//						}
//					}
//				}
//			}
//		}
//	}

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

                            if (m_db2.DelConcentrator(id))
                            {
                                o4->lst.removeOne(o5);
                                delete o5;

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

bool DatabaseProxy::delLine(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			for (int i = 0; pSubCompany->lst.size(); i++)
//			{
//				proAmso *pAmso = pSubCompany->lst.at(i);
//				for (int i = 0; i < pAmso->lst.size(); i++)
//				{
//					proRoute *pRoute = pAmso->lst.at(i);
//					for (int i = 0; i < pRoute->lst.size(); i++)
//					{
//						proConcentrator *pConcentrator = pRoute->lst.at(i);
//						for (int i = 0; i < pConcentrator->lst.size(); i++)
//						{
//							proLine *pLine = pConcentrator->lst.at(i);
//							if (pLine->id == id)
//							{
//								for (int i = 0; i < pLine->lst.size(); i++)
//								{
//									proMonitor *pMonitor = pLine->lst.at(i);
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//								if (m_db2.DelLine(pLine->id))
//								{
//									pConcentrator->lst.removeOne(pLine);
//									delete pLine;
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}

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

                                if (m_db2.DelLine(id))
                                {
                                    o5->lst.removeOne(o6);
                                    delete o6;

                                    return true;
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

bool DatabaseProxy::delMonitor(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			for (int i = 0; pSubCompany->lst.size(); i++)
//			{
//				proAmso *pAmso = pSubCompany->lst.at(i);
//				for (int i = 0; i < pAmso->lst.size(); i++)
//				{
//					proRoute *pRoute = pAmso->lst.at(i);
//					for (int i = 0; i < pRoute->lst.size(); i++)
//					{
//						proConcentrator *pConcentrator = pRoute->lst.at(i);
//						for (int i = 0; i < pConcentrator->lst.size(); i++)
//						{
//							proLine *pLine = pConcentrator->lst.at(i);
//							for (int i = 0; i < pLine->lst.size(); i++)
//							{
//								proMonitor *pMonitor = pLine->lst.at(i);
//								if (pMonitor->id == id)
//								{
//									for (int i = 0; i < pMonitor->lst.size(); i++)
//									{
//										proTerminal *pTerminal = pMonitor->lst.at(i);
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//									if (m_db2.DelMonitor(pMonitor->id))
//									{
//										pLine->lst.removeOne(pMonitor);
//										delete pMonitor;
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	return true;

    foreach(proCompany * o1, _lst) {
        foreach(proSubCompany * o2, o1->lst) {
            foreach(proAmso * o3, o2->lst) {
                foreach(proRoute * o4, o3->lst) {
                    foreach(proConcentrator * o5, o4->lst) {
                        foreach(proLine * o6, o5->lst) {
                            foreach(proMonitor * o7, o6->lst) {
                                if (o7->id == id)
                                {
                                    if (!o7->lst.isEmpty())
                                    {
                                        return false;
                                    }

                                    if (m_db2.DelMonitor(id))
                                    {
                                        o6->lst.removeOne(o7);
                                        delete o7;

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

bool DatabaseProxy::delTerminal(int id)
{
//	for (int i = 0; i < _lst.size(); i++)
//	{
//		proCompany *pCompany = _lst.at(i);
//		for (int i = 0; i < pCompany->lst.size(); i++)
//		{
//			proSubCompany *pSubCompany = pCompany->lst.at(i);
//			for (int i = 0; pSubCompany->lst.size(); i++)
//			{
//				proAmso *pAmso = pSubCompany->lst.at(i);
//				for (int i = 0; i < pAmso->lst.size(); i++)
//				{
//					proRoute *pRoute = pAmso->lst.at(i);
//					for (int i = 0; i < pRoute->lst.size(); i++)
//					{
//						proConcentrator *pConcentrator = pRoute->lst.at(i);
//						for (int i = 0; i < pConcentrator->lst.size(); i++)
//						{
//							proLine *pLine = pConcentrator->lst.at(i);
//							for (int i = 0; i < pLine->lst.size(); i++)
//							{
//								proMonitor *pMonitor = pLine->lst.at(i);
//								for (int i = 0; i < pMonitor->lst.size(); i++)
//								{
//									proTerminal *pTerminal = pMonitor->lst.at(i);
//									if (pTerminal->id == id)
//									{
//										if (m_db2.DelTerminal(pTerminal->id))
//										{
//											pMonitor->lst.removeOne(pTerminal);
//											delete pTerminal;
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}

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
                                        if (m_db2.DelTerminal(id))
                                        {
                                            o7->lst.removeOne(o8);
                                            delete o8;
                                        }

                                        // if monitor child num = 0, delete monitor
                                        if (o7->lst.isEmpty())
                                        {
                                            if (m_db2.DelMonitor(o7->id))
                                            {
                                                o6->lst.removeOne(o7);
                                                delete o7;
                                            }
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
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		if (pCompany->id == id)
		{
			return pCompany;
		}
		
	}
	
    //foreach(proCompany * o1, _lst) {
    //    if (o1->id == id)
    //    {
    //        return o1;
    //    }
    //}

    return NULL;
}

proSubCompany *DatabaseProxy::subCompany(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			if (pSubCompany->id == id)
			{
				return pSubCompany;
			}	
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        if (o2->id == id)
    //        {
    //            return o2;
    //        }
    //    }
    //}

    return NULL;
}

proAmso *DatabaseProxy::amso(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				if (pAmso->id == id)
				{
					return pAmso;
				}
				
			}
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        foreach(proAmso * o3, o2->lst) {
    //            if (o3->id == id)
    //            {
    //                return o3;
    //            }
    //        }
    //    }
    //}

    return NULL;
}

proRoute *DatabaseProxy::route(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					if (pRoute->id == id)
					{
						return pRoute;
					}
				}
			}
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        foreach(proAmso * o3, o2->lst) {
    //            foreach(proRoute * o4, o3->lst) {
    //                if (o4->id == id)
    //                {
    //                    return o4;
    //                }
    //            }
    //        }
    //    }
    //}

    return NULL;
}

proConcentrator *DatabaseProxy::concentrator(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					for (int i = 0; i < pRoute->lst.size(); i++)
					{
						proConcentrator *pConcentrator = pRoute->lst.at(i);
						if (pConcentrator->id == id)
						{
							return pConcentrator;
						}
					}
				}
			}
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        foreach(proAmso * o3, o2->lst) {
    //            foreach(proRoute * o4, o3->lst) {
    //                foreach(proConcentrator * o5, o4->lst) {
    //                    if (o5->id == id)
    //                    {
    //                        return o5;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    return NULL;
}

proLine *DatabaseProxy::line(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					for (int i = 0; i < pRoute->lst.size(); i++)
					{
						proConcentrator *pConcentrator = pRoute->lst.at(i);
						for (int i = 0; i < pConcentrator->lst.size(); i++)
						{
							proLine *pLine = pConcentrator->lst.at(i);
							if (pLine->id == id)
							{
								return pLine;
							}
						}
					}
				}
			}
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        foreach(proAmso * o3, o2->lst) {
    //            foreach(proRoute * o4, o3->lst) {
    //                foreach(proConcentrator * o5, o4->lst) {
    //                    foreach(proLine * o6, o5->lst) {
    //                        if (o6->id == id)
    //                        {
    //                            return o6;
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    return NULL;
}

proMonitor *DatabaseProxy::monitor(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					for (int i = 0; i < pRoute->lst.size(); i++)
					{
						proConcentrator *pConcentrator = pRoute->lst.at(i);
						for (int i = 0; i < pConcentrator->lst.size(); i++)
						{
							proLine *pLine = pConcentrator->lst.at(i);
							for (int i = 0; i < pLine->lst.size(); i++)
							{
								proMonitor *pMonitor = pLine->lst.at(i);
								if (pMonitor->id == id)
								{
									return pMonitor;
								}
							}
						}
					}
				}
			}
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        foreach(proAmso * o3, o2->lst) {
    //            foreach(proRoute * o4, o3->lst) {
    //                foreach(proConcentrator * o5, o4->lst) {
    //                    foreach(proLine * o6, o5->lst) {
    //                        foreach(proMonitor * o7, o6->lst) {
    //                            if (o7->id == id)
    //                            {
    //                                return o7;
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    return NULL;
}

proTerminal *DatabaseProxy::terminal(int id)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					for (int i = 0; i < pRoute->lst.size(); i++)
					{
						proConcentrator *pConcentrator = pRoute->lst.at(i);
						for (int i = 0; i < pConcentrator->lst.size(); i++)
						{
							proLine *pLine = pConcentrator->lst.at(i);
							for (int i = 0; i < pLine->lst.size(); i++)
							{
								proMonitor *pMonitor = pLine->lst.at(i);
								for (int i = 0; i < pMonitor->lst.size(); i++)
								{
									proTerminal *pTerminal = pMonitor->lst.at(i);
									if (pTerminal->id == id)
									{
										return pTerminal;
									}
								}
							}
						}
					}
				}
			}
		}
	}
    //foreach(proCompany * o1, _lst) {
    //    foreach(proSubCompany * o2, o1->lst) {
    //        foreach(proAmso * o3, o2->lst) {
    //            foreach(proRoute * o4, o3->lst) {
    //                foreach(proConcentrator * o5, o4->lst) {
    //                    foreach(proLine * o6, o5->lst) {
    //                        foreach(proMonitor * o7, o6->lst) {
    //                            foreach(proTerminal * o8, o7->lst) {
    //                                if (o8->id == id)
    //                                {
    //                                    return o8;
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

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

QList<proData> DatabaseProxy::historyData(int ConcentratorId, QDateTime begin, QDateTime end)
{
    QList<proData> lst;

    for (int i = 0; i < 20; i++)
    {
        proData data;
        data.CollectTime = QDateTime::currentDateTime().toMSecsSinceEpoch(); // 采集时间
        data.iValueA = 5 + i;//采集电流值
        data.iValueB = 5 + i * 2;//采集电流值
        data.iValueC = 55 + i;//采集电流值
        lst.append(data);
    }

    return lst;
}


void DatabaseProxy::GetShowDataInfoByConcentratorID(showData &sData, int ConcentratorId)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		sData.company = pCompany->name;
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			sData.subCompany = pSubCompany->name;
			for (int i = 0; pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				sData.amso = pAmso->name;
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					sData.route = pRoute->name;
					for (int i = 0; i < pRoute->lst.size(); i++)
					{
						proConcentrator *pConcentrator = pRoute->lst.at(i);
						sData.concentrator = pConcentrator->name;
						if (pConcentrator->id == ConcentratorId)
						{
							return;
						}
					}
				}
			}
		}
	}
}

bool DatabaseProxy::historyDataByTime(QList<showData> &pDatalist, int ConcentratorId)
{
	vector<DATA> vdata;
	m_db2.GetDatabyConcentratorID(vdata, ConcentratorId);
	showData sData;
	GetShowDataInfoByConcentratorID(sData, ConcentratorId);
	proConcentrator *pConcentrator =  concentrator(ConcentratorId);
	for (int i = 0; i < pConcentrator->lst.size(); i++)
	{
		proLine *pLine = pConcentrator->lst.at(i);
		sData.line = pLine->name;
		for (int i = 0; i < pLine->lst.size(); i++)
		{
			proMonitor *pMonitor = pLine->lst.at(i);
			sData.monitor = pMonitor->name;

			if (pMonitor->lst.size() == 1)
			{
				proTerminal *pTerminal = pMonitor->lst.at(0);
				vector<DATA> terminalData1;
				m_db2.GetDatabyTerminalID(terminalData1, ConcentratorId, pTerminal->addr);
				for (int i = 0; i < terminalData1.size(); i++)
				{
					DATA d = terminalData1[i];
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminal->addr, d.CollectTime);
					pDatalist<<sData;
				}
				
			}
			else if (pMonitor->lst.size() == 2)
			{
				proTerminal *pTerminal1 = pMonitor->lst.at(0);
				proTerminal *pTerminal2 = pMonitor->lst.at(1);
				vector<DATA> terminalData1;
				m_db2.GetDatabyTerminalID(terminalData1, ConcentratorId, pTerminal1->addr);
				for (int i = 0; i < terminalData1.size(); i++)
				{
					DATA d = terminalData1[i];
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminal1->addr, d.CollectTime);
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueB, pTerminal2->addr, d.CollectTime);
					pDatalist<<sData;
				}
			}
			else
			{
				proTerminal *pTerminal1 = pMonitor->lst.at(0);
				proTerminal *pTerminal2 = pMonitor->lst.at(1);
				proTerminal *pTerminal3 = pMonitor->lst.at(2);
				vector<DATA> terminalData1;
				m_db2.GetDatabyTerminalID(terminalData1, ConcentratorId, pTerminal1->addr);
				for (int i = 0; i < terminalData1.size(); i++)
				{
					DATA d = terminalData1[i];
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminal1->addr, d.CollectTime);
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueB, pTerminal2->addr, d.CollectTime);
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueC, pTerminal3->addr, d.CollectTime);
					pDatalist<<sData;
				}
			}
		}
		
	}
	return true;
}


bool DatabaseProxy::historyDataByTime(QList<showData> &pDatalist, int ConcentratorId, INT64 begin, INT64 end)
{
	vector<DATA> vdata;
	m_db2.GetDatabyConcentratorID(vdata, ConcentratorId);
	showData sData;
	GetShowDataInfoByConcentratorID(sData, ConcentratorId);
	proConcentrator *pConcentrator =  concentrator(ConcentratorId);
	for (int i = 0; i < pConcentrator->lst.size(); i++)
	{
		proLine *pLine = pConcentrator->lst.at(i);
		sData.line = pLine->name;
		for (int i = 0; i < pLine->lst.size(); i++)
		{
			proMonitor *pMonitor = pLine->lst.at(i);
			sData.monitor = pMonitor->name;

			if (pMonitor->lst.size() == 1)
			{
				proTerminal *pTerminal = pMonitor->lst.at(0);
				vector<DATA> terminalData1;
				m_db2.GetDatabyTerminalIDAndData(terminalData1, ConcentratorId, pTerminal->addr, begin, end);
				for (int i = 0; i < terminalData1.size(); i++)
				{
					DATA d = terminalData1[i];
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminal->addr, d.CollectTime);
					pDatalist<<sData;
				}

			}
			else if (pMonitor->lst.size() == 2)
			{
				proTerminal *pTerminal1 = pMonitor->lst.at(0);
				proTerminal *pTerminal2 = pMonitor->lst.at(1);
				vector<DATA> terminalData1;
				m_db2.GetDatabyTerminalIDAndData(terminalData1, ConcentratorId, pTerminal1->addr, begin, end);
				for (int i = 0; i < terminalData1.size(); i++)
				{
					DATA d = terminalData1[i];
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminal1->addr, d.CollectTime);
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueB, pTerminal2->addr, d.CollectTime);
					pDatalist<<sData;
				}
			}
			else
			{
				proTerminal *pTerminal1 = pMonitor->lst.at(0);
				proTerminal *pTerminal2 = pMonitor->lst.at(1);
				proTerminal *pTerminal3 = pMonitor->lst.at(2);
				vector<DATA> terminalData1;
				m_db2.GetDatabyTerminalIDAndData(terminalData1, ConcentratorId, pTerminal1->addr, begin, end);
				for (int i = 0; i < terminalData1.size(); i++)
				{
					DATA d = terminalData1[i];
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminal1->addr, d.CollectTime);
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueB, pTerminal2->addr, d.CollectTime);
					m_db2.GetDatabyTerminalAddrAndTime(sData.valueC, pTerminal3->addr, d.CollectTime);
					pDatalist<<sData;
				}
			}
		}

	}
	return true;
}

bool DatabaseProxy::historyWarning(QList<proWarning> &pDatalist)
{
	vector<WARNING> vdata;
	m_db2.GetWarning(vdata);
	for (int i = 0; i < vdata.size(); i++)
	{
		proWarning pWarning;
		pWarning.WarningTime = vdata[i].WarningTime;
		pWarning.WarningLine = vdata[i].WarningLine;
		pWarning.MonitorAddr1 = vdata[i].MonitorAddr1;
		pWarning.MonitorAddr2 = vdata[i].MonitorAddr2;
		pWarning.Type = vdata[i].Type;
		pWarning.iValue1 = vdata[i].iValue1;
		pWarning.iValue2 = vdata[i].iValue2;
		pWarning.WorkerName = QString::fromStdString(vdata[i].WorkerName);
		pWarning.WarningInfo = QString::fromStdString(vdata[i].WarningInfo);
		pWarning.SendTime = vdata[i].SendTime;
		pWarning.SendState = vdata[i].SendState;
		pDatalist<<pWarning;
	}
	
	return true;
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
