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

bool DatabaseProxy::connectDB(const QString &ip, const QString &usr, const QString &pwd)
{
    string strIP = ToString(ip);
    string strUsr = ToString(usr);
    string strPwd = ToString(pwd);

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
    return m_db2.GetUserID(ToString(usr),ToString(pwd));
}

int DatabaseProxy::userLevel(int id)
{
    return m_db2.GetUserLever(id);
}

int DatabaseProxy::addUser(const QString &usr, const QString &pwd, int level)
{
    return m_db2.InsertUser(ToString(usr),ToString(pwd), level);
}

bool DatabaseProxy::modifyUser(int id, const QString &usr, const QString &pwd, int level)
{
    m_db2.ModifyUser(id, ToString(usr),ToString(pwd), level);
    return true;
}

bool DatabaseProxy::delUser(int id)
{
	m_db2.DelUser(id);
    return true;
}

QList<proAmso> DatabaseProxy::amsos()
{
	QList<proAmso> ams;
	vector<AMSO> vAmso;
	m_db2.GetAllAMSO(vAmso);
	for (int i = 0; i < vAmso.size(); i++)
	{
		proAmso pAmso(NULL);
		pAmso.id = vAmso[i].AMSOID;
        pAmso.name = ToQString(vAmso[i].strName);//(vAmso[i].strName);
        pAmso.desc = ToQString(vAmso[i].strDescribe);//(vAmso[i].strDescribe);
		ams<<pAmso;
	}

	return ams;
}

QList<proUser> DatabaseProxy::users()
{
    // 胖子
    QList<proUser> usrs;
	vector<USR> vUsr;
	m_db2.GetAllUsers(vUsr);
	for (int i = 0; i < vUsr.size(); i++)
	{
		proUser pUsr;
		pUsr.id = vUsr[i].usrID;
        pUsr.name = ToQString(vUsr[i].usrName);//(vUsr[i].usrName);
        pUsr.pwd = ToQString(vUsr[i].usrPassWord);//(vUsr[i].usrPassWord);
		pUsr.level = vUsr[i].usrLever;
		usrs<<pUsr;
	}
	
    return usrs;
}

QList<proWorker> DatabaseProxy::workers()
{
	QList<proWorker> lst;
	vector<WORKER> vWorker;
	m_db2.GetAllWorker(vWorker);
	for (int i = 0; i < vWorker.size(); i++)
	{
		proWorker pWorker;
		pWorker.id = vWorker[i].id;
		pWorker.amsoId = vWorker[i].amsoId;
        pWorker.name = ToQString(vWorker[i].name);//(vWorker[i].name);
        pWorker.remarks = ToQString(vWorker[i].remarks);//(vWorker[i].remarks);
        pWorker.phone = ToQString(vWorker[i].phone);//(vWorker[i].phone);
		lst<<pWorker;
	}
	
	return lst;
}

bool DatabaseProxy::worker(int id, proWorker &worker)
{
	WORKER pWorker;
	m_db2.GetWorker(pWorker, id);
	worker.id = id;
    worker.name = ToQString(pWorker.name);//(pWorker.name);
	worker.amsoId = pWorker.amsoId;
    worker.remarks = ToQString(pWorker.remarks);//(pWorker.remarks);
    worker.phone = ToQString(pWorker.phone);//(pWorker.phone);
	return true;
}

int DatabaseProxy::addWorker(proWorker *u)
{
	WORKER worker;
	worker.amsoId = u->amsoId;
    worker.name = ToString(u->name);
    worker.remarks = ToString(u->remarks);
    worker.phone = ToString(u->phone);
	return m_db2.InsertWorker(worker);
}

bool DatabaseProxy::delWorker(int id)
{
	m_db2.DelWorker(id);
	return true;
}

bool DatabaseProxy::modifyWorker(proWorker *u)
{
	WORKER worker;
	worker.amsoId = u->amsoId;
    worker.name = ToString(u->name);
    worker.remarks = ToString(u->remarks);
    worker.phone = ToString(u->phone);
	m_db2.ModifyWorker(worker, u->id);
	return true;
}

bool DatabaseProxy::modifyWarningPopuped(int id)  // 客户端只修改报警数据中的字段isPopup=1
{
	m_db2.ModifyWarningPop(id);
	return true;
}

void DatabaseProxy::clearOrganizations()
{
    while (!_lst.isEmpty())
        delete _lst.takeFirst();
}

QList<proCompany *> DatabaseProxy::getOrganizations()
{
    if (!_lst.isEmpty()) {
        return _lst;
    }

    // read from db;
	clearOrganizations();

	vector<COMPANY> company; 
	m_db2.GetAllCompanyID(company);
	for (int i = 0; i < company.size(); i++)
	{
		proCompany *pCompany = new proCompany;
		pCompany->id = company[i].companyID;
        pCompany->name = ToQString(company[i].strName);
        pCompany->desc = ToQString(company[i].strDescribe);

		vector<SUBCOMPANY> subCompany;
		m_db2.GetAllSubCompanyByID(subCompany, company[i].companyID);
		for (int i = 0; i < subCompany.size(); i++)
		{
			proSubCompany *pSubCompany = new proSubCompany(pCompany);
			pSubCompany->id = subCompany[i].subCompanyID;
            pSubCompany->name = ToQString(subCompany[i].strName);//(subCompany[i].strName);
            pSubCompany->desc = ToQString(subCompany[i].strDescribe);//(subCompany[i].strDescribe);
			
			vector<AMSO> amso;
			m_db2.GetAllAMSOByID(amso, subCompany[i].subCompanyID);
			for (int i = 0; i < amso.size(); i++)
			{
				proAmso *pAmso = new proAmso(pSubCompany);
				pAmso->id = amso[i].AMSOID;
                pAmso->name = ToQString(amso[i].strName);//(amso[i].strName);
                pAmso->desc = ToQString(amso[i].strDescribe);//(amso[i].strDescribe);

				vector<ROUTE> route;
				m_db2.GetAllRouteByID(route, amso[i].AMSOID);
				for (int i = 0; i < route.size(); i++)
				{
					proRoute *pRoute = new proRoute(pAmso);
					pRoute->id = route[i].routeID;
                    pRoute->name = ToQString(route[i].strName);//(route[i].strName);
                    pRoute->desc = ToQString(route[i].strDescribe);//(route[i].strDescribe);

					vector<CONCENTRATOR> concentrator;
					m_db2.GetAllConcentratorByID(concentrator, route[i].routeID);
					for (int i = 0; i < concentrator.size(); i++)
					{
						proConcentrator *pConcentrator = new proConcentrator(pRoute);
						pConcentrator->id = concentrator[i].ConcentratorID;
                        pConcentrator->name = ToQString(concentrator[i].strName);//(concentrator[i].strName);
                        pConcentrator->destIp = ToQString(concentrator[i].strDestIP);//(concentrator[i].strDestIP);
                        pConcentrator->destPort = ToQString(concentrator[i].strDestPort);//(concentrator[i].strDestPort);
                        pConcentrator->type = ToQString(concentrator[i].strConnectType);//(concentrator[i].strConnectType);
                        pConcentrator->installAddr = ToQString(concentrator[i].strInstallPlace);//(concentrator[i].strInstallPlace);
                        pConcentrator->apName = ToQString(concentrator[i].strAPName);//(concentrator[i].strAPName);
                        pConcentrator->apProtocol = ToQString(concentrator[i].strAPProtocol);//(concentrator[i].strAPProtocol);
						pConcentrator->terminalTimer = concentrator[i].TerminalTimer;
						pConcentrator->concentratorTimer = concentrator[i].ConcentratorTimer;
						pConcentrator->heartTimer = concentrator[i].HeartTimer;
                        pConcentrator->strSimCard = ToQString(concentrator[i].strSimCard);//(concentrator[i].strSimCard);
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
                            pLine->name = ToQString(line[i].strName);//(line[i].strName);
                            pLine->type = line[i].strType;
                            pLine->addr = ToQString(line[i].strAddr);//(line[i].strAddr);
                            pLine->preAddr = ToQString(line[i].strPreAddr);//(line[i].strPreAddr);
                            pLine->nextAddr = ToQString(line[i].strNextAddr);//(line[i].strNextAddr);
							pLine->workerID = line[i].workerID;
							pLine->Ratio = line[i].Ratio;

							vector<MONITOR> monitor;
							m_db2.GetAllMonitorByID(monitor, line[i].lineID);
							for (int i = 0; i < monitor.size(); i++)
							{
								proMonitor *pMonitor = new proMonitor(pLine);
								pMonitor->id = monitor[i].MonitorID;
								pMonitor->addr = ToQString(monitor[i].MonitorAddr);
                                pMonitor->name = ToQString(monitor[i].strName);//(monitor[i].strName);
								pMonitor->PreMonitorID = monitor[i].PreMonitorID;

								vector<TERMINAL> terminal;
								m_db2.GetAllTerminalByID(terminal, monitor[i].MonitorID);
								for (int i = 0; i < terminal.size(); i++)
								{
									proTerminal *pTerminal = new proTerminal(pMonitor);
									pTerminal->id = terminal[i].TerminalID;
                                    pTerminal->name = ToQString(terminal[i].strName);//(terminal[i].strName);
                                    pTerminal->type = ToQString(terminal[i].strType);
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
    company.strName = ToString(o->name);
    company.strDescribe = ToString(o->desc);
    o->id = m_db2.InsertCompany(company);
    if (0 == o->id) {
        return false;
    }
    _lst.append(o);

    return true;
}

bool DatabaseProxy::addSubCompany(proSubCompany *o, int parentid)
{
	SUBCOMPANY subCompany;
	subCompany.companyID = parentid;
    subCompany.strName = ToString(o->name);
    subCompany.strDescribe = ToString(o->desc);
	o->id = m_db2.InsertSubCompany(subCompany, parentid);
    if (0 == o->id) {
        return false;
    }
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
    amso.strName = ToString(o->name);
    amso.strDescribe = ToString(o->desc);
	o->id = m_db2.InsertAMSO(amso, parentid);
    if (0 == o->id) {
        return false;
    }

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
    route.strName = ToString(o->name);
    route.strDescribe = ToString(o->desc);
	o->id = m_db2.InsertRoute(route, parentid);
    if (0 == o->id) {
        return false;
    }
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
    concentrator.strName = ToString(o->name);
    concentrator.strDestIP = ToString(o->destIp);
    concentrator.strDestPort = ToString(o->destPort);
    concentrator.strConnectType = ToString(o->type);
    concentrator.strInstallPlace = ToString(o->installAddr);
    concentrator.strAPName = ToString(o->apName);
    concentrator.strAPProtocol = ToString(o->apProtocol);
	concentrator.TerminalTimer = o->terminalTimer;
	concentrator.HeartTimer = o->heartTimer;
    concentrator.strSimCard = ToString(o->strSimCard);
	concentrator.GPRSReConnectTimer = o->gprsReConnectTimer;
	concentrator.GPRSSignalStrength = o->gprsSignalStrength;
	concentrator.SaveTimer = o->saveTimer;
	concentrator.wirelessSearchTimer = o->wirelessSearchTimer;
	concentrator.ConcentratorAddr = o->concentratorAddr;
	concentrator.ConcentratorCurrentTime = o->ConcentratorCurrentTime;
	concentrator.SelfReportOnOff = o->SelfReportOnOff;
    concentrator.ConcentratorTimer = o->concentratorTimer;
	o->id = m_db2.InsertConcentrator(concentrator, parentid);
    if (0 == o->id) {
        return false;
    }

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
    line.strName = ToString(o->name);
    line.strType = o->type;
    line.strAddr = ToString(o->addr);
    line.strPreAddr = ToString(o->preAddr);
    line.strNextAddr = ToString(o->nextAddr);
	line.workerID = o->workerID;
	line.Ratio = o->Ratio;
	o->id = m_db2.InsertLine(line, parentid);
    if (0 == o->id) {
        return false;
    }

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
    monitor.strName = ToString(o->name);
	monitor.MonitorAddr = ToString(o->addr);
	monitor.PreMonitorID = o->PreMonitorID;
	o->id = m_db2.InsertMonitor(monitor,parentid);
    if (0 == o->id) {
        return false;
    }

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
    terminal.strName = ToString(o->name);
    terminal.strType = ToString(o->type);
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
    if (0 == o->id) {
        return false;
    }

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

//添加指令
bool DatabaseProxy::addCommand(proCommand *o)
{
	COMMAND cmd;
	cmd.UserID = o->UserID;
    cmd.ConcentratorAddr = o->ConcentratorAddr;
    cmd.TerminalAddr = o->TerminalAddr;
	cmd.Commandtype = o->Commandtype;
	m_db2.InsertCommand(cmd);
	return true;
}

bool DatabaseProxy::addWarning(proWarning *o)
{
	WARNING w;
	w.WarningTime = o->WarningTime;
	w.WarningLine = o->WarningLine;
	w.MonitorAddr1 = ToString(o->MonitorAddr1);
	w.MonitorAddr2 = ToString(o->MonitorAddr2);
	w.Type = o->Type;
	w.iValue1 = o->iValue1;
	w.iValue2 = o->iValue2;
    w.WorkerName = ToString(o->WorkerName);
    w.WarningInfo = ToString(o->WarningInfo);
	w.SendTime = o->SendTime;
	w.SendState = o->SendState;
	w.Popuped = o->Popuped;
	m_db2.InsertWarning(w);
    return true;
}

bool DatabaseProxy::modifyCompany(proCompany *o)
{
	COMPANY company;
	company.companyID = o->id;
    company.strName = ToString(o->name);
    company.strDescribe = ToString(o->desc);
    m_db2.ModifyCompany(company, company.companyID);
    return true;
}

bool DatabaseProxy::modifySubCompany(proSubCompany *o)
{
	SUBCOMPANY subCompany;
	subCompany.subCompanyID = o->id;
	subCompany.companyID = o->parent->id;
    subCompany.strName = ToString(o->name);
    subCompany.strDescribe = ToString(o->desc);
    m_db2.InsertSubCompany(subCompany, subCompany.subCompanyID);
    return true;
}

bool DatabaseProxy::modifyAmso(proAmso *o)
{
	AMSO amso;
	amso.AMSOID = o->id;
	amso.subCompanyID = o->parent->id;
    amso.strName = ToString(o->name);
    amso.strDescribe = ToString(o->desc);
    m_db2.ModifyAMSO(amso, amso.AMSOID);
    return true;
}

bool DatabaseProxy::modifyRoute(proRoute *o)
{
	ROUTE route;
	route.routeID = o->id;
	route.AMSOID = o->parent->id;
    route.strName = ToString(o->name);//o->name.toStdString();
    route.strDescribe = ToString(o->desc);//o->desc.toStdString();
    m_db2.ModifyRoute(route, route.routeID);
    return true;
}

bool DatabaseProxy::modifyConcentrator(proConcentrator *o)
{
	CONCENTRATOR concentrator;
	concentrator.ConcentratorID = o->id;
	concentrator.routeID = o->parent->id;
    concentrator.strName = ToString(o->name);//o->name.toStdString();
    concentrator.strDestIP = ToString(o->destIp);//o->destIp.toStdString();
    concentrator.strDestPort = ToString(o->destPort);//o->destPort.toStdString();
    concentrator.strConnectType = ToString(o->type);//o->type.toStdString();
    concentrator.strInstallPlace = ToString(o->installAddr);//o->installAddr.toStdString();
    concentrator.strAPName = ToString(o->apName);//o->apName.toStdString();
    concentrator.strAPProtocol = ToString(o->apProtocol);//o->apProtocol.toStdString();
	concentrator.TerminalTimer = o->terminalTimer;
	concentrator.HeartTimer = o->heartTimer;
    concentrator.strSimCard = ToString(o->strSimCard);//o->strSimCard.toStdString();
	concentrator.GPRSReConnectTimer = o->gprsReConnectTimer;
	concentrator.GPRSSignalStrength = o->gprsSignalStrength;
	concentrator.SaveTimer = o->saveTimer;
	concentrator.wirelessSearchTimer = o->wirelessSearchTimer;
	concentrator.ConcentratorAddr = o->concentratorAddr;
	concentrator.ConcentratorCurrentTime = o->ConcentratorCurrentTime;
	concentrator.SelfReportOnOff = o->SelfReportOnOff;
	concentrator.ConcentratorTimer = o->concentratorTimer;
    m_db2.ModifyConcentrator(concentrator, concentrator.ConcentratorID);
    return true;
}

bool DatabaseProxy::modifyLine(proLine *o)
{
	LINE line;
	line.lineID = o->id;
	line.ConcentratorID = o->parent->id;
    line.strName = ToString(o->name);//o->name.toStdString();
	line.strType = o->type;
    line.strAddr = ToString(o->addr);//o->addr.toStdString();
    line.strPreAddr = ToString(o->preAddr);//o->preAddr.toStdString();
    line.strNextAddr = ToString(o->nextAddr);//o->nextAddr.toStdString();
	line.workerID = o->workerID;
	line.Ratio = o->Ratio;
    m_db2.ModifyLine(line, line.lineID);
    return true;
}

bool DatabaseProxy::modifyMonitor(proMonitor *o)
{
	MONITOR monitor;
	monitor.MonitorID = o->id;
	monitor.lineID = o->parent->id;
    monitor.strName = ToString(o->name);//o->name.toStdString();
	monitor.MonitorAddr = ToString(o->addr);
	monitor.PreMonitorID = o->PreMonitorID;
    m_db2.ModifyMonitor(monitor,monitor.MonitorID);
    return true;
}

bool DatabaseProxy::modifyTerminal(proTerminal *o)
{
	TERMINAL terminal;
	terminal.TerminalID = o->id;
	terminal.MonitorID = o->parent->id;
    terminal.strName = ToString(o->name);//o->name.toStdString();
    terminal.strType = ToString(o->type);//o->type.toStdString();
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
    m_db2.ModifyTerminal(terminal, terminal.TerminalID);
    return true;
}
/***************************************************************************************************************/
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
//                                        if (o7->lst.isEmpty())
//                                        {
//                                            if (m_db2.DelMonitor(o7->id))
//                                            {
//                                                o6->lst.removeOne(o7);
//                                                delete o7;
//                                            }
//                                        }

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
            for (int i = 0; i < pSubCompany->lst.size(); i++)
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
            for (int i = 0; i < pSubCompany->lst.size(); i++)
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

proConcentrator *DatabaseProxy::concentratorAddr(int addr)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			for (int i = 0; i < pSubCompany->lst.size(); i++)
			{
				proAmso *pAmso = pSubCompany->lst.at(i);
				for (int i = 0; i < pAmso->lst.size(); i++)
				{
					proRoute *pRoute = pAmso->lst.at(i);
					for (int i = 0; i < pRoute->lst.size(); i++)
					{
						proConcentrator *pConcentrator = pRoute->lst.at(i);
						if (pConcentrator->concentratorAddr == addr)
						{
							return pConcentrator;
						}
					}
				}
			}
		}
	}
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
			for (int i = 0; i < pSubCompany->lst.size(); i++)
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
			for (int i = 0; i < pSubCompany->lst.size(); i++)
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
			for (int i = 0; i < pSubCompany->lst.size(); i++)
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
			for (int i = 0; i < pSubCompany->lst.size(); i++)
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


void DatabaseProxy::GetShowDataInfoByConcentratorAddr(showData &sData, int ConcentratorAddr)
{
	for (int i = 0; i < _lst.size(); i++)
	{
		proCompany *pCompany = _lst.at(i);
		sData.company = pCompany->name;
		for (int i = 0; i < pCompany->lst.size(); i++)
		{
			proSubCompany *pSubCompany = pCompany->lst.at(i);
			sData.subCompany = pSubCompany->name;
			for (int i = 0; i < pSubCompany->lst.size(); i++)
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
						if (pConcentrator->concentratorAddr == ConcentratorAddr)
						{
							return;
						}
					}
				}
			}
		}
	}
}

//获得实时数据 注意这里传入的是终端地址
bool DatabaseProxy::realTimeData(vector<DATA> &v)
{
	m_db2.GetRealData(v);
	return true;
}

//获得所有的历史数据 注意这里传入的是终端
bool DatabaseProxy::historyDataByTerminalAddr(QList<proData> &pDatalist, int ConcentratorAddr, int TerminalAddr)
{
	vector<DATA> vData;
	m_db2.GetDatabyTerminalAddr(vData, ConcentratorAddr, TerminalAddr);
	for (int i = 0; i < vData.size(); i++)
	{
		DATA d = vData[i];
		proData pData;
		pData.DataID = d.DataID;
		pData.CollectTime = d.CollectTime;
		pData.ConcentratorAddr = d.ConcentratorAddr;
		pData.GetherUnitAddr = d.GetherUnitAddr;
		pData.iAngValue = d.iAngValue;
		pData.iValue = d.iValue;
		pData.vAngValue = d.vAngValue;
		pData.vValue = d.vValue;
		pData.relaycnt = d.relaycnt;
		pData.relayPosition = d.relayPosition;
		
		pDatalist<<pData;
	}
	return true;
}

bool DatabaseProxy::historyDataByTime(QList<showData> &pDatalist, int ConcentratorAddr)
{
	//vector<DATA> vdata;
	//m_db2.GetDatabyConcentratorAddr(vdata, ConcentratorAddr);
	showData sData;
	GetShowDataInfoByConcentratorAddr(sData, ConcentratorAddr);
	proConcentrator *pConcentrator =  concentratorAddr(ConcentratorAddr);
	if (pConcentrator ==  NULL)
	{
		return false;
	}
	vector<TIME_ID> vTime;
	m_db2.GetCollectTimeAndMoniterID(vTime);
	for (int i = 0; i < vTime.size(); i++)
	{
		proMonitor *mo = monitor(vTime[i].MonitorID);
		sData.line = mo->parent->name;
		sData.monitor = mo->name;
		if (mo->lst.size() != 3)
		{
			continue;
		}
		proTerminal *pTerminalA = mo->lst.at(0);
		proTerminal *pTerminalB = mo->lst.at(1);
		proTerminal *pTerminalC = mo->lst.at(2);

		memset(&sData.valueA, 0, sizeof(DATA));
		memset(&sData.valueB, 0, sizeof(DATA));
		memset(&sData.valueC, 0, sizeof(DATA));
		//获得A数据
		m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminalA->addr, vTime[i].CollectTime);
		//获得B数据
		m_db2.GetDatabyTerminalAddrAndTime(sData.valueB, pTerminalB->addr, vTime[i].CollectTime);
		//获得C数据
		m_db2.GetDatabyTerminalAddrAndTime(sData.valueC, pTerminalC->addr, vTime[i].CollectTime);

		pDatalist<<sData;
	}
	return true;
}


bool DatabaseProxy::historyDataByTime(QList<showData> &pDatalist, int ConcentratorAddr, INT64 begin, INT64 end)
{
	//vector<DATA> vdata;
	//m_db2.GetDatabyConcentratorAddr(vdata, ConcentratorAddr);
	showData sData;
	GetShowDataInfoByConcentratorAddr(sData, ConcentratorAddr);
	proConcentrator *pConcentrator =  concentratorAddr(ConcentratorAddr);
	if (pConcentrator ==  NULL)
	{
		return false;
	}
	vector<TIME_ID> vTime;
	m_db2.GetCollectTimeAndMoniterID(vTime);
	for (int i = 0; i < vTime.size(); i++)
	{
		if (begin > vTime[i].CollectTime)
		{
			continue;
		}
		if (end < vTime[i].CollectTime)
		{
			continue;
		}
		proMonitor *mo = monitor(vTime[i].MonitorID);
		sData.line = mo->parent->name;
		sData.monitor = mo->name;
		if (mo->lst.size() != 3)
		{
			continue;
		}
		proTerminal *pTerminalA = mo->lst.at(0);
		proTerminal *pTerminalB = mo->lst.at(1);
		proTerminal *pTerminalC = mo->lst.at(2);

		memset(&sData.valueA, 0, sizeof(DATA));
		memset(&sData.valueB, 0, sizeof(DATA));
		memset(&sData.valueC, 0, sizeof(DATA));
		//获得A数据
		m_db2.GetDatabyTerminalAddrAndTime(sData.valueA, pTerminalA->addr, vTime[i].CollectTime);
		//获得B数据
		m_db2.GetDatabyTerminalAddrAndTime(sData.valueB, pTerminalB->addr, vTime[i].CollectTime);
		//获得C数据
		m_db2.GetDatabyTerminalAddrAndTime(sData.valueC, pTerminalC->addr, vTime[i].CollectTime);

		pDatalist<<sData;
	}
	return true;
}

//获得所有的报警信息，包含已报的和未报的
bool DatabaseProxy::historyWarningAll(QList<proWarning> &pDatalist)
{
	vector<WARNING> vdata;
	m_db2.GetWarning(vdata);
	for (int i = 0; i < vdata.size(); i++)
	{
		proWarning pWarning;
		pWarning.WarningID = vdata[i].WarningID;
		pWarning.WarningTime = vdata[i].WarningTime;
		pWarning.WarningLine = vdata[i].WarningLine;
		pWarning.MonitorAddr1 = ToQString(vdata[i].MonitorAddr1);
		pWarning.MonitorAddr2 = ToQString(vdata[i].MonitorAddr2);
		pWarning.Type = vdata[i].Type;
		pWarning.iValue1 = vdata[i].iValue1;
		pWarning.iValue2 = vdata[i].iValue2;
        pWarning.WorkerName = ToQString(vdata[i].WorkerName);//(vdata[i].WorkerName);
        pWarning.WarningInfo = ToQString(vdata[i].WarningInfo);//(vdata[i].WarningInfo);
		pWarning.SendTime = vdata[i].SendTime;
		pWarning.SendState = vdata[i].SendState;
		pDatalist<<pWarning;
	}
	return true;
}
//获得已经报过警的信息
bool DatabaseProxy::historyWarningPoped(QList<proWarning> &pDatalist)
{
	vector<WARNING> vdata;
	m_db2.GetWarningPoped(vdata);
	for (int i = 0; i < vdata.size(); i++)
	{
		proWarning pWarning;
		pWarning.WarningID = vdata[i].WarningID;
		pWarning.WarningTime = vdata[i].WarningTime;
		pWarning.WarningLine = vdata[i].WarningLine;
		pWarning.MonitorAddr1 = ToQString(vdata[i].MonitorAddr1);
		pWarning.MonitorAddr2 = ToQString(vdata[i].MonitorAddr2);
		pWarning.Type = vdata[i].Type;
		pWarning.iValue1 = vdata[i].iValue1;
		pWarning.iValue2 = vdata[i].iValue2;
        pWarning.WorkerName = ToQString(vdata[i].WorkerName);//(vdata[i].WorkerName);
        pWarning.WarningInfo = ToQString(vdata[i].WarningInfo);//(vdata[i].WarningInfo);
		pWarning.SendTime = vdata[i].SendTime;
		pWarning.SendState = vdata[i].SendState;
		pDatalist<<pWarning;
	}
	return true;
}

bool DatabaseProxy::historyWarningNopop(QList<proWarning> &pDatalist)
{
	vector<WARNING> vdata;
	m_db2.GetWarningNopop(vdata);
	for (int i = 0; i < vdata.size(); i++)
	{
		proWarning pWarning;
		pWarning.WarningID = vdata[i].WarningID;
		pWarning.WarningTime = vdata[i].WarningTime;
		pWarning.WarningLine = vdata[i].WarningLine;
		pWarning.MonitorAddr1 = ToQString(vdata[i].MonitorAddr1);
		pWarning.MonitorAddr2 = ToQString(vdata[i].MonitorAddr2);
		pWarning.Type = vdata[i].Type;
		pWarning.iValue1 = vdata[i].iValue1;
		pWarning.iValue2 = vdata[i].iValue2;
        pWarning.WorkerName = ToQString(vdata[i].WorkerName);//(vdata[i].WorkerName);
        pWarning.WarningInfo = ToQString(vdata[i].WarningInfo);//(vdata[i].WarningInfo);
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
        QList<proData> datas;
        DatabaseProxy::instance().historyDataByTerminalAddr(datas, parent->parent->concentratorAddr, lst.first()->addr);
        if (!datas.isEmpty()) {
            return QString::number(datas.last().iValue);
        }
    }

    return QString("-");
}

QString proMonitor::pressureValueB()
{
    if (lst.count() > 1)
    {
        QList<proData> datas;
        DatabaseProxy::instance().historyDataByTerminalAddr(datas, parent->parent->concentratorAddr, lst.at(1)->addr);
        if (!datas.isEmpty()) {
            return QString::number(datas.last().iValue);
        }
    }

    return QString("-");
}

QString proMonitor::pressureValueC()
{
    if (lst.count() > 2)
    {
        QList<proData> datas;
        DatabaseProxy::instance().historyDataByTerminalAddr(datas, parent->parent->concentratorAddr, lst.at(2)->addr);
        if (!datas.isEmpty()) {
            return QString::number(datas.last().iValue);
        }
    }

    return QString("-");
}
#include <QTextCodec>
string DatabaseProxy::ToString(const QString &qstr)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");

    QByteArray arr = pCodec->fromUnicode(qstr);
    string cstr = arr.data();
    return cstr;
}

QString DatabaseProxy::ToQString(const string &cstr)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");

    QString qstr = pCodec->toUnicode(cstr.c_str(), cstr.length());
    return qstr;
}
