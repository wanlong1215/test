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
		pAmso.name = QString(QString::fromLocal8Bit(vAmso[i].strName.c_str()));//QString::fromStdString(vAmso[i].strName);
		pAmso.desc = QString(QString::fromLocal8Bit(vAmso[i].strDescribe.c_str()));//QString::fromStdString(vAmso[i].strDescribe);
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
		pUsr.name = QString(QString::fromLocal8Bit(vUsr[i].usrName.c_str()));//QString::fromStdString(vUsr[i].usrName);
		pUsr.pwd = QString(QString::fromLocal8Bit(vUsr[i].usrPassWord.c_str()));//QString::fromStdString(vUsr[i].usrPassWord);
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
		pWorker.name = QString(QString::fromLocal8Bit(vWorker[i].name.c_str()));//QString::fromStdString(vWorker[i].name);
		pWorker.remarks = QString(QString::fromLocal8Bit(vWorker[i].remarks.c_str()));//QString::fromStdString(vWorker[i].remarks);
		pWorker.phone = QString(QString::fromLocal8Bit(vWorker[i].phone.c_str()));//QString::fromStdString(vWorker[i].phone);
		lst<<pWorker;
	}
	
	return lst;
}

bool DatabaseProxy::worker(int id, proWorker &worker)
{
	WORKER pWorker;
	m_db2.GetWorker(pWorker, id);
	worker.id = id;
	worker.name = QString(QString::fromLocal8Bit(pWorker.name.c_str()));//QString::fromStdString(pWorker.name);
	worker.amsoId = pWorker.amsoId;
	worker.remarks = QString(QString::fromLocal8Bit(pWorker.remarks.c_str()));//QString::fromStdString(pWorker.remarks);
	worker.phone = QString(QString::fromLocal8Bit(pWorker.phone.c_str()));//QString::fromStdString(pWorker.phone);
	return true;
}

int DatabaseProxy::addWorker(proWorker *u)
{
	WORKER worker;
	worker.amsoId = u->amsoId;
	worker.name = string((const char *)u->name.toLocal8Bit());//u->name.toStdString();
	worker.remarks = string((const char *)u->remarks.toLocal8Bit());//u->remarks.toStdString();
	worker.phone = string((const char *)u->phone.toLocal8Bit());//u->phone.toStdString();
	m_db2.InsertWorker(worker);
	return 1;
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
	worker.name = string((const char *)u->name.toLocal8Bit());//u->name.toStdString();
	worker.remarks = string((const char *)u->remarks.toLocal8Bit());//u->remarks.toStdString();
	worker.phone = string((const char *)u->phone.toLocal8Bit());//u->phone.toStdString();
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
			pSubCompany->name = QString(QString::fromLocal8Bit(subCompany[i].strName.c_str()));//QString::fromStdString(subCompany[i].strName);
			pSubCompany->desc = QString(QString::fromLocal8Bit(subCompany[i].strDescribe.c_str()));//QString::fromStdString(subCompany[i].strDescribe);
			
			vector<AMSO> amso;
			m_db2.GetAllAMSOByID(amso, subCompany[i].subCompanyID);
			for (int i = 0; i < amso.size(); i++)
			{
				proAmso *pAmso = new proAmso(pSubCompany);
				pAmso->id = amso[i].AMSOID;
				pAmso->name = QString(QString::fromLocal8Bit(amso[i].strName.c_str()));//QString::fromStdString(amso[i].strName);
				pAmso->desc = QString(QString::fromLocal8Bit(amso[i].strDescribe.c_str()));//QString::fromStdString(amso[i].strDescribe);

				vector<ROUTE> route;
				m_db2.GetAllRouteByID(route, amso[i].AMSOID);
				for (int i = 0; i < route.size(); i++)
				{
					proRoute *pRoute = new proRoute(pAmso);
					pRoute->id = route[i].routeID;
					pRoute->name = QString(QString::fromLocal8Bit(route[i].strName.c_str()));//QString::fromStdString(route[i].strName);
					pRoute->desc = QString(QString::fromLocal8Bit(route[i].strDescribe.c_str()));//QString::fromStdString(route[i].strDescribe);

					vector<CONCENTRATOR> concentrator;
					m_db2.GetAllConcentratorByID(concentrator, route[i].routeID);
					for (int i = 0; i < concentrator.size(); i++)
					{
						proConcentrator *pConcentrator = new proConcentrator(pRoute);
						pConcentrator->id = concentrator[i].ConcentratorID;
						pConcentrator->name = QString(QString::fromLocal8Bit(concentrator[i].strName.c_str()));//QString::fromStdString(concentrator[i].strName);
						pConcentrator->destIp = QString(QString::fromLocal8Bit(concentrator[i].strDestIP.c_str()));//QString::fromStdString(concentrator[i].strDestIP);
						pConcentrator->destPort = QString(QString::fromLocal8Bit(concentrator[i].strDestPort.c_str()));//QString::fromStdString(concentrator[i].strDestPort);
						pConcentrator->type = QString(QString::fromLocal8Bit(concentrator[i].strConnectType.c_str()));//QString::fromStdString(concentrator[i].strConnectType);
						pConcentrator->installAddr = QString(QString::fromLocal8Bit(concentrator[i].strInstallPlace.c_str()));//QString::fromStdString(concentrator[i].strInstallPlace);
						pConcentrator->apName = QString(QString::fromLocal8Bit(concentrator[i].strAPName.c_str()));//QString::fromStdString(concentrator[i].strAPName);
						pConcentrator->apProtocol = QString(QString::fromLocal8Bit(concentrator[i].strAPProtocol.c_str()));//QString::fromStdString(concentrator[i].strAPProtocol);
						pConcentrator->terminalTimer = concentrator[i].TerminalTimer;
						pConcentrator->concentratorTimer = concentrator[i].ConcentratorTimer;
						pConcentrator->heartTimer = concentrator[i].HeartTimer;
						pConcentrator->strSimCard = QString(QString::fromLocal8Bit(concentrator[i].strSimCard.c_str()));//QString::fromStdString(concentrator[i].strSimCard);
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
							pLine->name = QString(QString::fromLocal8Bit(line[i].strName.c_str()));//QString::fromStdString(line[i].strName);
                            pLine->type = line[i].strType;
							pLine->addr = QString(QString::fromLocal8Bit(line[i].strAddr.c_str()));//QString::fromStdString(line[i].strAddr);
							pLine->preAddr = QString(QString::fromLocal8Bit(line[i].strPreAddr.c_str()));//QString::fromStdString(line[i].strPreAddr);
							pLine->nextAddr = QString(QString::fromLocal8Bit(line[i].strNextAddr.c_str()));//QString::fromStdString(line[i].strNextAddr);
							pLine->workerID = line[i].workerID;

							vector<MONITOR> monitor;
							m_db2.GetAllMonitorByID(monitor, line[i].lineID);
							for (int i = 0; i < monitor.size(); i++)
							{
								proMonitor *pMonitor = new proMonitor(pLine);
								pMonitor->id = monitor[i].MonitorID;
								pMonitor->name = QString(QString::fromLocal8Bit(monitor[i].strName.c_str()));//QString::fromStdString(monitor[i].strName);

								vector<TERMINAL> terminal;
								m_db2.GetAllTerminalByID(terminal, monitor[i].MonitorID);
								for (int i = 0; i < terminal.size(); i++)
								{
									proTerminal *pTerminal = new proTerminal(pMonitor);
									pTerminal->id = terminal[i].TerminalID;
									pTerminal->name = QString(QString::fromLocal8Bit(terminal[i].strName.c_str()));//QString::fromStdString(terminal[i].strName);
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
    company.strName = ToString(o->name);
    company.strDescribe = ToString(o->desc);
    o->id = m_db2.InsertCompany(company);
    _lst.append(o);

    return true;
}

bool DatabaseProxy::addSubCompany(proSubCompany *o, int parentid)
{
	SUBCOMPANY subCompany;
	subCompany.companyID = parentid;
	subCompany.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	subCompany.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
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
	amso.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	amso.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
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
	route.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	route.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
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
	concentrator.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	concentrator.strDestIP = string((const char *)o->destIp.toLocal8Bit());//o->destIp.toStdString();
	concentrator.strDestPort = string((const char *)o->destPort.toLocal8Bit());//o->destPort.toStdString();
	concentrator.strConnectType = string((const char *)o->type.toLocal8Bit());//o->type.toStdString();
	concentrator.strInstallPlace = string((const char *)o->installAddr.toLocal8Bit());//o->installAddr.toStdString();
	concentrator.strAPName = string((const char *)o->apName.toLocal8Bit());//o->apName.toStdString();
	concentrator.strAPProtocol = string((const char *)o->apProtocol.toLocal8Bit());//o->apProtocol.toStdString();
	concentrator.TerminalTimer = o->terminalTimer;
	concentrator.HeartTimer = o->heartTimer;
	concentrator.strSimCard = string((const char *)o->strSimCard.toLocal8Bit());//o->strSimCard.toStdString();
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
	line.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
    line.strType = o->type;
	line.strAddr = string((const char *)o->addr.toLocal8Bit());//o->addr.toStdString();
	line.strPreAddr = string((const char *)o->preAddr.toLocal8Bit());//o->preAddr.toStdString();
	line.strNextAddr = string((const char *)o->nextAddr.toLocal8Bit());//o->nextAddr.toStdString();
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
	monitor.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	monitor.MonitorAddr = o->addr;
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
	terminal.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	terminal.strType = string((const char *)o->type.toLocal8Bit());//o->type.toStdString();
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

//添加指令
bool DatabaseProxy::addCommand(proCommand *o)
{
	COMMAND cmd;
	cmd.UserID = o->UserID;
	cmd.CommandType = string((const char *)o->CommandType.toLocal8Bit());
	cmd.CommandInfo = string((const char *)o->CommandInfo.toLocal8Bit());
	m_db2.InsertCommand(cmd);
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
	w.WorkerName = string((const char *)o->WorkerName.toLocal8Bit());//o->WorkerName.toStdString();
	w.WarningInfo = string((const char *)o->WarningInfo.toLocal8Bit());//o->WarningInfo.toStdString();
	w.SendTime = o->SendTime;
	w.SendState = o->SendState;
	w.Popuped = o->Popuped;
	m_db2.InsertWarning(w);
    return true;
}

/***************************************************************************************************************/
// 胖子
bool DatabaseProxy::modifyCompany(proCompany *o)
{
	COMPANY company;
	company.companyID = o->id;
	company.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	company.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
	o->id = m_db2.ModifyCompany(company, company.companyID);
    return true;
}

bool DatabaseProxy::modifySubCompany(proSubCompany *o)
{
	SUBCOMPANY subCompany;
	subCompany.subCompanyID = o->id;
	subCompany.companyID = o->parent->id;
	subCompany.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	subCompany.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
	o->id = m_db2.InsertSubCompany(subCompany, subCompany.subCompanyID);
    return true;
}

bool DatabaseProxy::modifyAmso(proAmso *o)
{
	AMSO amso;
	amso.AMSOID = o->id;
	amso.subCompanyID = o->parent->id;
	amso.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	amso.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
	o->id = m_db2.ModifyAMSO(amso, amso.AMSOID);
    return true;
}

bool DatabaseProxy::modifyRoute(proRoute *o)
{
	ROUTE route;
	route.routeID = o->id;
	route.AMSOID = o->parent->id;
	route.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	route.strDescribe = string((const char *)o->desc.toLocal8Bit());//o->desc.toStdString();
	o->id = m_db2.ModifyRoute(route, route.routeID);
    return true;
}

bool DatabaseProxy::modifyConcentrator(proConcentrator *o)
{
	CONCENTRATOR concentrator;
	concentrator.ConcentratorID = o->id;
	concentrator.routeID = o->parent->id;
	concentrator.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	concentrator.strDestIP = string((const char *)o->destIp.toLocal8Bit());//o->destIp.toStdString();
	concentrator.strDestPort = string((const char *)o->destPort.toLocal8Bit());//o->destPort.toStdString();
	concentrator.strConnectType = string((const char *)o->type.toLocal8Bit());//o->type.toStdString();
	concentrator.strInstallPlace = string((const char *)o->installAddr.toLocal8Bit());//o->installAddr.toStdString();
	concentrator.strAPName = string((const char *)o->apName.toLocal8Bit());//o->apName.toStdString();
	concentrator.strAPProtocol = string((const char *)o->apProtocol.toLocal8Bit());//o->apProtocol.toStdString();
	concentrator.TerminalTimer = o->terminalTimer;
	concentrator.HeartTimer = o->heartTimer;
	concentrator.strSimCard = string((const char *)o->strSimCard.toLocal8Bit());//o->strSimCard.toStdString();
	concentrator.GPRSReConnectTimer = o->gprsReConnectTimer;
	concentrator.GPRSSignalStrength = o->gprsSignalStrength;
	concentrator.SaveTimer = o->saveTimer;
	concentrator.wirelessSearchTimer = o->wirelessSearchTimer;
	concentrator.ConcentratorAddr = o->concentratorAddr;
	concentrator.ConcentratorCurrentTime = o->ConcentratorCurrentTime;
	concentrator.SelfReportOnOff = o->SelfReportOnOff;
	concentrator.ConcentratorTimer = o->concentratorTimer;
	o->id = m_db2.ModifyConcentrator(concentrator, concentrator.ConcentratorID);
    return true;
}

bool DatabaseProxy::modifyLine(proLine *o)
{
	LINE line;
	line.lineID = o->id;
	line.ConcentratorID = o->parent->id;
	line.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	line.strType = o->type;
	line.strAddr = string((const char *)o->addr.toLocal8Bit());//o->addr.toStdString();
	line.strPreAddr = string((const char *)o->preAddr.toLocal8Bit());//o->preAddr.toStdString();
	line.strNextAddr = string((const char *)o->nextAddr.toLocal8Bit());//o->nextAddr.toStdString();
	line.workerID = o->workerID;
	o->id = m_db2.ModifyLine(line, line.lineID);
    return true;
}

bool DatabaseProxy::modifyMonitor(proMonitor *o)
{
	MONITOR monitor;
	monitor.MonitorID = o->id;
	monitor.lineID = o->parent->id;
	monitor.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	monitor.MonitorAddr = o->addr;
	o->id = m_db2.ModifyMonitor(monitor,monitor.MonitorID);
    return true;
}

bool DatabaseProxy::modifyTerminal(proTerminal *o)
{
	TERMINAL terminal;
	terminal.TerminalID = o->id;
	terminal.MonitorID = o->parent->id;
	terminal.strName = string((const char *)o->name.toLocal8Bit());//o->name.toStdString();
	terminal.strType = string((const char *)o->type.toLocal8Bit());//o->type.toStdString();
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
	o->id = m_db2.ModifyTerminal(terminal, terminal.TerminalID);
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

//获得实时数据 注意这里传入的是集中器地址
bool DatabaseProxy::realTimeDataByAddr(showData &pShowData, int ConcentratorAddr)
{
	GetShowDataInfoByConcentratorAddr(pShowData, ConcentratorAddr);
	proConcentrator *pConcentrator =  concentratorAddr(ConcentratorAddr);
	if (pConcentrator ==  NULL)
	{
		return false;
	}
	for (int i = 0; i < pConcentrator->lst.size(); i++)
	{
		proLine *pLine = pConcentrator->lst.at(i);
		pShowData.line = pLine->name;
		for (int i = 0; i < pLine->lst.size(); i++)
		{
			proMonitor *pMonitor = pLine->lst.at(i);
			pShowData.monitor = pMonitor->name;

			if (pMonitor->lst.size() == 1)
			{
				proTerminal *pTerminal = pMonitor->lst.at(0);
				m_db2.GetRealDatabyTerminalAddr(pShowData.valueA, pTerminal->addr);

			}
			else if (pMonitor->lst.size() == 2)
			{
				proTerminal *pTerminal1 = pMonitor->lst.at(0);
				proTerminal *pTerminal2 = pMonitor->lst.at(1);
				m_db2.GetRealDatabyTerminalAddr(pShowData.valueA, pTerminal1->addr);
				m_db2.GetRealDatabyTerminalAddr(pShowData.valueB, pTerminal2->addr);
			}
			else
			{
				proTerminal *pTerminal1 = pMonitor->lst.at(0);
				proTerminal *pTerminal2 = pMonitor->lst.at(1);
				proTerminal *pTerminal3 = pMonitor->lst.at(2);
				m_db2.GetRealDatabyTerminalAddr(pShowData.valueA, pTerminal1->addr);
				m_db2.GetRealDatabyTerminalAddr(pShowData.valueB, pTerminal2->addr);
				m_db2.GetRealDatabyTerminalAddr(pShowData.valueC, pTerminal3->addr);
			}
		}

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
				m_db2.GetDatabyTerminalAddr(terminalData1, ConcentratorAddr, pTerminal->addr);
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
				m_db2.GetDatabyTerminalAddr(terminalData1, ConcentratorAddr, pTerminal1->addr);
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
				m_db2.GetDatabyTerminalAddr(terminalData1, ConcentratorAddr, pTerminal1->addr);
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
				m_db2.GetDatabyTerminalAddrAndDate(terminalData1, ConcentratorAddr, pTerminal->addr, begin, end);
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
				m_db2.GetDatabyTerminalAddrAndDate(terminalData1, ConcentratorAddr, pTerminal1->addr, begin, end);
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
				m_db2.GetDatabyTerminalAddrAndDate(terminalData1, ConcentratorAddr, pTerminal1->addr, begin, end);
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

//获得所有的报警信息，包含已报的和未报的
bool DatabaseProxy::historyWarningAll(QList<proWarning> &pDatalist)
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
		pWarning.WorkerName = QString(QString::fromLocal8Bit(vdata[i].WorkerName.c_str()));//QString::fromStdString(vdata[i].WorkerName);
		pWarning.WarningInfo = QString(QString::fromLocal8Bit(vdata[i].WarningInfo.c_str()));//QString::fromStdString(vdata[i].WarningInfo);
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
		pWarning.WarningTime = vdata[i].WarningTime;
		pWarning.WarningLine = vdata[i].WarningLine;
		pWarning.MonitorAddr1 = vdata[i].MonitorAddr1;
		pWarning.MonitorAddr2 = vdata[i].MonitorAddr2;
		pWarning.Type = vdata[i].Type;
		pWarning.iValue1 = vdata[i].iValue1;
		pWarning.iValue2 = vdata[i].iValue2;
		pWarning.WorkerName = QString(QString::fromLocal8Bit(vdata[i].WorkerName.c_str()));//QString::fromStdString(vdata[i].WorkerName);
		pWarning.WarningInfo = QString(QString::fromLocal8Bit(vdata[i].WarningInfo.c_str()));//QString::fromStdString(vdata[i].WarningInfo);
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
		pWarning.WarningTime = vdata[i].WarningTime;
		pWarning.WarningLine = vdata[i].WarningLine;
		pWarning.MonitorAddr1 = vdata[i].MonitorAddr1;
		pWarning.MonitorAddr2 = vdata[i].MonitorAddr2;
		pWarning.Type = vdata[i].Type;
		pWarning.iValue1 = vdata[i].iValue1;
		pWarning.iValue2 = vdata[i].iValue2;
		pWarning.WorkerName = QString(QString::fromLocal8Bit(vdata[i].WorkerName.c_str()));//QString::fromStdString(vdata[i].WorkerName);
		pWarning.WarningInfo = QString(QString::fromLocal8Bit(vdata[i].WarningInfo.c_str()));//QString::fromStdString(vdata[i].WarningInfo);
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
