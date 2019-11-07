//#include "stdafx.h"
#include "DATAOperate.h"

CDATAOperate::CDATAOperate(void):m_conn(NULL)
{
}

CDATAOperate::~CDATAOperate(void)
{
}


BOOL CDATAOperate::DB2Connect(string strIP, string strUsr, string strCode)
{
	try
	{
		string strConnectionString = "Provider=IBMDADB2.DB2COPY1;Persist Security Info=False;Data Source=AUTOTEST;Location=";
		strConnectionString += strIP;
		string strName = strUsr;
		string strPwd = strCode;
		m_conn.open( strConnectionString.c_str(), strName.c_str(), strPwd.c_str() );	
	}
	catch (_com_error& e)
	{
		///
		char pLog[2048] = {0};
		sprintf(pLog, "数据库连接失败！用户名：%s，密码：%s，地址：%s \n", strUsr, strCode, strIP);
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}

int CDATAOperate::InsertCompany(COMPANY p)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select CompanyID from final table (insert into BR_COMPANY(CName,SVersion) values (?, ?) )";
		st.prepare(sql);
		st.set_string( 0,p.strName );
		st.set_string( 1, p.strDescribe );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int Company_ID;
		Company_ID = rs.get_long( 0 );
		rs.close();
		return Company_ID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入公司失败！%s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertSubCompany(SUBCOMPANY p, int CompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select SubCompanyID from final table (insert into BR_SUBCOMPANY(CompanyID,SubNAME,SVersion) values (?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, CompanyID);
		st.set_string( 1, p.strName );
		st.set_string( 2, p.strDescribe );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入子公司失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertAMSO(AMSO p, int SubCompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select AMSOID from final table (insert into BR_AMSO(SubCompanyID,AMSONAME,SVersion) values (?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, SubCompanyID);
		st.set_string( 1, p.strName );
		st.set_string( 2, p.strDescribe );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入供电所失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertRoute(ROUTE p, int AMSOID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select RouteID from final table (insert into BR_ROUTE(AMSOID,RouteNAME,SVersion) values (?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, AMSOID);
		st.set_string( 1, p.strName );
		st.set_string( 2, p.strDescribe );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入线路失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertConcentrator(CONCENTRATOR p, int RouteID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select ConcentratorID from final table (insert into BR_CONCENTRATOR(RouteID,ConcentratorNAME,DestAddr,DestPort,ConnectType,InstallPlace,APName,APProtocol,GetTerminalTimer,GetConcentratorTimer,HeartTimer,SimCardNumber,GPRSReConnectTimer,GPRSSignalStrength,SaveTimer,wirelessSearchTimer,ConcentratorAddr,ConcentratorCurrentTime,SelfReportOnOff) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, RouteID);
		st.set_string( 1, p.strName );
		st.set_string( 2, p.strDestIP );
		st.set_string( 3, p.strDestPort );
		st.set_string( 4, p.strConnectType );
		st.set_string( 5, p.strInstallPlace );
		st.set_string( 6, p.strAPName );
		st.set_string( 7, p.strAPProtocol );
		st.set_long( 8, p.TerminalTimer);
		st.set_long( 9, p.ConcentratorTimer);
		st.set_long( 10, p.HeartTimer);
		st.set_string( 11, p.strSimCard );
		st.set_long( 12, p.GPRSReConnectTimer);
		st.set_long( 13, p.GPRSSignalStrength);
		st.set_long( 14, p.SaveTimer);
		st.set_long( 15, p.wirelessSearchTimer);
		st.set_long( 16, p.ConcentratorAddr);
		st.set_bigInt( 17, p.ConcentratorCurrentTime);
		st.set_long( 18, p.SelfReportOnOff);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入集中器失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertLine(LINE p, int ConcentratorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select LineID from final table (insert into BR_LINE(ConcentratorID,LineNAME,LineType,LineAddr,PreAddr,NextAddr,WorkerID,Ratio) values (?, ?, ?, ?, ?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, ConcentratorID);
		st.set_string( 1, p.strName );
        st.set_long( 2, p.strType );
		st.set_string( 3, p.strAddr );
		st.set_string( 4, p.strPreAddr );
		st.set_string( 5, p.strNextAddr );
		st.set_long(6, p.workerID);
		st.set_float(7, p.Ratio);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入线段失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertMonitor(MONITOR p, int LineID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select MonitorID from final table (insert into BR_MONITOR(LineID,MonitorNAME,MonitorAddr,PreMonitorID) values (?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, LineID);
		st.set_string( 1, p.strName );
		st.set_string(2, p.MonitorAddr);
		st.set_long(3, p.PreMonitorID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入监测点失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::InsertTerminal(TERMINAL p, int MonitorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select TerminalID from final table (insert into BR_TERMINAL(MonitorID,TerminalNAME,TerminalTYPE,TerminalIndex,TerminalInstallTime,TerminalAddr,TerminalPreAddr,TerminalNextAddr,ConcentratorAddr,TerminalCurrentTime,RouteState1,RouteState2,RouteState3,RouteState4,RouteState5,RouteState6,HighValue,HighOffset,HighSymbol,LowValue) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, MonitorID);
		st.set_string( 1, p.strName );
		st.set_string( 2, p.strType );
		st.set_long(3, p.index);
		st.set_bigInt(4, p.installTime);
		st.set_long(5, p.addr);
		st.set_long(6, p.preAddr);
		st.set_long(7, p.nextAddr);
		st.set_long(8, p.ConcentratorAddr);
		st.set_bigInt(9, p.TerminalCurrentTime);
		st.set_long(10, p.RouteState1);
		st.set_long(11, p.RouteState2);
		st.set_long(12, p.RouteState3);
		st.set_long(13, p.RouteState4);
		st.set_long(14, p.RouteState5);
		st.set_long(15, p.RouteState6);
		st.set_long(16, p.HighValue);
		st.set_float(17, p.HighOffset);
		st.set_long(18, p.HighSymbol);
		st.set_long(19, p.LowValue);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入终端失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::InsertData(DATA p)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select DataID from final table (insert into BR_DATA(TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue,intRev1) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, p.TerminalAddr);
		st.set_long( 1, p.ConcentratorAddr );
		st.set_bigInt(2, p.CollectTime);
		st.set_long( 3, p.relaycnt );
		st.set_long( 4, p.relayPosition );
		st.set_long( 5, p.GetherUnitAddr );
		st.set_float( 6, p.vValue );
		st.set_float( 7, p.vAngValue );
		st.set_float( 8, p.iValue );
		st.set_float( 9, p.iAngValue );
		st.set_float( 10, p.intRev1 );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入数据失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::InsertRealData(DATA p)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select DataID from final table (insert into BR_DATA_REAL(TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_long(0, p.TerminalAddr);
		st.set_long( 1, p.ConcentratorAddr );
		st.set_bigInt(2, p.CollectTime);
		st.set_long( 3, p.relaycnt );
		st.set_long( 4, p.relayPosition );
		st.set_long( 5, p.GetherUnitAddr );
		st.set_float( 6, p.vValue );
		st.set_float( 7, p.vAngValue );
		st.set_float( 8, p.iValue );
		st.set_float( 9, p.iAngValue );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入实时数据失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//插入指令
int CDATAOperate::InsertCommand(COMMAND p)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "insert into BR_COMMAND(UserID,ConcentratorAddr,TerminalAddr,Commandtype) values (?, ?, ?, ?)";
		st.prepare(sql);
		st.set_long( 0, p.UserID);
		st.set_long(1, p.ConcentratorAddr);
		st.set_long(2, p.TerminalAddr);
		st.set_long(3, p.Commandtype);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入指令失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::InsertWarning(WARNING p)
{
	try
	{
		//SYSTEMTIME sysTime;
		//GetLocalTime(&sysTime);

		//FILETIME ft={0}; 
		//SystemTimeToFileTime(&sysTime, &ft); 

		//ULONGLONG collectTime;
		//// Copy the time into a quadword.
		//collectTime = (((ULONGLONG) ft.dwHighDateTime) << 32) + ft.dwLowDateTime;


		PreparedStatement st(m_conn);
        const char *sql = "select WarningID from final table (insert into BR_WARNING(WarningTime,WarningLine,MonitorAddr1,MonitorAddr2,Type,iValue1,iValue2,WorkerName,WarningInfo,SendTime,SendState,Popuped) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_bigInt(0, p.WarningTime);
		st.set_long( 1, p.WarningLine );
		st.set_string(2, p.MonitorAddr1);
		st.set_string( 3, p.MonitorAddr2 );
		st.set_long( 4, p.Type );
		st.set_float( 5, p.iValue1 );
		st.set_float( 6, p.iValue2 );
		st.set_string( 7, p.WorkerName );
		st.set_string( 8, p.WarningInfo );
		st.set_bigInt( 9, p.SendTime );
		st.set_long( 10, p.SendState );
		st.set_long(11, p.Popuped);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}

		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入警告信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::InsertUser(string usr, string code, int level)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select UserID from final table (insert into BR_USER(UserName,UserCode,UserPri) values (?, ?, ?) )";
		st.prepare(sql);
		st.set_string(0, usr);
		st.set_string( 1, code);
		st.set_long(2, level);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入用户失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::InsertWorker(WORKER p)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select WorkerID from final table (insert into BR_WORKER(WorkerName,AmsoID,WorkerPosition,WorkerTEL) values (?, ?, ?, ?) )";
		st.prepare(sql);
		st.set_string(0, p.name);
		st.set_long( 1, p.amsoId);
		st.set_string(2, p.remarks);
		st.set_string(3, p.phone);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		int newID;
		newID = rs.get_long( 0 );
		rs.close();
		return newID;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "插入接警员失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}


BOOL CDATAOperate::DelCompany(int companyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_COMPANY where CompanyID = ?";
		st.prepare(sql);
		st.set_long( 0, companyID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除公司失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelSubCompany(int subCompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_SUBCOMPANY where SubCompanyID = ?";
		st.prepare(sql);
		st.set_long( 0, subCompanyID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除子公司失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelAMSO(int AMSOID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_AMSO where AmsoID = ?";
		st.prepare(sql);
		st.set_long( 0, AMSOID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除供电所失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelRoute(int RouteID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_ROUTE where RouteID = ?";
		st.prepare(sql);
		st.set_long( 0, RouteID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除线路失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelConcentrator(int ConcentratorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_CONCENTRATOR where ConcentratorID = ?";
		st.prepare(sql);
		st.set_long( 0, ConcentratorID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除集中器失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelLine(int lineID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_LINE where LineID = ?";
		st.prepare(sql);
		st.set_long( 0, lineID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除线段失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelMonitor(int MonitorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_MONITOR where MonitorID = ?";
		st.prepare(sql);
		st.set_long( 0, MonitorID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除监测点失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelTerminal(int TerminalID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_TERMINAL where TerminalID = ?";
		st.prepare(sql);
		st.set_long( 0, TerminalID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除终端失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}
BOOL CDATAOperate::DelData(int TerminalID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "delete from BR_DATA where TerminalID = ?";
		st.prepare(sql);
		st.set_long( 0, TerminalID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除数据失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}

BOOL CDATAOperate::DelUser(int userID)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "delete from BR_USER where UserID = ?";
		st.prepare(sql);
		st.set_long( 0, userID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除用户失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}

BOOL CDATAOperate::DelWorker(int WorkerID)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "delete from BR_WORKER where WorkerID = ?";
		st.prepare(sql);
		st.set_long( 0, WorkerID );
		st.execute();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "删除接警员失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return FALSE;
	}
	return TRUE;
}

int CDATAOperate::ModifyCompany(COMPANY p, int CompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_COMPANY set CName=?,SVersion=? where CompanyID=?";
		st.prepare(sql);
		st.set_string( 0,p.strName );
		st.set_string( 1, p.strDescribe );
		st.set_long(2, CompanyID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改公司失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifySubCompany(SUBCOMPANY p, int SubCompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_SUBCOMPANY set SubNAME=?,SVersion=? where SubCompanyID=?";
		st.prepare(sql);
		st.set_string( 0,p.strName );
		st.set_string( 1, p.strDescribe );
		st.set_long(2, SubCompanyID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改子公司失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyAMSO(AMSO p, int AMSOID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_AMSO set AmsoNAME=?,SVersion=? where AmsoID=?";
		st.prepare(sql);
		st.set_string( 0,p.strName );
		st.set_string( 1, p.strDescribe );
		st.set_long(2, AMSOID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改供电所失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyRoute(ROUTE p, int RouteID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_ROUTE set RouteNAME=?,SVersion=? where RouteID=?";
		st.prepare(sql);
		st.set_string( 0,p.strName );
		st.set_string( 1, p.strDescribe );
		st.set_long(2, RouteID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改线路失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyConcentrator(CONCENTRATOR p, int ConcentratorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_CONCENTRATOR set ConcentratorNAME=?,DestAddr=?,DestPort=?,ConnectType=?,InstallPlace=?,APName=?,APProtocol=?,GetTerminalTimer=?,GetConcentratorTimer=?,HeartTimer=?,SimCardNumber=?,GPRSReConnectTimer=?,GPRSSignalStrength=?,SaveTimer=?,wirelessSearchTimer=?,ConcentratorAddr=?,ConcentratorCurrentTime=?,SelfReportOnOff=? where ConcentratorID=?";
		st.prepare(sql);
		st.set_string( 0, p.strName );
		st.set_string( 1, p.strDestIP );
		st.set_string( 2, p.strDestPort );
		st.set_string( 3, p.strConnectType );
		st.set_string( 4, p.strInstallPlace );
		st.set_string( 5, p.strAPName );
		st.set_string( 6, p.strAPProtocol );
		st.set_long( 7, p.TerminalTimer);
		st.set_long( 8, p.ConcentratorTimer);
		st.set_long( 9, p.HeartTimer);
		st.set_string( 10, p.strSimCard );
		st.set_long(11, p.GPRSReConnectTimer);
		st.set_long(12, p.GPRSSignalStrength);
		st.set_long(13, p.SaveTimer);
		st.set_long(14, p.wirelessSearchTimer);
		st.set_long(15, p.ConcentratorAddr);
		st.set_bigInt(16, p.ConcentratorCurrentTime);
		st.set_long(17, p.SelfReportOnOff);
		st.set_long(18, ConcentratorID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改集中器失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyConcentratorReserve(CONCENTRATOR p, int ConcentratorID)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "update BR_CONCENTRATOR set intRev1=?,intRev2=?,intRev3=?,intRev4=? where ConcentratorID=?";
		st.prepare(sql);
		st.set_long( 0, p.reserve1);
		st.set_long( 1, p.reserve2 );
		st.set_long( 2, p.reserve3 );
		st.set_long( 3, p.reserve4 );
		st.set_long(4, ConcentratorID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改集中器保留字段失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyLine(LINE p, int LineID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_LINE set LineNAME=?,LineType=?,LineAddr=?,PreAddr=?,NextAddr=?,WorkerID=?,Ratio=? where LineID=?";
		st.prepare(sql);
		st.set_string( 0,p.strName );
        st.set_long( 1, p.strType );
        st.set_string( 2, p.strAddr );
        st.set_string( 3, p.strPreAddr );
        st.set_string( 4, p.strNextAddr );
		st.set_long(5, p.workerID);
		st.set_float(6, p.Ratio);
		st.set_long(7, LineID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改线段失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyMonitor(MONITOR p, int MonitorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_MONITOR set MonitorNAME=?,MonitorAddr=?,PreMonitorID=? where MonitorID=?";
		st.prepare(sql);
		st.set_string( 0,p.strName );
		st.set_string(1, p.MonitorAddr);
		st.set_long(2, p.PreMonitorID);
		st.set_long(3, MonitorID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改监测点失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::ModifyTerminal(TERMINAL p, int TerminalID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "update BR_TERMINAL set TerminalNAME=?,TerminalTYPE=?,TerminalIndex=?,TerminalInstallTime=?,TerminalAddr=?,TerminalPreAddr=?,TerminalNextAddr=?,ConcentratorAddr=?,TerminalCurrentTime=?,RouteState1=?,RouteState2=?,RouteState3=?,RouteState4=?,RouteState5=?,RouteState6=?,HighValue=?,HighOffset=?,HighSymbol=?,LowValue=? where TerminalID=?";
		st.prepare(sql);
		st.set_string( 0, p.strName );
		st.set_string( 1, p.strType );
		st.set_long(2, p.index);
		st.set_bigInt(3, p.installTime);
		st.set_long(4, p.addr);
		st.set_long(5, p.preAddr);
		st.set_long(6, p.nextAddr);
		st.set_long(7, p.ConcentratorAddr);
		st.set_bigInt(8, p.TerminalCurrentTime);
		st.set_long(9, p.RouteState1);
		st.set_long(10, p.RouteState2);
		st.set_long(11, p.RouteState3);
		st.set_long(12, p.RouteState4);
		st.set_long(13, p.RouteState5);
		st.set_long(14, p.RouteState6);
		st.set_long(15, p.HighValue);
		st.set_float(16, p.HighOffset);
		st.set_long(17, p.HighSymbol);
		st.set_long(18, p.LowValue);
		st.set_long(19, TerminalID);
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改终端失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::ModifyUser(int id, string usr, string code, int level)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "update BR_USER set UserName=?,UserCode=?,UserPri=? where UserID=?";
		st.prepare(sql);
		st.set_string( 0, usr );
		st.set_string( 1, code );
		st.set_long( 2, level );
		st.set_long( 3, id );
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改用户失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::ModifyWorker(WORKER p, int WorkerID)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "update BR_WORKER set WorkerName=?,AmsoID=?,WorkerPosition=?,WorkerTEL=? where WorkerID=?";
		st.prepare(sql);
		st.set_string( 0, p.name );
		st.set_long( 1, p.amsoId );
		st.set_string( 2, p.remarks );
		st.set_string( 3, p.phone );
		st.set_long( 4, WorkerID );
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改接警员失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::ModifyWarningPop(int id)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "update BR_WARNING set Popuped=? where WarningID=?";
		st.prepare(sql);
		st.set_long( 0, 1 );
		st.set_long( 1, id );
		st.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "修改报警信息Pop失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}


int CDATAOperate::GetCompany(COMPANY &p, int CompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select CName,SVersion from BR_COMPANY where CompanyID = ?";
		st.prepare(sql);
		st.set_long( 0, CompanyID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.strName = rs.get_string(0);
		p.strDescribe = rs.get_string(1);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得公司信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetSubCompany(SUBCOMPANY &p, int SubCompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select CompanyID,SubNAME,SVersion from BR_SUBCOMPANY where SubCompanyID = ?";
		st.prepare(sql);
		st.set_long( 0, SubCompanyID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.companyID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.strDescribe = rs.get_string(2);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得子公司信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetAMSO(AMSO &p, int AMSOID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select SubCompanyID,AmsoNAME,SVersion from BR_AMSO where AmsoID = ?";
		st.prepare(sql);
		st.set_long( 0, AMSOID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.subCompanyID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.strDescribe = rs.get_string(2);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得供电所信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetRoute(ROUTE &p, int RouteID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select AmsoID,RouteNAME,SVersion from BR_ROUTE where RouteID = ?";
		st.prepare(sql);
		st.set_long( 0, RouteID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.AMSOID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.strDescribe = rs.get_string(2);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得线路信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetConcentrator(CONCENTRATOR &p, int ConcentratorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select RouteID,ConcentratorNAME,DestAddr,DestPort,ConnectType,InstallPlace,APName,APProtocol,GetTerminalTimer,GetConcentratorTimer,HeartTimer,SimCardNumber,GPRSReConnectTimer,GPRSSignalStrength,SaveTimer,wirelessSearchTimer,ConcentratorAddr,ConcentratorCurrentTime,SelfReportOnOff,intRev1,intRev2,intRev3,intRev4 from BR_CONCENTRATOR where ConcentratorID = ?";
		st.prepare(sql);
		st.set_long( 0, ConcentratorID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.routeID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.strDestIP = rs.get_string(2);
		p.strDestPort = rs.get_string(3);
		p.strConnectType = rs.get_string(4);
		p.strInstallPlace = rs.get_string(5);
		p.strAPName = rs.get_string(6);
		p.strAPProtocol = rs.get_string(7);
		p.TerminalTimer = rs.get_long(8);
		p.ConcentratorTimer = rs.get_long(9);
		p.HeartTimer = rs.get_long(10);
		p.strSimCard = rs.get_string(11);
		p.GPRSReConnectTimer = rs.get_long(12);
		p.GPRSSignalStrength = rs.get_long(13);
		p.SaveTimer = rs.get_long(14);
		p.wirelessSearchTimer = rs.get_long(15);
		p.ConcentratorAddr = rs.get_long(16);
		p.ConcentratorCurrentTime = rs.get_bigInt(17);
		p.SelfReportOnOff = rs.get_long(18);
		p.reserve1 = rs.get_long(19);
		p.reserve2 = rs.get_long(20);
		p.reserve3 = rs.get_long(21);
		p.reserve4 = rs.get_long(22);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得集中器信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetLine(LINE &p, int LineID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select ConcentratorID,LineNAME,LineType,LineAddr,PreAddr,NextAddr,WorkerID,Ratio from BR_LINE where LineID = ?";
		st.prepare(sql);
		st.set_long( 0, LineID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.ConcentratorID = rs.get_long(0);
		p.strName = rs.get_string(1);
        p.strType = rs.get_long(2);
		p.strAddr = rs.get_string(3);
		p.strPreAddr = rs.get_string(4);
		p.strNextAddr = rs.get_string(5);
		p.workerID = rs.get_long(6);
		p.Ratio = rs.get_float(7);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得线段信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetMonitor(MONITOR &p, int MonitorID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select LineID,MonitorNAME,MonitorAddr,PreMonitorID from BR_MONITOR where MonitorID = ?";
		st.prepare(sql);
		st.set_long( 0, MonitorID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.lineID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.MonitorAddr = rs.get_string(2);
		p.PreMonitorID = rs.get_long(3);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "获得监测点信息失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetTerminalByAddr(TERMINAL &p, int ConcentratorAddr, int TerminalAddr)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select MonitorID,TerminalNAME,TerminalTYPE,TerminalIndex,TerminalInstallTime,TerminalAddr,TerminalPreAddr,TerminalNextAddr,ConcentratorAddr,TerminalCurrentTime,RouteState1,RouteState2,RouteState3,RouteState4,RouteState5,RouteState6,HighValue,HighOffset,HighSymbol,LowValue from BR_TERMINAL where ConcentratorAddr = ? and TerminalAddr = ?";
		st.prepare(sql);
		st.set_long( 0, ConcentratorAddr );
		st.set_long( 1, TerminalAddr );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.MonitorID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.strType = rs.get_string(2);
		p.index = rs.get_long(3);
		p.installTime = rs.get_bigInt(4);
		p.addr = rs.get_long(5);
		p.preAddr = rs.get_long(6);
		p.nextAddr = rs.get_long(7);
		p.ConcentratorAddr = rs.get_long(8);
		p.TerminalCurrentTime = rs.get_bigInt(9);
		p.RouteState1 = rs.get_long(10);
		p.RouteState2 = rs.get_long(11);
		p.RouteState3 = rs.get_long(12);
		p.RouteState4 = rs.get_long(13);
		p.RouteState5 = rs.get_long(14);
		p.RouteState6 = rs.get_long(15);
		p.HighValue = rs.get_long(16);
		p.HighOffset = rs.get_float(17);
		p.HighSymbol = rs.get_long(18);
		p.LowValue = rs.get_long(19);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetTerminalByAddr 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetTerminal(TERMINAL &p, int TerminalID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select MonitorID,TerminalNAME,TerminalTYPE,TerminalIndex,TerminalInstallTime,TerminalAddr,TerminalPreAddr,TerminalNextAddr,ConcentratorAddr,TerminalCurrentTime,RouteState1,RouteState2,RouteState3,RouteState4,RouteState5,RouteState6,HighValue,HighOffset,HighSymbol,LowValue from BR_TERMINAL where TerminalID = ?";
		st.prepare(sql);
		st.set_long( 0, TerminalID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.MonitorID = rs.get_long(0);
		p.strName = rs.get_string(1);
		p.strType = rs.get_string(2);
		p.index = rs.get_long(3);
		p.installTime = rs.get_bigInt(4);
		p.addr = rs.get_long(5);
		p.preAddr = rs.get_long(6);
		p.nextAddr = rs.get_long(7);
		p.ConcentratorAddr = rs.get_long(8);
		p.TerminalCurrentTime = rs.get_bigInt(9);
		p.RouteState1 = rs.get_long(10);
		p.RouteState2 = rs.get_long(11);
		p.RouteState3 = rs.get_long(12);
		p.RouteState4 = rs.get_long(13);
		p.RouteState5 = rs.get_long(14);
		p.RouteState6 = rs.get_long(15);
		p.HighValue = rs.get_long(16);
		p.HighOffset = rs.get_float(17);
		p.HighSymbol = rs.get_long(18);
		p.LowValue = rs.get_long(19);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetTerminal 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetWorker(WORKER &p, int WorkerID)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select WorkerName,AmsoID,WorkerPosition,WorkerTEL from BR_WORKER where WorkerID = ?";
		st.prepare(sql);
		st.set_long( 0, WorkerID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if( rs.db_eof() )
		{
			rs.close();
			return 0;
		}
		p.name = rs.get_string(0);
		p.amsoId = rs.get_long(1);
		p.remarks = rs.get_string(2);
		p.phone = rs.get_string(3);
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetWorker 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllCompanyID(vector<COMPANY> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select CompanyID,CName,SVersion from BR_COMPANY";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			COMPANY p;
			p.companyID = rs.get_long(0);
			p.strName = rs.get_string(1);
			p.strDescribe = rs.get_string(2);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllCompanyID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetAllSubCompanyID(vector<SUBCOMPANY> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select SubCompanyID,CompanyID,SubNAME,SVersion from BR_SUBCOMPANY";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			SUBCOMPANY p;
			p.subCompanyID = rs.get_long(0);
			p.companyID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllSubCompanyID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetAllAMSOID(vector<AMSO> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select AmsoID,SubCompanyID,AmsoNAME,SVersion from BR_AMSO";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			AMSO p;
			p.AMSOID = rs.get_long(0);
			p.subCompanyID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllAMSOID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetAllRouteID(vector<ROUTE> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select RouteID,AmsoID,RouteNAME,SVersion from BR_ROUTE";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			ROUTE p;
			p.routeID = rs.get_long(0);
			p.AMSOID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllRouteID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetAllConcentratorID(vector<CONCENTRATOR> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select ConcentratorID,RouteID,ConcentratorNAME,DestAddr,DestPort,ConnectType,InstallPlace,APName,APProtocol,GetTerminalTimer,GetConcentratorTimer,HeartTimer,SimCardNumber,GPRSReConnectTimer,GPRSSignalStrength,SaveTimer,wirelessSearchTimer,ConcentratorAddr,ConcentratorCurrentTime,SelfReportOnOff from BR_CONCENTRATOR";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			CONCENTRATOR p;
			p.ConcentratorID = rs.get_long(0);
			p.routeID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDestIP = rs.get_string(3);
			p.strDestPort = rs.get_string(4);
			p.strConnectType = rs.get_string(5);
			p.strInstallPlace = rs.get_string(6);
			p.strAPName = rs.get_string(7);
			p.strAPProtocol = rs.get_string(8);
			p.TerminalTimer = rs.get_long(9);
			p.ConcentratorTimer = rs.get_long(10);
			p.HeartTimer = rs.get_long(11);
			p.strSimCard = rs.get_string(12);
			p.GPRSReConnectTimer = rs.get_long(13);
			p.GPRSSignalStrength = rs.get_long(14);
			p.SaveTimer = rs.get_long(15);
			p.wirelessSearchTimer = rs.get_long(16);
			p.ConcentratorAddr = rs.get_long(17);
			p.ConcentratorCurrentTime = rs.get_bigInt(18);
			p.SelfReportOnOff = rs.get_long(19);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllConcentratorID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
int CDATAOperate::GetAllLineID(vector<LINE> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select LineID,ConcentratorID,LineNAME,LineType,LineAddr,PreAddr,NextAddr,WorkerID,Ratio from BR_LINE";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			LINE p;
			p.lineID = rs.get_long(0);
			p.ConcentratorID = rs.get_long(1);
			p.strName = rs.get_string(2);
            p.strType = rs.get_long(3);
			p.strAddr = rs.get_string(4);
			p.strPreAddr = rs.get_string(5);
			p.strNextAddr = rs.get_string(6);
			p.workerID = rs.get_long(7);
			p.Ratio = rs.get_float(8);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllLineID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllTerminalByConcentratorID(vector<TERMINAL> &v, int ConcentratorAddr)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select TerminalID,MonitorID,TerminalNAME,TerminalTYPE,TerminalIndex,TerminalInstallTime,TerminalAddr,TerminalPreAddr,TerminalNextAddr,ConcentratorAddr,TerminalCurrentTime,RouteState1,RouteState2,RouteState3,RouteState4,RouteState5,RouteState6,HighValue,HighOffset,HighSymbol,LowValue from BR_TERMINAL where ConcentratorAddr = ?";
		st.prepare(sql);
		st.set_long(0, ConcentratorAddr);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			TERMINAL p;
			p.TerminalID = rs.get_long(0);
			p.MonitorID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strType = rs.get_string(3);
			p.index = rs.get_long(4);
			p.installTime = rs.get_bigInt(5);
			p.addr = rs.get_long(6);
			p.preAddr = rs.get_long(7);
			p.nextAddr = rs.get_long(8);
			p.ConcentratorAddr = rs.get_long(9);
			p.TerminalCurrentTime = rs.get_bigInt(10);
			p.RouteState1 = rs.get_long(11);
			p.RouteState2 = rs.get_long(12);
			p.RouteState3 = rs.get_long(13);
			p.RouteState4 = rs.get_long(14);
			p.RouteState5 = rs.get_long(15);
			p.RouteState6 = rs.get_long(16);
			p.HighValue = rs.get_long(17);
			p.HighOffset = rs.get_float(18);
			p.HighSymbol = rs.get_long(19);
			p.LowValue = rs.get_long(20);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllTerminalByConcentratorID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllTerminalID(vector<TERMINAL> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select TerminalID,MonitorID,TerminalNAME,TerminalTYPE,TerminalIndex,TerminalInstallTime,TerminalAddr,TerminalPreAddr,TerminalNextAddr,ConcentratorAddr,TerminalCurrentTime,RouteState1,RouteState2,RouteState3,RouteState4,RouteState5,RouteState6,HighValue,HighOffset,HighSymbol,LowValue from BR_TERMINAL";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			TERMINAL p;
			p.TerminalID = rs.get_long(0);
			p.MonitorID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strType = rs.get_string(3);
			p.index = rs.get_long(4);
			p.installTime = rs.get_bigInt(5);
			p.addr = rs.get_long(6);
			p.preAddr = rs.get_long(7);
			p.nextAddr = rs.get_long(8);
			p.ConcentratorAddr = rs.get_long(9);
			p.TerminalCurrentTime = rs.get_bigInt(10);
			p.RouteState1 = rs.get_long(11);
			p.RouteState2 = rs.get_long(12);
			p.RouteState3 = rs.get_long(13);
			p.RouteState4 = rs.get_long(14);
			p.RouteState5 = rs.get_long(15);
			p.RouteState6 = rs.get_long(16);
			p.HighValue = rs.get_long(17);
			p.HighOffset = rs.get_float(18);
			p.HighSymbol = rs.get_long(19);
			p.LowValue = rs.get_long(20);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllTerminalID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllWorker(vector<WORKER> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select WorkerID,WorkerName,AmsoID,WorkerPosition,WorkerTEL from BR_WORKER";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			WORKER p;
			p.id = rs.get_long(0);
			p.name = rs.get_string(1);
			p.amsoId = rs.get_long(2);
			p.remarks = rs.get_string(3);
			p.phone = rs.get_string(4);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllWorker 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetDatabyConcentratorAddr(vector<DATA> &v, int ConAddr)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue from BR_DATA where ConcentratorAddr = ? order by CollectTime";
		st.prepare(sql);
		st.set_long( 0, ConAddr );
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			DATA p;
			p.DataID = rs.get_long(0);
			p.TerminalAddr = rs.get_long(1);
			p.ConcentratorAddr = rs.get_long(2);
			p.CollectTime = rs.get_bigInt(3);
			p.relaycnt = rs.get_long(4);
			p.relayPosition = rs.get_long(5);
			p.GetherUnitAddr = rs.get_long(6);
			p.vValue = rs.get_float(7);
			p.vAngValue = rs.get_float(8);
			p.iValue = rs.get_float(9);
			p.iAngValue = rs.get_float(10);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetDatabyConcentratorAddr 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//获得数据表中的时间，并以集中器分组
int CDATAOperate::GetDataByCollectTimeAndMonitorID(DATA &A, DATA &B, DATA &C, TIME_ID ti)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select TerminalAddr where MonitorID = ?";
		st.prepare(sql);
		st.set_long(0, ti.MonitorID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if (rs.db_eof())
		{
			rs.close();
			return -1;
		}
		int addr[3] = {-1};
		int index = 0;
		while( !rs.db_eof() )
		{
			if (index >= 3)
			{
				break;
			}
			addr[index++] = rs.get_long(0);
			rs.move_next();			
		}
		rs.close();


		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetDataByCollectTimeAndMonitorID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//获得数据表中的时间，并以集中器分组
int CDATAOperate::GetCollectTimeAndMoniterID(vector<TIME_ID> &v, int ConcentratorAddr)
{
	try
	{
		PreparedStatement st(m_conn);
        //const char *sql = "select a.MonitorID, b.CollectTime from br_terminal a, br_data b where a.terminaladdr = b.terminaladdr group by a.MonitorID, b.CollectTime order by b.CollectTime fetch first 50 rows only";
        const char *sql = "select b.MonitorID, a.CollectTime "
                        "from br_data a, br_terminal b, br_monitor c, br_line d, br_concentrator e "
                        "where a.terminaladdr = b.terminaladdr "
                        "and b.monitorid = c.monitorid and c.lineid = d.lineid and d.concentratorid = e.concentratorid and e.concentratoraddr=? "
                        "group by b.MonitorID, a.CollectTime order by a.CollectTime";
        st.prepare(sql);
        st.set_long(0, ConcentratorAddr);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if (rs.db_eof())
		{
			rs.close();
			return -1;
		}

		while( !rs.db_eof() )
		{
			TIME_ID time_id;
			time_id.MonitorID = rs.get_long(0);
			time_id.CollectTime = rs.get_bigInt(1);
			v.push_back(time_id);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetCollectTimeAndMoniterID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
    return 0;
}

int CDATAOperate::GetCollectTimeAndMoniterID(vector<TIME_ID> &v, int ConcentratorAddr, INT64 begin, INT64 end)
{
    try
    {
        PreparedStatement st(m_conn);
        //const char *sql = "select a.MonitorID, b.CollectTime from br_terminal a, br_data b where a.terminaladdr = b.terminaladdr and b.CollectTime > ? and b.CollectTime < ? group by a.MonitorID, b.CollectTime order by b.CollectTime";
        const char *sql = "select b.MonitorID, a.CollectTime "
                        "from br_data a, br_terminal b, br_monitor c, br_line d, br_concentrator e "
                        "where a.terminaladdr = b.terminaladdr "
                        "and b.monitorid = c.monitorid and c.lineid = d.lineid and d.concentratorid = e.concentratorid and e.concentratoraddr=? "
                        "and a.CollectTime > ? and a.CollectTime < ? "
                        "group by b.MonitorID, a.CollectTime order by a.CollectTime";
        st.prepare(sql);
        st.set_long(0, ConcentratorAddr);
        st.set_bigInt(1, begin);
        st.set_bigInt(2, end);
        ADO_WRAPPER::ResultSet rs = st.execute();
        if (rs.db_eof())
        {
            rs.close();
            return -1;
        }

        while( !rs.db_eof() )
        {
            TIME_ID time_id;
            time_id.MonitorID = rs.get_long(0);
            time_id.CollectTime = rs.get_bigInt(1);
            v.push_back(time_id);
            rs.move_next();
        }
        rs.close();
        return 1;
    }
    catch (_com_error& e)
    {
        char pLog[2048] = {0};
        sprintf(pLog, "GetCollectTimeAndMoniterID 失败！ %s\n", (char*)e.Description());
        WriteLog(pLog);
        OutputDebugString(e.Description());
        return 0;
    }
    return 0;
}

//获得实时指令
int CDATAOperate::GetCommand(vector<COMMAND> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select UserID,ConcentratorAddr,TerminalAddr,Commandtype from BR_COMMAND";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if (rs.db_eof())
		{
			rs.close();
			return -1;
		}
		
		while( !rs.db_eof() )
		{
			COMMAND cmd;
			cmd.UserID = rs.get_long(0);
			cmd.ConcentratorAddr = rs.get_long(1);
			cmd.TerminalAddr = rs.get_long(2);
			cmd.Commandtype = rs.get_long(3);
			v.push_back(cmd);
			rs.move_next();
		}
		rs.close();

		PreparedStatement st1(m_conn);
		const char *sql1 = "Delete from BR_COMMAND";
		st1.prepare(sql1);
		st1.execute();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetCommand 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//通过终端地址来寻找当前的实时数据
int CDATAOperate::GetRealData(vector<DATA> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue from BR_DATA_REAL";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		if (rs.db_eof())
		{
			rs.close();
			return -1;
		}
		while( !rs.db_eof() )
		{
			DATA data;
			data.DataID = rs.get_long(0);
			data.TerminalAddr = rs.get_long(1);
			data.ConcentratorAddr = rs.get_long(2);
			data.CollectTime = rs.get_bigInt(3);
			data.relaycnt = rs.get_long(4);
			data.relayPosition = rs.get_long(5);
			data.GetherUnitAddr = rs.get_long(6);
			data.vValue = rs.get_float(7);
			data.vAngValue = rs.get_float(8);
			data.iValue = rs.get_float(9);
			data.iAngValue = rs.get_float(10);
			v.push_back(data);
			rs.move_next();
		}
		rs.close();

		PreparedStatement st1(m_conn);
		const char *sql1 = "Delete from BR_DATA_REAL";
		st1.prepare(sql1);
		st1.execute();

		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetRealData 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//通过集中器和终端地址和时间来寻找当前的数据
int CDATAOperate::GetDatabyAddrAndTime(DATA &data, int TerminalAddr, int ConAddr, INT64 time)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue,intRev1 from BR_DATA where TerminalAddr = ? and ConcentratorAddr = ? and CollectTime = ?";
		st.prepare(sql);
		st.set_long( 0, TerminalAddr );
		st.set_long(1, ConAddr);
		st.set_bigInt( 2, time );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if (rs.db_eof())
		{
			return 0 ;
		}

		while( !rs.db_eof() )
		{
			data.DataID = rs.get_long(0);
			data.TerminalAddr = rs.get_long(1);
			data.ConcentratorAddr = rs.get_long(2);
			data.CollectTime = rs.get_bigInt(3);
			data.relaycnt = rs.get_long(4);
			data.relayPosition = rs.get_long(5);
			data.GetherUnitAddr = rs.get_long(6);
			data.vValue = rs.get_float(7);
			data.vAngValue = rs.get_float(8);
			data.iValue = rs.get_float(9);
			data.iAngValue = rs.get_float(10);
			data.intRev1 = rs.get_long(11);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetDatabyAddrAndTime 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetDatabyTerminalAddrAndTime(DATA &data, int TerminalAddr, INT64 time)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue,intRev1 from BR_DATA where TerminalAddr = ? and CollectTime = ?";
		st.prepare(sql);
		st.set_long( 0, TerminalAddr );
		st.set_bigInt( 1, time );
		ADO_WRAPPER::ResultSet rs = st.execute();
		if (rs.db_eof())
		{
			return 0 ;
		}
		
		while( !rs.db_eof() )
		{
			data.DataID = rs.get_long(0);
			data.TerminalAddr = rs.get_long(1);
			data.ConcentratorAddr = rs.get_long(2);
			data.CollectTime = rs.get_bigInt(3);
			data.relaycnt = rs.get_long(4);
			data.relayPosition = rs.get_long(5);
			data.GetherUnitAddr = rs.get_long(6);
			data.vValue = rs.get_float(7);
			data.vAngValue = rs.get_float(8);
			data.iValue = rs.get_float(9);
			data.iAngValue = rs.get_float(10);
			data.intRev1 = rs.get_long(11);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetDatabyTerminalAddrAndTime 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
    return 0;
}

int CDATAOperate::GetDatabyTerminalAddrAndTime(DATA &dataA, DATA &dataB, DATA &dataC, int TerminalAddrA, int TerminalAddrB, int TerminalAddrC, INT64 time)
{
    try
    {
        PreparedStatement st(m_conn);
        const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue,intRev1 from BR_DATA where ivalue <> 0 and TerminalAddr IN (?, ?, ?) and CollectTime = ?";
        st.prepare(sql);
        st.set_long( 0, TerminalAddrA );
        st.set_long( 1, TerminalAddrB );
        st.set_long( 2, TerminalAddrC );
        st.set_bigInt( 3, time );
        ADO_WRAPPER::ResultSet rs = st.execute();
        if (rs.db_eof())
        {
            return 0 ;
        }

        while( !rs.db_eof() )
        {
            if (rs.get_long(1) == TerminalAddrA) {
                dataA.DataID = rs.get_long(0);
                dataA.TerminalAddr = rs.get_long(1);
                dataA.ConcentratorAddr = rs.get_long(2);
                dataA.CollectTime = rs.get_bigInt(3);
                dataA.relaycnt = rs.get_long(4);
                dataA.relayPosition = rs.get_long(5);
                dataA.GetherUnitAddr = rs.get_long(6);
                dataA.vValue = rs.get_float(7);
                dataA.vAngValue = rs.get_float(8);
                dataA.iValue = rs.get_float(9);
                dataA.iAngValue = rs.get_float(10);
                dataA.intRev1 = rs.get_long(11);
            } else if (rs.get_long(1) == TerminalAddrB) {
                dataB.DataID = rs.get_long(0);
                dataB.TerminalAddr = rs.get_long(1);
                dataB.ConcentratorAddr = rs.get_long(2);
                dataB.CollectTime = rs.get_bigInt(3);
                dataB.relaycnt = rs.get_long(4);
                dataB.relayPosition = rs.get_long(5);
                dataB.GetherUnitAddr = rs.get_long(6);
                dataB.vValue = rs.get_float(7);
                dataB.vAngValue = rs.get_float(8);
                dataB.iValue = rs.get_float(9);
                dataB.iAngValue = rs.get_float(10);
                dataB.intRev1 = rs.get_long(11);
            } else if (rs.get_long(1) == TerminalAddrC) {
                dataC.DataID = rs.get_long(0);
                dataC.TerminalAddr = rs.get_long(1);
                dataC.ConcentratorAddr = rs.get_long(2);
                dataC.CollectTime = rs.get_bigInt(3);
                dataC.relaycnt = rs.get_long(4);
                dataC.relayPosition = rs.get_long(5);
                dataC.GetherUnitAddr = rs.get_long(6);
                dataC.vValue = rs.get_float(7);
                dataC.vAngValue = rs.get_float(8);
                dataC.iValue = rs.get_float(9);
                dataC.iAngValue = rs.get_float(10);
                dataC.intRev1 = rs.get_long(11);
            }

            rs.move_next();
        }
        rs.close();
        return 1;
    }
    catch (_com_error& e)
    {
        char pLog[2048] = {0};
        sprintf(pLog, "GetDatabyTerminalAddrAndTime 3 PARA 失败！ %s\n", (char*)e.Description());
        WriteLog(pLog);
        OutputDebugString(e.Description());
        return 0;
    }
    return 0;
}

int CDATAOperate::GetDatabyTerminalAddrAndDate(vector<DATA> &v, int ConAddr, int TerminalAddr, INT64 begin, INT64 end)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue from BR_DATA where TerminalAddr = ? and ConcentratorAddr = ? and CollectTime > ? and CollectTime < ?";
		st.prepare(sql);
		st.set_long( 0, TerminalAddr );
		st.set_long( 1, ConAddr );
		st.set_bigInt(2, begin);
		st.set_bigInt(3, end);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			DATA p;
			p.DataID = rs.get_long(0);
			p.TerminalAddr = rs.get_long(1);
			p.ConcentratorAddr = rs.get_long(2);
			p.CollectTime = rs.get_bigInt(3);
			p.relaycnt = rs.get_long(4);
			p.relayPosition = rs.get_long(5);
			p.GetherUnitAddr = rs.get_long(6);
			p.vValue = rs.get_float(7);
			p.vAngValue = rs.get_float(8);
			p.iValue = rs.get_float(9);
			p.iAngValue = rs.get_float(10);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetDatabyTerminalAddrAndDate 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetDatabyTerminalAddr(vector<DATA> &v, int ConAddr, int TerminalAddr)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue from BR_DATA where TerminalAddr = ? and ConcentratorAddr = ?";
		st.prepare(sql);
		st.set_long( 0, TerminalAddr );
		st.set_long( 1, ConAddr );
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			DATA p;
			p.DataID = rs.get_long(0);
			p.TerminalAddr = rs.get_long(1);
			p.ConcentratorAddr = rs.get_long(2);
			p.CollectTime = rs.get_bigInt(3);
			p.relaycnt = rs.get_long(4);
			p.relayPosition = rs.get_long(5);
			p.GetherUnitAddr = rs.get_long(6);
			p.vValue = rs.get_float(7);
			p.vAngValue = rs.get_float(8);
			p.iValue = rs.get_float(9);
			p.iAngValue = rs.get_float(10);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetDatabyTerminalAddr 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//禄帽碌脙碌卤脟掳鹿芦脣戮ID碌脛脣霉脫脨脳脫鹿芦脣戮
int CDATAOperate::GetAllSubCompanyByID(vector<SUBCOMPANY> &v, int CompanyID)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select SubCompanyID,CompanyID,SubNAME,SVersion from BR_SUBCOMPANY where CompanyID = ?";
		st.prepare(sql);
		st.set_long( 0, CompanyID );
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			SUBCOMPANY p;
			p.subCompanyID = rs.get_long(0);
			p.companyID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllSubCompanyByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//获得所有的供电所
int CDATAOperate::GetAllAMSO(vector<AMSO> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select AmsoID,SubCompanyID,AmsoNAME,SVersion from BR_AMSO";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			AMSO p;
			p.AMSOID = rs.get_long(0);
			p.subCompanyID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllAMSO 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllAMSOByID(vector<AMSO> &v, int subCompanyID)//禄帽碌脙碌卤脟掳脳脫鹿芦脣戮ID碌脛脣霉脫脨鹿漏碌莽脣霉
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select AmsoID,SubCompanyID,AmsoNAME,SVersion from BR_AMSO where SubCompanyID=?";
		st.prepare(sql);
		st.set_long(0, subCompanyID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			AMSO p;
			p.AMSOID = rs.get_long(0);
			p.subCompanyID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllAMSOByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllRouteByID(vector<ROUTE> &v, int amsoID)//禄帽碌脙碌卤脟掳鹿漏碌莽脣霉碌脛脣霉脫脨脧脽脗路
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select RouteID,AmsoID,RouteNAME,SVersion from BR_ROUTE where AmsoID=?";
		st.prepare(sql);
		st.set_long(0, amsoID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			ROUTE p;
			p.routeID = rs.get_long(0);
			p.AMSOID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDescribe = rs.get_string(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllRouteByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllConcentratorByID(vector<CONCENTRATOR> &v, int routeID)//禄帽碌脙碌卤脟掳脧脽脗路碌脛脣霉脫脨录炉脰脨脝梅
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select ConcentratorID,RouteID,ConcentratorNAME,DestAddr,DestPort,ConnectType,InstallPlace,APName,APProtocol,GetTerminalTimer,GetConcentratorTimer,HeartTimer,SimCardNumber,GPRSReConnectTimer,GPRSSignalStrength,SaveTimer,wirelessSearchTimer,ConcentratorAddr,ConcentratorCurrentTime,SelfReportOnOff from BR_CONCENTRATOR where RouteID=?";
		st.prepare(sql);
		st.set_long(0, routeID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			CONCENTRATOR p;
			p.ConcentratorID = rs.get_long(0);
			p.routeID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strDestIP = rs.get_string(3);
			p.strDestPort = rs.get_string(4);
			p.strConnectType = rs.get_string(5);
			p.strInstallPlace = rs.get_string(6);
			p.strAPName = rs.get_string(7);
			p.strAPProtocol = rs.get_string(8);
			p.TerminalTimer = rs.get_long(9);
			p.ConcentratorTimer = rs.get_long(10);
			p.HeartTimer = rs.get_long(11);
			p.strSimCard = rs.get_string(12);
			p.GPRSReConnectTimer = rs.get_long(13);
			p.GPRSSignalStrength = rs.get_long(14);
			p.SaveTimer = rs.get_long(15);
			p.wirelessSearchTimer = rs.get_long(16);
			p.ConcentratorAddr = rs.get_long(17);
			p.ConcentratorCurrentTime = rs.get_bigInt(18);
			p.SelfReportOnOff = rs.get_long(19);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllConcentratorByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllLineByID(vector<LINE> &v, int concentratorID)//禄帽碌脙碌卤脟掳录炉脰脨脝梅碌脛脣霉脫脨脧脽露脦
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select LineID,ConcentratorID,LineNAME,LineType,LineAddr,PreAddr,NextAddr,WorkerID,Ratio from BR_LINE where ConcentratorID=?";
		st.prepare(sql);
		st.set_long(0, concentratorID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			LINE p;
			p.lineID = rs.get_long(0);
			p.ConcentratorID = rs.get_long(1);
			p.strName = rs.get_string(2);
            p.strType = rs.get_long(3);
			p.strAddr = rs.get_string(4);
			p.strPreAddr = rs.get_string(5);
			p.strNextAddr = rs.get_string(6);
			p.workerID = rs.get_long(7);
			p.Ratio = rs.get_float(8);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllLineByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllMonitorByID(vector<MONITOR> &v, int lineID)//禄帽碌脙碌卤脟掳脧脽露脦碌脛脣霉脫脨录脿虏芒碌茫
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select MonitorID,LineID,MonitorNAME,MonitorAddr,PreMonitorID from BR_MONITOR where LineID=?";
		st.prepare(sql);
		st.set_long(0, lineID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			MONITOR p;
			p.MonitorID = rs.get_long(0);
			p.lineID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.MonitorAddr = rs.get_string(3);
			p.PreMonitorID = rs.get_long(4);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllMonitorByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllTerminalByID(vector<TERMINAL> &v, int MonitorID)//禄帽碌脙碌卤脟掳录矛虏芒碌茫脧脗碌脛脣霉脫脨脰脮露脣
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select TerminalID,MonitorID,TerminalNAME,TerminalTYPE,TerminalIndex,TerminalInstallTime,TerminalAddr,TerminalPreAddr,TerminalNextAddr,ConcentratorAddr,TerminalCurrentTime,RouteState1,RouteState2,RouteState3,RouteState4,RouteState5,RouteState6,HighValue,HighOffset,HighSymbol,LowValue from BR_TERMINAL where MonitorID=?";
		st.prepare(sql);
		st.set_long(0, MonitorID);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			TERMINAL p;
			p.TerminalID = rs.get_long(0);
			p.MonitorID = rs.get_long(1);
			p.strName = rs.get_string(2);
			p.strType = rs.get_string(3);
			p.index = rs.get_long(4);
			p.installTime = rs.get_bigInt(5);
			p.addr = rs.get_long(6);
			p.preAddr = rs.get_long(7);
			p.nextAddr = rs.get_long(8);
			p.ConcentratorAddr = rs.get_long(9);
			p.TerminalCurrentTime = rs.get_bigInt(10);
			p.RouteState1 = rs.get_long(11);
			p.RouteState2 = rs.get_long(12);
			p.RouteState3 = rs.get_long(13);
			p.RouteState4 = rs.get_long(14);
			p.RouteState5 = rs.get_long(15);
			p.RouteState6 = rs.get_long(16);
			p.HighValue = rs.get_long(17);
			p.HighOffset = rs.get_float(18);
			p.HighSymbol = rs.get_long(19);
			p.LowValue = rs.get_long(20);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllTerminalByID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetAllUsers(vector<USR> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select UserID,UserName,UserCode,UserPri from BR_USER";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			USR p;
			p.usrID =  rs.get_long(0);
			p.usrName = rs.get_string(1);
			p.usrPassWord = rs.get_string(2);
			p.usrLever = rs.get_long(3);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetAllUsers 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//获得所有的还没报过警的报警信息
int CDATAOperate::GetWarningNopop(vector<WARNING> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select WarningTime,WarningLine,MonitorAddr1,MonitorAddr2,Type,iValue1,iValue2,WorkerName,WarningInfo,SendTime,SendState,WarningID from BR_WARNING where Popuped = ?";
		st.prepare(sql);
		st.set_long(0, 0);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			WARNING p;
			p.WarningTime = rs.get_bigInt(0);
			p.WarningLine = rs.get_long(1);
			p.MonitorAddr1 = rs.get_string(2);
			p.MonitorAddr2 = rs.get_string(3);
			p.Type = rs.get_long(4);
			p.iValue1 = rs.get_float(5);
			p.iValue2 = rs.get_float(6);
			p.WorkerName = rs.get_string(7);
			p.WarningInfo = rs.get_string(8);
			p.SendTime = rs.get_bigInt(9);
			p.SendState = rs.get_long(10);
			p.WarningID = rs.get_long(11);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetWarningNopop 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}
//获得所有的已经报过警的报警信息
int CDATAOperate::GetWarningPoped(vector<WARNING> &v)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select WarningTime,WarningLine,MonitorAddr1,MonitorAddr2,Type,iValue1,iValue2,WorkerName,WarningInfo,SendTime,SendState,WarningID from BR_WARNING where Popuped = ?";
		st.prepare(sql);
		st.set_long(0, 1);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			WARNING p;
			p.WarningTime = rs.get_bigInt(0);
			p.WarningLine = rs.get_long(1);
			p.MonitorAddr1 = rs.get_string(2);
			p.MonitorAddr2 = rs.get_string(3);
			p.Type = rs.get_long(4);
			p.iValue1 = rs.get_float(5);
			p.iValue2 = rs.get_float(6);
			p.WorkerName = rs.get_string(7);
			p.WarningInfo = rs.get_string(8);
			p.SendTime = rs.get_bigInt(9);
			p.SendState = rs.get_long(10);
			p.WarningID = rs.get_long(11);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetWarningPoped 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetWarning(vector<WARNING> &v)
{
	try
	{
		PreparedStatement st(m_conn);
        const char *sql = "select WarningTime,WarningLine,MonitorAddr1,MonitorAddr2,Type,iValue1,iValue2,WorkerName,WarningInfo,SendTime,SendState,WarningID from BR_WARNING";
		st.prepare(sql);
		ADO_WRAPPER::ResultSet rs = st.execute();
		v.clear();
		while( !rs.db_eof() )
		{
			WARNING p;
			p.WarningTime = rs.get_bigInt(0);
			p.WarningLine = rs.get_long(1);
			p.MonitorAddr1 = rs.get_string(2);
			p.MonitorAddr2 = rs.get_string(3);
			p.Type = rs.get_long(4);
			p.iValue1 = rs.get_float(5);
			p.iValue2 = rs.get_float(6);
			p.WorkerName = rs.get_string(7);
			p.WarningInfo = rs.get_string(8);
			p.SendTime = rs.get_bigInt(9);
			p.SendState = rs.get_long(10);
			p.WarningID = rs.get_long(11);
			v.push_back(p);
			rs.move_next();
		}
		rs.close();
		return 1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetWarning 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetUserID(string usr, string code)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select UserID from BR_USER where UserName = ? and UserCode = ?";
		st.prepare(sql);
		st.set_string(0, usr);
		st.set_string(1, code);
		ADO_WRAPPER::ResultSet rs = st.execute();
		while( !rs.db_eof() )
		{
			int res =  rs.get_long(0);
			rs.close();
			return res;
		}
		rs.close();
		return -1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetUserID 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetUserLever(int id)
{
	try
	{
		PreparedStatement st(m_conn);
		const char *sql = "select UserPri from BR_USER where UserID = ?";
		st.prepare(sql);
		st.set_long(0, id);
		ADO_WRAPPER::ResultSet rs = st.execute();
		while( !rs.db_eof() )
		{
			int res =  rs.get_long(0);
			rs.close();
			return res;
		}
		rs.close();
		return -1;
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetUserLever 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

//小帅用//通过一条记录找到下一个监测点相同类型的终端的同时刻记录。返回说明 1,返回成功、-1前面没有监测点、0未知错误
int CDATAOperate::GetNextDatabyData(DATA &dest, DATA src)
{
	try
	{

		int iMonitorID = -1;
		int curMonitorAddr = -1;
		int nextMonitorID = -1;
		int preTerminalAddr = -1;
		string strType;
		//根据一条记录中的终端地址找到其所属监测点ID，并获取终端类型
		PreparedStatement st(m_conn);
		const char *sql = "select MonitorID,TerminalTYPE from BR_TERMINAL where TerminalAddr = ?";
		st.prepare(sql);
		st.set_long(0, src.TerminalAddr);
		ADO_WRAPPER::ResultSet rs = st.execute();
		while( !rs.db_eof() )
		{
			iMonitorID =  rs.get_long(0);
			strType = rs.get_string(1);
			break;
		}
		rs.close();

		//根据当前监测点ID获得下一监测点ID
		PreparedStatement st1(m_conn);
		const char *sql1 = "select MonitorID from BR_MONITOR where PreMonitorID = ?";
		st1.prepare(sql1);
		st1.set_long(0, iMonitorID);
		ADO_WRAPPER::ResultSet rs1 = st1.execute();
		while( !rs1.db_eof() )
		{
			nextMonitorID =  rs1.get_long(0);
			break;
		}
		rs1.close();
		if (nextMonitorID < 0)
		{
			return -1;
		}

		//根据前一个监测点ID找到对应的同类型终端地址
		PreparedStatement st3(m_conn);
		const char *sql3 = "select TerminalAddr from BR_TERMINAL where MonitorID = ? and TerminalTYPE = ?";
		st3.prepare(sql3);
		st3.set_long(0, nextMonitorID);
		st3.set_string(1, strType);
		ADO_WRAPPER::ResultSet rs3 = st3.execute();
		while( !rs3.db_eof() )
		{
			preTerminalAddr =  rs3.get_long(0);
			break;
		}
		rs3.close();
		//最后根据终端地址和时间找到最终要找的数据
		PreparedStatement st4(m_conn);
		const char *sql4 = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue from BR_DATA where TerminalAddr = ? and CollectTime = ?";
		st4.prepare(sql4);
		st4.set_long(0, preTerminalAddr);
		st4.set_bigInt(1, src.CollectTime);
		ADO_WRAPPER::ResultSet rs4 = st4.execute();
		while( !rs4.db_eof() )
		{
			dest.DataID = rs4.get_long(0);
			dest.TerminalAddr = rs4.get_long(1);
			dest.ConcentratorAddr = rs4.get_long(2);
			dest.CollectTime = rs4.get_bigInt(3);
			dest.relaycnt = rs4.get_long(4);
			dest.relayPosition = rs4.get_long(5);
			dest.GetherUnitAddr = rs4.get_long(6);
			dest.vValue = rs4.get_float(7);
			dest.vAngValue = rs4.get_float(8);
			dest.iValue = rs4.get_float(9);
			dest.iAngValue = rs4.get_float(10);
			return 1;
		}
		rs4.close();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetNextDatabyData 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

int CDATAOperate::GetPreDatabyData(DATA &dest, DATA src)
{
	try
	{

		int iMonitorID = -1;
		int curMonitorAddr = -1;
		int preMonitorID = -1;
		int preTerminalAddr = -1;
		string strType;
		//根据一条记录中的终端地址找到其所属监测点ID，并获取终端类型
		PreparedStatement st(m_conn);
		const char *sql = "select MonitorID,TerminalTYPE from BR_TERMINAL where TerminalAddr = ?";
		st.prepare(sql);
		st.set_long(0, src.TerminalAddr);
		ADO_WRAPPER::ResultSet rs = st.execute();
		while( !rs.db_eof() )
		{
			iMonitorID =  rs.get_long(0);
			strType = rs.get_string(1);
			break;
		}
		rs.close();
		
		//根据当前监测点ID获得上一监测点ID
		PreparedStatement st1(m_conn);
		const char *sql1 = "select PreMonitorID from BR_MONITOR where MonitorID = ?";
		st1.prepare(sql1);
		st1.set_long(0, iMonitorID);
		ADO_WRAPPER::ResultSet rs1 = st1.execute();
		while( !rs1.db_eof() )
		{
			preMonitorID =  rs1.get_long(0);
			break;
		}
		rs1.close();
		if (preMonitorID < 0)
		{
			return -1;
		}
		
		//根据前一个监测点ID找到对应的同类型终端地址
		PreparedStatement st3(m_conn);
		const char *sql3 = "select TerminalAddr from BR_TERMINAL where MonitorID = ? and TerminalTYPE = ?";
		st3.prepare(sql3);
		st3.set_long(0, preMonitorID);
		st3.set_string(1, strType);
		ADO_WRAPPER::ResultSet rs3 = st3.execute();
		while( !rs3.db_eof() )
		{
			preTerminalAddr =  rs3.get_long(0);
			break;
		}
		rs3.close();
		//最后根据终端地址和时间找到最终要找的数据
		PreparedStatement st4(m_conn);
		const char *sql4 = "select DataID,TerminalAddr,ConcentratorAddr,CollectTime,relaycnt,relayPosition,GetherUnitAddr,vValue,vAngValue,iValue,iAngValue from BR_DATA where TerminalAddr = ? and CollectTime = ?";
		st4.prepare(sql4);
		st4.set_long(0, preTerminalAddr);
		st4.set_bigInt(1, src.CollectTime);
		ADO_WRAPPER::ResultSet rs4 = st4.execute();
		while( !rs4.db_eof() )
		{
			dest.DataID = rs4.get_long(0);
			dest.TerminalAddr = rs4.get_long(1);
			dest.ConcentratorAddr = rs4.get_long(2);
			dest.CollectTime = rs4.get_bigInt(3);
			dest.relaycnt = rs4.get_long(4);
			dest.relayPosition = rs4.get_long(5);
			dest.GetherUnitAddr = rs4.get_long(6);
			dest.vValue = rs4.get_float(7);
			dest.vAngValue = rs4.get_float(8);
			dest.iValue = rs4.get_float(9);
			dest.iAngValue = rs4.get_float(10);
			return 1;
		}
		rs4.close();
	}
	catch (_com_error& e)
	{
		char pLog[2048] = {0};
		sprintf(pLog, "GetPreDatabyData 失败！ %s\n", (char*)e.Description());
		WriteLog(pLog);
		OutputDebugString(e.Description());
		return 0;
	}
	return 0;
}

void CDATAOperate::WriteLog(string strInfo)
{
	FILE *pFile = NULL;
	pFile = fopen("D:\\Db2Log.txt","a");
	if (NULL == pFile )
	{
		return; //文件打开失败
	}
	int writeLen =0;
	writeLen = fwrite(strInfo.c_str(),1,strInfo.size(),pFile);  //写头长度
	fclose(pFile);
}
