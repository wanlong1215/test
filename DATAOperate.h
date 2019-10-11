#pragma once
#include "dbtool_ado.h"
using namespace ADO_WRAPPER;
#include <vector>
using namespace std;


struct COMPANY 
{
	int companyID;
	string strName;
	string strDescribe;
};

struct SUBCOMPANY
{
	int subCompanyID;
	int companyID;
	string strName;
	string strDescribe;
};

struct AMSO 
{
	int AMSOID;
	int subCompanyID;
	string strName;
	string strDescribe;
};

struct ROUTE
{
	int routeID;//ËùÊôÏßÂ·ID
	int AMSOID;
	string strName;
	string strDescribe;
};

struct CONCENTRATOR 
{
	int ConcentratorID;
	int routeID;//ËùÊôÏßÂ·ID
	string strName;//¼¯ÖÐÆ÷Ãû³Æ
	string strDestIP;//Ä¿±êÓò
	string strDestPort;//Ä¿±ê¶Ë¿Ú
	string strConnectType;//Á¬½ÓÀàÐÍ
	string strInstallPlace;//°²×°µØµã
	string strAPName;//½ÓÈëµãÃû³Æ
	string strAPProtocol;//½ÓÈëµãÐ­Òé
	int TerminalTimer;//ÖÕ¶Ë¶ÁÈ¡¼ä¸ô
	int	ConcentratorTimer;//¼¯ÖÐÆ÷¶ÁÈ¡¼ä¸ô
	int HeartTimer;//ÐÄÌø¼ä¸ô
	string strSimCard;//sim¿¨ºÅ
	int GPRSReConnectTimer;//GprsµôÏßÖØ²¦¼ä¸ô-·Ö
	int GPRSSignalStrength;//GPRSÐÅºÅÇ¿¶È
	int SaveTimer;//´æ´¢¼ä¸ô
	int wirelessSearchTimer;//ÎÞÏßÄ£×éÂÖÑ°¼ä¸ô
	int ConcentratorAddr;//¼¯ÖÐÆ÷µØÖ·
	INT64 ConcentratorCurrentTime;//¼¯ÖÐÆ÷µ±Ç°Ê±¼ä
	int SelfReportOnOff;//×Ô±¬¼ä¸ô¿ª¹Ø
};

struct LINE 
{
	int lineID;
	int ConcentratorID;
	string strName;
    int strType;
	string strAddr;
	string strPreAddr;
	string strNextAddr;
	int	workerID;
};

struct MONITOR
{
	int MonitorID;
	int lineID;
	string strName;
};

struct TERMINAL 
{
	int TerminalID;
	int MonitorID;
	string strName;//ÖÕ¶ËÃû³Æ
	string strType;//ÖÕ¶ËÀàÐÍ
	int index;//ÖÕ¶ËË÷Òý
	INT64 installTime;//ÖÕ¶Ë°²×°Ê±¼ä
	int addr;//ÖÕ¶ËµØÖ·±àºÅ
	int preAddr;//ÏàÁÚÇ°Ò»¸öÖÕ¶Ë±àºÅ
	int nextAddr;//ÏàÁÚºóÒ»¸öÖÕ¶Ë±àºÅ
	int ConcentratorAddr;//¼¯ÖÐÆ÷µØÖ·
	INT64 TerminalCurrentTime;//ÖÕ¶Ëµ±Ç°ÐÅÏ¢
	int RouteState1;//Â·ÓÉ½Úµã1
	int RouteState2;//Â·ÓÉ½Úµã2
	int RouteState3;//Â·ÓÉ½Úµã3
	int RouteState4;//Â·ÓÉ½Úµã4
	int RouteState5;//Â·ÓÉ½Úµã5
	int RouteState6;//Â·ÓÉ½Úµã6
	int HighValue;//¸ßÑ¹Öµ
	float HighOffset;//¸ßÑ¹¸¡¶¯Öµ
	int HighSymbol;//0:+ 1:- ¸ßÑ¹¸¡¶¯·ûºÅ
	int LowValue;//µÍÑ¹Öµ
};

struct DATA 
{
	int DataID;
	int TerminalAddr;
	int ConcentratorAddr;
	INT64 CollectTime;
	int relaycnt;
	int relayPosition;
	int GetherUnitAddr;
	float vValue;
	float vAngValue;
	float iValue;
	float iAngValue;
	int intRev1;
	int intRev2;
	int intRev3;
	int intRev4;
	int intRev5;
	int intRev6;
};

struct WARNING 
{
	int WarningID;
	INT64 WarningTime;
	int WarningLine;
	int MonitorAddr1;
	int MonitorAddr2;
	int Type;
	float iValue1;
	float iValue2;
	string WorkerName;
	string WarningInfo;
	INT64 SendTime;
	int SendState;

};

class CDATAOperate
{
public:
	CDATAOperate();
	~CDATAOperate(void);
	BOOL DB2Connect(string strIP, string strUsr, string strCode);
	///¸÷¸ö±íµÄ²åÈë²Ù×÷£¬³É¹¦·µ»Ø²åÈëÏîµÄË÷Òý£¬Ê§°Ü·µ»Ø0
	int InsertCompany(COMPANY p);
	int InsertSubCompany(SUBCOMPANY p, int CompanyID);
	int InsertAMSO(AMSO p, int SubCompanyID);
	int InsertRoute(ROUTE p, int AMSOID);
	int InsertConcentrator(CONCENTRATOR p, int RouteID);
	int InsertLine(LINE p, int ConcentratorID);
	int InsertMonitor(MONITOR p, int LineID);
	int InsertTerminal(TERMINAL p, int MonitorID);
	int InsertData(DATA p);
	int InsertWarning(WARNING p);
	///¸÷¸ö±íµÄÉ¾³ý²Ù×÷£¬³É¹¦·µ»ØTURE£¬Ê§°Ü·µ»ØFALSE
	BOOL DelCompany(int companyID);
	BOOL DelSubCompany(int subCompanyID);
	BOOL DelAMSO(int AMSOID);
	BOOL DelRoute(int RouteID);
	BOOL DelConcentrator(int ConcentratorID);
	BOOL DelLine(int lineID);
	BOOL DelMonitor(int MonitorID);
	BOOL DelTerminal(int TerminalID);
	BOOL DelData(int TerminalAddr);
	///ÐÞ¸Ä¸÷¸ö±íµÄÖµ£¬³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
	int ModifyCompany(COMPANY p, int CompanyID);
	int ModifySubCompany(SUBCOMPANY p, int SubCompanyID);
	int ModifyAMSO(AMSO p, int AMSOID);
	int ModifyRoute(ROUTE p, int RouteID);
	int ModifyConcentrator(CONCENTRATOR p, int ConcentratorID);
	int ModifyLine(LINE p, int LineID);
	int ModifyMonitor(MONITOR p, int MonitorID);
	int ModifyTerminal(TERMINAL p, int TerminalID);
	//int ModifyData(int TerminalID, string strValue);
	///²éÑ¯¸÷¸ö±íµÄÖµ£¬³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
	int GetCompany(COMPANY &p, int CompanyID);
	int GetSubCompany(SUBCOMPANY &p, int SubCompanyID);
	int GetAMSO(AMSO &p, int AMSOID);
	int GetRoute(ROUTE &p, int RouteID);
	int GetConcentrator(CONCENTRATOR &p, int ConcentratorID);
	int GetLine(LINE &p, int LineID);
	int GetMonitor(MONITOR &p, int MonitorID);
	int GetTerminal(TERMINAL &p, int TerminalID);
	///·µ»Ø¸÷¸ö±íµÄËùÓÐÏî£¬³É¹¦·µ»Ø1£¬Ê§°Ü·µ»Ø0
	int GetAllCompanyID(vector<COMPANY> &v);
	int GetAllSubCompanyID(vector<SUBCOMPANY> &v);
	int GetAllAMSOID(vector<AMSO> &v);
	int GetAllRouteID(vector<ROUTE> &v);
	int GetAllConcentratorID(vector<CONCENTRATOR> &v);
	int GetAllLineID(vector<LINE> &v);
	int GetAllTerminalID(vector<TERMINAL> &v);
	///Í¨¹ý¸÷¸öÍâ¼üµÄID»ñµÃËùÊôÍâ¼üµÄÏî
	int GetAllSubCompanyByID(vector<SUBCOMPANY> &v, int CompanyID);//»ñµÃµ±Ç°¹«Ë¾IDµÄËùÓÐ×Ó¹«Ë¾
	int GetAllAMSOByID(vector<AMSO> &v, int subCompanyID);//»ñµÃµ±Ç°×Ó¹«Ë¾IDµÄËùÓÐ¹©µçËù
	int GetAllRouteByID(vector<ROUTE> &v, int amsoID);//»ñµÃµ±Ç°¹©µçËùµÄËùÓÐÏßÂ·
	int GetAllConcentratorByID(vector<CONCENTRATOR> &v, int routeID);//»ñµÃµ±Ç°ÏßÂ·µÄËùÓÐ¼¯ÖÐÆ÷
	int GetAllLineByID(vector<LINE> &v, int concentratorID);//»ñµÃµ±Ç°¼¯ÖÐÆ÷µÄËùÓÐÏß¶Î
	int GetAllMonitorByID(vector<MONITOR> &v, int lineID);//»ñµÃµ±Ç°Ïß¶ÎµÄËùÓÐ¼à²âµã
	int GetAllTerminalByID(vector<TERMINAL> &v, int MonitorID);//»ñµÃµ±Ç°¼ì²âµãÏÂµÄËùÓÐÖÕ¶Ë
	//int GetData(int TerminalID);
	///Í¨¹ý¼¯ÖÐÆ÷ID·µ»ØËùÓÐµ±Ç°¼¯ÖÐÆ÷ÏÂµÄËùÓÐÖÕ¶ËÊý¾Ý
	int GetDatabyConcentratorID(vector<DATA> &v, int ConAddr);
	int GetDatabyTerminalID(vector<DATA> &v, int ConAddr, int TerminalAddr);
	int GetDatabyTerminalIDAndData(vector<DATA> &v, int ConAddr, int TerminalAddr, INT64 begin, INT64 end);
	int GetDatabyTerminalAddrAndTime(DATA &data, int TerminalAddr, INT64 time);
	int GetWarning(vector<WARNING> &v);


private:
	Connection m_conn;
};
