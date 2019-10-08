#ifndef DATABASEPROXY_H
#define DATABASEPROXY_H

#include <QList>

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
    int type;//终端类型：高压 低压 变压
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

    int highPressureValue;//变比参数
    float highPressureOffset;
    int highPressureSymbol;//0:+ 1:-
    int lowPressureValue;

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

    QString pressureValueA();
    QString pressureValueB();
    QString pressureValueC();

    int id;
    QString name;
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
    int id;
    QString name;
    int type; // TODO: 高压 低压 变压
    int workerID;

    int preAddr;//相邻前一个终端编号
    int nextAddr;//相邻后一个终端编号

    int iValueA;
    int iValueB;
    int iValueC;

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

    int id;
    QString name;
    QString desc;

    proCompany *parent;
    QList<proAmso *> lst;
};

class proCompany
{
public:
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
    int vValue;//采集电压值
    int vAngValue;//采集电压角度
    int iValue;//采集电流值
    int iAngValue;//采集电流角度
    int intRev1;
    int intRev2;
    int intRev3;
    int intRev4;
    int intRev5;
    int intRev6;

    int iValueA;// 界面展示三个电流值
    int iValueB;
    int iValueC;
};

class DatabaseProxy
{
public:
    static DatabaseProxy &instance();

    QList<proCompany *> getOrganizations();

    bool addCompany(proCompany *o);
    bool addSubCompany(proSubCompany *o, int parentid);
    bool addAmso(proAmso *o, int parentid);
    bool addRoute(proRoute *o, int parentid);
    bool addConcentrator(proConcentrator *o, int parentid);
    bool addLine(proLine *o, int parentid);
    bool addMonitor(proMonitor *o, int parentid);
    bool addTerminal(proTerminal *o, int parentid);

    bool delCompany(int id);
    bool delSubCompany(int id);
    bool delAmso(int id);
    bool delRoute(int id);
    bool delConcentrator(int id);
    bool delLine(int id);
    bool delTerminal(int id);

    proCompany *company(int id);
    proSubCompany *subCompany(int id);
    proAmso *amso(int id);
    proRoute *route(int id);
    proConcentrator *concentrator(int id);
    proLine *line(int id);
    proMonitor *monitor(int id);
    proTerminal *terminal(int id);

    proConcentrator *firstConcentrator();

    QList<proData> historyData(int ConcentratorId, QDateTime begin, QDateTime end);

    int createId();

private:
    DatabaseProxy();

    QList<proCompany *> _lst;
    QList<int> _ids;
};

#endif // DATABASEPROXY_H
