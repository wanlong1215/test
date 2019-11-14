#ifndef DATABASEPROXY_H
#define DATABASEPROXY_H

#include <QDateTime>
#include <QString>
#include <QList>
#include "DATAOperate.h"

class proMonitor;
class proTerminal
{
public:
    proTerminal(proMonitor *p)
    {
        parent = p;
    }
    int id;
    QString name;
    QString type;
    int index;
    qint64 installTime;
    int addr;
    int preAddr;
    int nextAddr;
    int ConcentratorAddr;
    qint64 TerminalCurrentTime;
    int RouteState1;
    int RouteState2;
    int RouteState3;
    int RouteState4;
    int RouteState5;
    int RouteState6;

    int highPressureValue;
    float highPressureOffset;
    int highPressureSymbol;
    int lowPressureValue;

    proMonitor *parent;
};

class proLine;
class proMonitor
{
public:
    proMonitor(proLine * p)
    {
        parent = p;
    }
    ~proMonitor() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }

    QString pressureValueA();
    QString pressureValueB();
    QString pressureValueC();

    QString rtva;
    QString rtvb;
    QString rtvc;

    int id;
    QString name;
	QString addr;
	int PreMonitorID;
    QList<proTerminal *> lst;

    proLine *parent;
};

class proConcentrator;
class proLine
{
public:
    proLine(proConcentrator *p)
    {
        parent = p;
    }
    ~proLine() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }
    int id;
    QString name;
    int type;
    int workerID;

	QString addr;
    QString preAddr;
    QString nextAddr;
	float Ratio;

    proConcentrator *parent;
    QList<proMonitor *> lst;
};

class proRoute;
class proConcentrator
{
public:
    proConcentrator(proRoute *p)
    {
        parent = p;
    }
    ~proConcentrator() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }

    QList<proLine *> getSortLine();

    int id;
    QString name;
    QString destIp;
    QString destPort;
    QString type;
    QString installAddr;
    QString apName;
    QString apProtocol;
    int terminalTimer;
    int concentratorTimer;
    int heartTimer;
    QString strSimCard;
    int gprsReConnectTimer;
    int gprsSignalStrength;
    int saveTimer;
    int wirelessSearchTimer;
    int concentratorAddr;
    qint64 ConcentratorCurrentTime;
    int SelfReportOnOff;

    proRoute *parent;
    QList<proLine *> lst;
};

class proAmso;
class proRoute
{
public:
    proRoute(proAmso *p)
    {
        parent = p;
    }
    ~proRoute() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }
    int id;
    QString name;
    QString desc;

    proAmso *parent;
    QList<proConcentrator *> lst;
};

class proSubCompany;
class proAmso
{
public:
    proAmso(proSubCompany *p)
    {
        parent = p;
    }
    ~proAmso() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }
    int id;
    QString name;
    QString desc;

    proSubCompany *parent;
    QList<proRoute *> lst;
};

class proCompany;
class proSubCompany
{
public:
    proSubCompany(proCompany *p)
    {
        parent = p;
    }
    ~proSubCompany() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }

    int id;
    QString name;
    QString desc;

    proCompany *parent;
    QList<proAmso *> lst;
};

class proCompany
{
public:
    proCompany(){}
    ~proCompany() {
        while (!lst.isEmpty())
            delete lst.takeFirst();
    }
    int id;
    QString name;
    QString desc;

    QList<proSubCompany *> lst;
};

struct proData
{
    int DataID;
    int TerminalAddr;
    int ConcentratorAddr;
    qint64 CollectTime;
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

	int iValueA;
	int iValueB;
	int iValueC;
};

struct proWarning 
{
	int WarningID;
	INT64 WarningTime;
	int WarningLine;
	QString MonitorAddr1;
	QString MonitorAddr2;
	int Type;
	float iValue1;
	float iValue2;
	QString WorkerName;
	QString WarningInfo;
	INT64 SendTime;
	int SendState;
	int Popuped;
};


struct showData
{
	QString company;
	QString subCompany;
	QString amso;
	QString route;
	QString concentrator;
	QString line;
	QString monitor;
	DATA valueA;
	DATA valueB;
	DATA valueC;
};

struct proUser
{
    int id;
    QString name;
    QString pwd;
    int level;
};

struct proWorker
{
	int id;
	int amsoId;
	QString name;
	QString phone;
	QString remarks;
};

struct proCommand 
{
	int UserID;
	int ConcentratorAddr;
	int TerminalAddr;
	int Commandtype;
};


class DatabaseProxy
{
public:
	DatabaseProxy();
    static DatabaseProxy &instance();

    bool connectDB(const QString &ip, const QString &usr, const QString &pwd);
    bool isDBConnected();

    bool testDB(const QString &ip, const QString &usr, const QString &pwd);

    int userId(const QString &usr, const QString &pwd);
    int userLevel(int id);
    int addUser(const QString &usr, const QString &pwd, int level); // 添加用户，返回ID，错误返回-1
    bool modifyUser(int id, const QString &usr, const QString &pwd, int level); // 修改用户信息，错误返回false
    bool delUser(int id); // 删除用户

    QList<proUser> users();
	QList<proAmso> amsos();

	QList<proWorker> workers();
	bool worker(int id, proWorker &worker);
	int addWorker(proWorker *u);
	bool delWorker(int id);
	bool modifyWorker(proWorker *u); // 以ID为准修改其他信息

    QList<proCompany *> getOrganizations();//获得所有的组织架构，在获得之前需要清楚组织架构，另外，获得组织架构之前应该先连接数据库，不应该在这个函数里，你看看在哪里合适吧
	void clearOrganizations();//清除组织架构，但是没有清数据库。

    bool addCompany(proCompany *o);
    bool addSubCompany(proSubCompany *o, int parentid);
    bool addAmso(proAmso *o, int parentid);
    bool addRoute(proRoute *o, int parentid);
    bool addConcentrator(proConcentrator *o, int parentid);
    bool addLine(proLine *o, int parentid);
    bool addMonitor(proMonitor *o, int parentid);
    bool addTerminal(proTerminal *o, int parentid);
	bool addData(proData *o);//添加一条终端的数据，这里的时间需要注意一下，小帅那边应该都把秒去掉了
	bool addWarning(proWarning *o);//添加报警信息
	bool addCommand(proCommand *o);//添加指令

    bool modifyCompany(proCompany *o);
    bool modifySubCompany(proSubCompany *o);
    bool modifyAmso(proAmso *o);
    bool modifyRoute(proRoute *o);
    bool modifyConcentrator(proConcentrator *o);
    bool modifyLine(proLine *o);
    bool modifyMonitor(proMonitor *o);
    bool modifyTerminal(proTerminal *o);
	bool modifyWarningPopuped(int id);  // 客户端只修改报警数据中的字段isPopup=1

    bool delCompany(int id);
    bool delSubCompany(int id);
    bool delAmso(int id);
    bool delRoute(int id);
    bool delConcentrator(int id);
    bool delLine(int id);
	bool delMonitor(int id);
    bool delTerminal(int id);

	void GetShowDataInfoByConcentratorAddr(showData &sData, int ConcentratorAddr);//这个函数是通过组织架构寻找需要显示的信息中的各个名称

    proCompany *company(int id);
    proSubCompany *subCompany(int id);
    proAmso *amso(int id);
    proRoute *route(int id);
    proConcentrator *concentrator(int id);
    proLine *line(int id);
    proMonitor *monitor(int id);
    proTerminal *terminal(int id);
	proConcentrator *concentratorAddr(int addr);

    proConcentrator *firstConcentrator();

    QList<proData> historyData(int ConcentratorId, QDateTime begin, QDateTime end);//我没用

	bool historyDataByTerminalAddr(QList<proData> &pDatalist, int ConcentratorAddr, int TerminalAddr);//获得所有的历史数据 注意这里传入的是终端
	bool historyDataByTime(QList<showData> &pDatalist, int ConcentratorAddr);//获得所有的历史数据 注意这里传入的是集中器地址
	bool historyDataByTime(QList<showData> &pDatalist, int ConcentratorAddr, INT64 begin, INT64 end);//通过时间查询历史数据，这里注意传入的时间是转换后的64位int
	bool historyWarningNopop(QList<proWarning> &pDatalist);//获得没有报警信息
	bool historyWarningAll(QList<proWarning> &pDatalist);//获得所有的报警信息，包含已报的和未报的
	bool historyWarningPoped(QList<proWarning> &pDatalist);//获得已经报过警的信息

	bool realTimeData(vector<DATA> &v);//获得实时数据 注意这里传入的是集中器地址

    string ToString(const QString& qstr);
    QString ToQString(const string& cstr);

    void sortOrganization();

    static bool compareCompany(const proCompany *o1, const proCompany *o2);
    static bool compareSubCompany(const proSubCompany *o1, const proSubCompany *o2);
    static bool compareAmso(const proAmso *o1, const proAmso *o2);
    static bool compareRoute(const proRoute *o1, const proRoute *o2);
    static bool compareConcentrator(const proConcentrator *o1, const proConcentrator *o2);
    static bool compareLine(const proLine *o1, const proLine *o2);
    static bool compareMonitor(const proMonitor *o1, const proMonitor *o2);
private:
    CDATAOperate m_db2;
    bool m_connectState;

    QList<proCompany *> _lst;
};

#endif // DATABASEPROXY_H
