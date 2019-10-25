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
	int ConcentratorID;//��������ID����
	int routeID;//������·��ID
	string strName;//����������
	string strDestIP;//Ŀ�ĵ�ַ
	string strDestPort;//Ŀ�Ķ˿�
	string strConnectType;//��������
	string strInstallPlace;//��װ�ص�
	string strAPName;//���������
	string strAPProtocol;//�����Э��
	int TerminalTimer;//��ȡ�ն˼�������ӣ�
	int	ConcentratorTimer;//��ȡ��������������ӣ�
	int HeartTimer;//�������
	string strSimCard;//sim����
	int GPRSReConnectTimer;//Gprs�����ز����-��
	int GPRSSignalStrength;//GPRS�ź�ǿ��
	int SaveTimer;//�洢���
	int wirelessSearchTimer;//����ģ����Ѱ���
	int ConcentratorAddr;//��������ַ
	INT64 ConcentratorCurrentTime;//��������ǰʱ��
	int SelfReportOnOff;//����Ա�����
	int reserve1;
	int reserve2;
	int reserve3;
	int reserve4;
	int heartCount;//��������
	int conState;//����״̬
	string selfIP;//�Լ���IP
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
	string strName;//�ն�����
	string strType;//�ն�����
	int index;//�ն�����
	INT64 installTime;//�ն˰�װʱ��
	int addr;//�ն˵�ַ���
	int preAddr;//����ǰһ���ն˱��
	int nextAddr;//���ں�һ���ն˱��
	int ConcentratorAddr;//��������ַ
	INT64 TerminalCurrentTime;//�ն˵�ǰ��Ϣ
	int RouteState1;//·�ɽڵ�1
	int RouteState2;//·�ɽڵ�2
	int RouteState3;//·�ɽڵ�3
	int RouteState4;//·�ɽڵ�4
	int RouteState5;//·�ɽڵ�5
	int RouteState6;//·�ɽڵ�6
	int HighValue;//��ѹֵ
	float HighOffset;//��ѹƫ��ֵ
	int HighSymbol;//0:+ 1:- ��ѹ����
	int LowValue;//��ѹֵ
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
	int intRev1;//���Ԥ����Ϊ�쳣��־
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
	///������Ĳ���������ɹ����ز������������ʧ�ܷ���0
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
	int InsertRealData(DATA p);//����ʵʱ����
	int InsertCommand(COMMAND p);//����ָ��
	///�������ɾ���������ɹ�����TURE��ʧ�ܷ���FALSE
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
	///�޸ĸ������ֵ���ɹ�����1��ʧ�ܷ���0
	int ModifyCompany(COMPANY p, int CompanyID);
	int ModifySubCompany(SUBCOMPANY p, int SubCompanyID);
	int ModifyAMSO(AMSO p, int AMSOID);
	int ModifyRoute(ROUTE p, int RouteID);
	int ModifyConcentrator(CONCENTRATOR p, int ConcentratorID);//û��д���� С˧��
	int ModifyConcentratorReserve(CONCENTRATOR p, int ConcentratorID);//�˺���ר��д���õ��ĸ����� С˧��
	int ModifyLine(LINE p, int LineID);
	int ModifyMonitor(MONITOR p, int MonitorID);
	int ModifyTerminal(TERMINAL p, int TerminalID);
	int ModifyUser(int id, string usr, string code, int level);
	int ModifyWorker(WORKER p, int WorkerID);
	int ModifyWarningPop(int id);
	//int ModifyData(int TerminalID, string strValue);
	///��ѯ�������ֵ���ɹ�����1��ʧ�ܷ���0
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
	///���ظ������������ɹ�����1��ʧ�ܷ���0
	int GetAllCompanyID(vector<COMPANY> &v);
	int GetAllSubCompanyID(vector<SUBCOMPANY> &v);
	int GetAllAMSOID(vector<AMSO> &v);
	int GetAllRouteID(vector<ROUTE> &v);
	int GetAllConcentratorID(vector<CONCENTRATOR> &v);
	int GetAllLineID(vector<LINE> &v);
	int GetAllTerminalID(vector<TERMINAL> &v);
	int GetAllTerminalByConcentratorID(vector<TERMINAL> &v, int ConcentratorAddr);
	int GetAllWorker(vector<WORKER> &v);
	///����ӿ�
	int GetAllSubCompanyByID(vector<SUBCOMPANY> &v, int CompanyID);//ͨ����˾ID��������ӹ�˾ID
	int GetAllAMSOByID(vector<AMSO> &v, int subCompanyID);//ͨ���ӹ�˾ID����ӹ�˾���еĹ�����
	int GetAllAMSO(vector<AMSO> &v);//������еĹ�����
	int GetAllRouteByID(vector<ROUTE> &v, int amsoID);//ͨ��������ID����������ڹ���������·
	int GetAllConcentratorByID(vector<CONCENTRATOR> &v, int routeID);//ͨ����·ID������и���·�ϵļ�����
	int GetAllLineByID(vector<LINE> &v, int concentratorID);//ͨ��������ID��ü������ϵ������߶�
	int GetAllMonitorByID(vector<MONITOR> &v, int lineID);//ͨ���߶�ID��õ�ǰ�߶������еļ���
	int GetAllTerminalByID(vector<TERMINAL> &v, int MonitorID);//ͨ������ID������иü����ϵ��ն�
	
	int GetDatabyConcentratorAddr(vector<DATA> &v, int ConAddr);//�������-ͨ����������ַ�������������ڸü��������ն˵��������ݶ�����
	int GetDatabyTerminalAddr(vector<DATA> &v, int ConAddr, int TerminalAddr);//�������-ͨ����������ַ���ն˵�ַ
	 //С˧��//ͨ��һ����¼�ҵ���һ��������ͬ���͵��ն˵�ͬʱ�̼�¼������˵�� 1,���سɹ���-1ǰ��û�м��㡢0δ֪����
	int GetPreDatabyData(DATA &dest, DATA src);
	//С˧��//ͨ��һ����¼�ҵ���һ��������ͬ���͵��ն˵�ͬʱ�̼�¼������˵�� 1,���سɹ���-1ǰ��û�м��㡢0δ֪����
	int GetNextDatabyData(DATA &dest, DATA src);
	 //��ʱ��ȷ�����ص�����
	int GetDatabyTerminalAddrAndDate(vector<DATA> &v, int ConAddr, int TerminalAddr, INT64 begin, INT64 end);
	//ͨ���ն˵�ַ��ʱ����Ѱ�ҵ�ǰ������
	int GetDatabyTerminalAddrAndTime(DATA &data, int TerminalAddr, INT64 time);
	//ͨ�����������ն˵�ַ��ʱ����Ѱ�ҵ�ǰ������
	int GetDatabyAddrAndTime(DATA &data, int TerminalAddr, int ConAddr, INT64 time);
	//������еı�����Ϣ
	int GetWarning(vector<WARNING> &v);
	//������еĻ�û�������ı�����Ϣ
	int GetWarningNopop(vector<WARNING> &v);
	//������е��Ѿ��������ı�����Ϣ
	int GetWarningPoped(vector<WARNING> &v);
	//ͨ���û����������õ�ǰ�û������ݿ��е�ID
	int GetUserID(string usr, string code);
	//ͨ���û�ID��ȡ�û�����
	int GetUserLever(int id);
	//��������û���Ϣ
	int GetAllUsers(vector<USR> &v);
	//ͨ���ն˵�ַ��Ѱ�ҵ�ǰ��ʵʱ����  -1��ʾĿǰ����û������ 0��ʾsql�д���, 1��ʾ��ȡ�ɹ�
	int GetRealData(vector<DATA> &v);
	//���ʵʱָ�� -1��ʾĿǰ����û��ָ�� 0��ʾsql�д���, 1��ʾ��ȡ�ɹ�
	int GetCommand(vector<COMMAND> &v);
	//������ݱ��е�ʱ�䣬���Լ���������
	int GetCollectTimeAndMoniterID(vector<TIME_ID> &v);
	//������ݱ��е�ʱ�䣬���Լ���������
	int GetDataByCollectTimeAndMonitorID(DATA &A, DATA &B, DATA &C, TIME_ID ti);

	void WriteLog(string strInfo);


private:
	Connection m_conn;
};
