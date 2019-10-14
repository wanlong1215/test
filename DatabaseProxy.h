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
    QString name;//终端名称
    QString type;//终端类型：A高压 B低压 C变压
    int index;//终端索引
    qint64 installTime;//终端安装时间
    int addr;//终端地址编号
    int preAddr;//相邻前一个终端编号
    int nextAddr;//相邻后一个终端编号
    int ConcentratorAddr;//集中器地址
    qint64 TerminalCurrentTime;//终端当前信息
    int RouteState1;//路由节点1
    int RouteState2;//路由节点2
    int RouteState3;//路由节点3
    int RouteState4;//路由节点4
    int RouteState5;//路由节点5
    int RouteState6;//路由节点6

	//变比参数
    int highPressureValue;//高压值
    float highPressureOffset;//高压浮动值
    int highPressureSymbol;//0:+ 1:- 高压浮动符号
    int lowPressureValue;//低压值

    proMonitor *parent;
};

// 监测点内包含3个终端
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

    int id;
    QString name;
    QString addr;
    QString preAddr;
    QString nextAddr;
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
    int type; // TODO: 高压 低压 变压
    int workerID;

	QString addr;
    QString preAddr;//相邻前一个终端编号
    QString nextAddr;//相邻后一个终端编号

    //int iValueA;
    //int iValueB;
    //int iValueC;

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
    QString name;//集中器名称
    QString destIp;//目标域
    QString destPort;//目标端口
    QString type;//连接类型
    QString installAddr;//安装地点
    QString apName;//接入点名称
    QString apProtocol;//接入点协议
    int terminalTimer;//终端读取间隔
    int concentratorTimer;//集中器读取间隔
    int heartTimer;//心跳间隔
    QString strSimCard;//sim卡号
    int gprsReConnectTimer;//Gprs掉线重拨间隔-分
    int gprsSignalStrength;//GPRS信号强度
    int saveTimer;//存储间隔
    int wirelessSearchTimer;//无线模组轮寻间隔
    int concentratorAddr;//集中器地址
    qint64 ConcentratorCurrentTime;//集中器当前时间
	int SelfReportOnOff;//自检开关

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
    qint64 CollectTime; // 采集时间
    int relaycnt;//中继次数
    int relayPosition;//中继位置
    int GetherUnitAddr;//采集单位地址
    float vValue;//采集电压值
    float vAngValue;//采集电压角度
    float iValue;//采集电流值
    float iAngValue;//采集电流角度
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
	int MonitorAddr1;
	int MonitorAddr2;
	int Type;
	float iValue1;
	float iValue2;
	QString WorkerName;
	QString WarningInfo;
	INT64 SendTime;
	int SendState;
    int isPopup; // 客户端是否弹窗，默认0，弹出后客户端修改成1
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

class DatabaseProxy
{
public:
	DatabaseProxy();
    static DatabaseProxy &instance();

    bool connectDB(const QString &ip, const QString &usr, const QString &pwd); // 连接DB
    bool isDBConnected(); // 当前DB状态是否连接

    bool testDB(const QString &ip, const QString &usr, const QString &pwd); // 测试数据库连接

    int userId(const QString &usr, const QString &pwd); // 根据用户名和密码返回对应的id值
    int userLevel(int id); // 根据用户ID获取用户级别，错误返回-1
    int addUser(const QString &usr, const QString &pwd, int level); // 添加用户，返回ID，错误返回-1
    bool modifyUser(int id, const QString &usr, const QString &pwd, int level); // 修改用户信息，错误返回false
    bool delUser(int id); // 删除用户

    QList<proUser> users();

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
    bool addWarning(proWarning *o);     //添加报警信息，这里客户端不调用，统一从服务端插入

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

	void GetShowDataInfoByConcentratorID(showData &sData, int ConcentratorId);//这个函数是通过组织架构寻找需要显示的信息中的各个名称

    proCompany *company(int id);
    proSubCompany *subCompany(int id);
    proAmso *amso(int id);
    proRoute *route(int id);
    proConcentrator *concentrator(int id);
    proLine *line(int id);
    proMonitor *monitor(int id);
    proTerminal *terminal(int id);

    QList<proAmso *> amsos();

    proConcentrator *firstConcentrator();

    QList<proData> historyData(int ConcentratorId, QDateTime begin, QDateTime end);//我没用

	bool historyDataByTime(QList<showData> &pDatalist, int ConcentratorId);//获得所有的历史数据
	bool historyDataByTime(QList<showData> &pDatalist, int ConcentratorId, INT64 begin, INT64 end);//通过时间查询历史数据，这里注意传入的时间是转换后的64位int
    bool historyWarning(QList<proWarning> &pDatalist, bool containsPopup = true);//获得历史报警信息

    int createId();

private:

	CDATAOperate m_db2;//操作DB2的对象
	bool m_connectState;//当前数据库的连接状态

    QList<proCompany *> _lst;
    QList<int> _ids;//应该没有用了
};

#endif // DATABASEPROXY_H
