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
	int routeID;//脣霉脢么脧脽脗路ID
	int AMSOID;
	string strName;
	string strDescribe;
};

struct CONCENTRATOR 
{
	int ConcentratorID;//集中器的ID主键
	int routeID;//所属线路的ID
	string strName;//集中器名称
	string strDestIP;//目的地址
	string strDestPort;//目的端口
	string strConnectType;//连接类型
	string strInstallPlace;//安装地点
	string strAPName;//接入点名称
	string strAPProtocol;//接入点协议
	int TerminalTimer;//获取终端间隔（分钟）
	int	ConcentratorTimer;//获取集中器间隔（分钟）
	int HeartTimer;//心跳间隔
	string strSimCard;//sim卡号
	int GPRSReConnectTimer;//Gprs掉线重拨间隔-分
	int GPRSSignalStrength;//GPRS信号强度
	int SaveTimer;//存储间隔
	int wirelessSearchTimer;//无线模组轮寻间隔
	int ConcentratorAddr;//集中器地址
	INT64 ConcentratorCurrentTime;//集中器当前时间
	int SelfReportOnOff;//间隔自报开关
	int reserve1;
	int reserve2;
	int reserve3;
	int reserve4;
	int heartCount;//心跳计数
	int conState;//连接状态
	string selfIP;//自己的IP
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
	float Ratio;
};

struct MONITOR
{
	int MonitorID;
	int lineID;
	string strName;
	string MonitorAddr;
	int PreMonitorID;
};

struct TERMINAL 
{
	int TerminalID;
	int MonitorID;
	string strName;//终端名称
	string strType;//终端类型
	int index;//终端索引
	INT64 installTime;//终端安装时间
	int addr;//终端地址编号
	int preAddr;//相邻前一个终端编号
	int nextAddr;//相邻后一个终端编号
	int ConcentratorAddr;//集中器地址
	INT64 TerminalCurrentTime;//终端当前信息
	int RouteState1;//路由节点1
	int RouteState2;//路由节点2
	int RouteState3;//路由节点3
	int RouteState4;//路由节点4
	int RouteState5;//路由节点5
	int RouteState6;//路由节点6
	int HighValue;//高压值
	float HighOffset;//高压偏移值
	int HighSymbol;//0:+ 1:- 高压符号
	int LowValue;//低压值
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
	int intRev1;//这个预留作为异常标志
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
	string MonitorAddr1;
	string MonitorAddr2;
	int Type;
	float iValue1;
	float iValue2;
	string WorkerName;
	string WarningInfo;
	INT64 SendTime;
	int SendState;
	int Popuped;
};

struct USR 
{
	int usrID;
	string usrName;
	string usrPassWord;
	int usrLever;
};

struct WORKER
{
	int id;
	int amsoId;
	string name;
	string phone;
	string remarks;
};

struct COMMAND 
{
	int UserID;
	int ConcentratorAddr;
	int TerminalAddr;
	int Commandtype;
};

struct TIME_ID 
{
	int MonitorID;
	INT64 CollectTime;
};


class CDATAOperate
{
public:
	CDATAOperate();
	~CDATAOperate(void);
	BOOL DB2Connect(string strIP, string strUsr, string strCode);
	///各个表的插入操作，成功返回插入项的索引，失败返回0
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
	int InsertUser(string usr, string code, int level);
	int InsertWorker(WORKER p);
	int InsertRealData(DATA p);//插入实时数据
	int InsertCommand(COMMAND p);//插入指令
	///各个表的删除操作，成功返回TURE，失败返回FALSE
	BOOL DelCompany(int companyID);
	BOOL DelSubCompany(int subCompanyID);
	BOOL DelAMSO(int AMSOID);
	BOOL DelRoute(int RouteID);
	BOOL DelConcentrator(int ConcentratorID);
	BOOL DelLine(int lineID);
	BOOL DelMonitor(int MonitorID);
	BOOL DelTerminal(int TerminalID);
	BOOL DelData(int TerminalAddr);
	BOOL DelUser(int userID);
	BOOL DelWorker(int WorkerID);
	///修改各个表的值，成功返回1，失败返回0
	int ModifyCompany(COMPANY p, int CompanyID);
	int ModifySubCompany(SUBCOMPANY p, int SubCompanyID);
	int ModifyAMSO(AMSO p, int AMSOID);
	int ModifyRoute(ROUTE p, int RouteID);
	int ModifyConcentrator(CONCENTRATOR p, int ConcentratorID);//没有写备用 小帅用
	int ModifyConcentratorReserve(CONCENTRATOR p, int ConcentratorID);//此函数专门写备用的四个整形 小帅用
	int ModifyLine(LINE p, int LineID);
	int ModifyMonitor(MONITOR p, int MonitorID);
	int ModifyTerminal(TERMINAL p, int TerminalID);
	int ModifyUser(int id, string usr, string code, int level);
	int ModifyWorker(WORKER p, int WorkerID);
	int ModifyWarningPop(int id);
	//int ModifyData(int TerminalID, string strValue);
	///查询各个表的值，成功返回1，失败返回0
	int GetCompany(COMPANY &p, int CompanyID);
	int GetSubCompany(SUBCOMPANY &p, int SubCompanyID);
	int GetAMSO(AMSO &p, int AMSOID);
	int GetRoute(ROUTE &p, int RouteID);
	int GetConcentrator(CONCENTRATOR &p, int ConcentratorID);
	int GetLine(LINE &p, int LineID);
	int GetMonitor(MONITOR &p, int MonitorID);
	int GetTerminal(TERMINAL &p, int TerminalID);
	int GetTerminalByAddr(TERMINAL &p, int ConcentratorAddr, int TerminalAddr);
	int GetWorker(WORKER &p, int WorkerID);
	///返回各个表的所有项，成功返回1，失败返回0
	int GetAllCompanyID(vector<COMPANY> &v);
	int GetAllSubCompanyID(vector<SUBCOMPANY> &v);
	int GetAllAMSOID(vector<AMSO> &v);
	int GetAllRouteID(vector<ROUTE> &v);
	int GetAllConcentratorID(vector<CONCENTRATOR> &v);
	int GetAllLineID(vector<LINE> &v);
	int GetAllTerminalID(vector<TERMINAL> &v);
	int GetAllTerminalByConcentratorID(vector<TERMINAL> &v, int ConcentratorAddr);
	int GetAllWorker(vector<WORKER> &v);
	///龙哥接口
	int GetAllSubCompanyByID(vector<SUBCOMPANY> &v, int CompanyID);//通过公司ID获得所有子公司ID
	int GetAllAMSOByID(vector<AMSO> &v, int subCompanyID);//通过子公司ID获得子公司所有的供电所
	int GetAllAMSO(vector<AMSO> &v);//获得所有的供电所
	int GetAllRouteByID(vector<ROUTE> &v, int amsoID);//通过供电所ID获得所有属于供电所的线路
	int GetAllConcentratorByID(vector<CONCENTRATOR> &v, int routeID);//通过线路ID获得所有该线路上的集中器
	int GetAllLineByID(vector<LINE> &v, int concentratorID);//通过集中器ID获得集中器上的所有线段
	int GetAllMonitorByID(vector<MONITOR> &v, int lineID);//通过线段ID获得当前线段上所有的监测点
	int GetAllTerminalByID(vector<TERMINAL> &v, int MonitorID);//通过监测点ID获得所有该监测点上的终端
	
	int GetDatabyConcentratorAddr(vector<DATA> &v, int ConAddr);//获得数据-通过集中器地址，即将所有属于该集中器的终端的所有数据读出来
	int GetDatabyTerminalAddr(vector<DATA> &v, int ConAddr, int TerminalAddr);//获得数据-通过集中器地址与终端地址
	 //小帅用//通过一条记录找到上一个监测点相同类型的终端的同时刻记录。返回说明 1,返回成功、-1前面没有监测点、0未知错误
	int GetPreDatabyData(DATA &dest, DATA src);
	//小帅用//通过一条记录找到下一个监测点相同类型的终端的同时刻记录。返回说明 1,返回成功、-1前面没有监测点、0未知错误
	int GetNextDatabyData(DATA &dest, DATA src);
	 //用时间确定返回的数据
	int GetDatabyTerminalAddrAndDate(vector<DATA> &v, int ConAddr, int TerminalAddr, INT64 begin, INT64 end);
	//通过终端地址和时间来寻找当前的数据
	int GetDatabyTerminalAddrAndTime(DATA &data, int TerminalAddr, INT64 time);
	//通过集中器和终端地址和时间来寻找当前的数据
	int GetDatabyAddrAndTime(DATA &data, int TerminalAddr, int ConAddr, INT64 time);
	//获得所有的报警信息
	int GetWarning(vector<WARNING> &v);
	//获得所有的还没报过警的报警信息
	int GetWarningNopop(vector<WARNING> &v);
	//获得所有的已经报过警的报警信息
	int GetWarningPoped(vector<WARNING> &v);
	//通过用户名和密码获得当前用户在数据库中的ID
	int GetUserID(string usr, string code);
	//通过用户ID获取用户级别
	int GetUserLever(int id);
	//获得所有用户信息
	int GetAllUsers(vector<USR> &v);
	//通过终端地址来寻找当前的实时数据  -1表示目前库中没有数据 0表示sql有错误, 1表示读取成功
	int GetRealData(vector<DATA> &v);
	//获得实时指令 -1表示目前库中没有指令 0表示sql有错误, 1表示读取成功
	int GetCommand(vector<COMMAND> &v);
	//获得数据表中的时间，并以集中器分组
	int GetCollectTimeAndMoniterID(vector<TIME_ID> &v);
	//获得数据表中的时间，并以集中器分组
	int GetDataByCollectTimeAndMonitorID(DATA &A, DATA &B, DATA &C, TIME_ID ti);

	void WriteLog(string strInfo);


private:
	Connection m_conn;
};
