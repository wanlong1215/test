drop TABLE BR_COMPANY;
drop TABLE BR_SUBCOMPANY;
drop TABLE BR_AMSO;
drop TABLE BR_ROUTE;
drop TABLE BR_CONCENTRATOR;
drop TABLE BR_LINE;
drop TABLE BR_MONITOR;
drop TABLE BR_TERMINAL;
drop TABLE BR_DATA;
drop TABLE BR_WORKER;
drop TABLE BR_USER;
drop TABLE BR_WARNING;
drop TABLE BR_DATA_REAL;
drop TABLE BR_COMMAND;

CREATE TABLE BR_COMPANY
(
	CompanyID             INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	CNAME                 VARCHAR(128)  NOT NULL,
	SVERSION              VARCHAR(128)  ,
	CONSTRAINT  PKBR_COMPANY_ID  PRIMARY KEY (CompanyID)
)
;



COMMENT ON TABLE BR_COMPANY IS '公司表'
;


COMMENT ON COLUMN BR_COMPANY.CompanyID IS '公司的ID主键'
;

COMMENT ON COLUMN BR_COMPANY.CNAME IS '公司名称'
;

COMMENT ON COLUMN BR_COMPANY.SVERSION IS '备注'
;


CREATE TABLE BR_SUBCOMPANY
(
	SubCompanyID          INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	CompanyID             INTEGER  NOT NULL,
	SubNAME               VARCHAR(128)  NOT NULL,
	SVERSION              VARCHAR(128)  ,
	CONSTRAINT  PKBR_COMPANY_ID  PRIMARY KEY (SubCompanyID),
	CONSTRAINT FK_COMPANY_ID FOREIGN KEY (CompanyID) REFERENCES BR_COMPANY(CompanyID)
)
;



COMMENT ON TABLE BR_SUBCOMPANY IS '子公司表'
;


COMMENT ON COLUMN BR_SUBCOMPANY.SubCompanyID IS '子公司的ID主键'
;

COMMENT ON COLUMN BR_SUBCOMPANY.CompanyID IS '总公司的ID'
;

COMMENT ON COLUMN BR_SUBCOMPANY.SubNAME IS '子公司名称'
;

COMMENT ON COLUMN BR_SUBCOMPANY.SVERSION IS '备注'
;


CREATE TABLE BR_AMSO
(
	AmsoID                INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	SubCompanyID          INTEGER  NOT NULL,
	AmsoNAME              VARCHAR(128)  NOT NULL,
	SVERSION              VARCHAR(128)  ,
	CONSTRAINT  PK_BR_AMSO  PRIMARY KEY (AmsoID),
	CONSTRAINT FK_BR_AMSO FOREIGN KEY (SubCompanyID) REFERENCES BR_SUBCOMPANY(SubCompanyID)
)
;



COMMENT ON TABLE BR_AMSO IS '供电所表'
;


COMMENT ON COLUMN BR_AMSO.AmsoID IS '供电所的ID主键'
;

COMMENT ON COLUMN BR_AMSO.SubCompanyID IS '所属分公司的ID'
;

COMMENT ON COLUMN BR_AMSO.AmsoNAME IS '供电所名称'
;

COMMENT ON COLUMN BR_AMSO.SVERSION IS '备注'
;



CREATE TABLE BR_ROUTE
(
	RouteID                INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	AmsoID         	       INTEGER  NOT NULL,
	RouteNAME              VARCHAR(128)  NOT NULL,
	SVERSION               VARCHAR(128)  ,
	CONSTRAINT  PK_BR_ROUTE  PRIMARY KEY (RouteID),
	CONSTRAINT FK_BR_ROUTE FOREIGN KEY (AmsoID) REFERENCES BR_AMSO(AmsoID)
)
;



COMMENT ON TABLE BR_ROUTE IS '线路表'
;


COMMENT ON COLUMN BR_ROUTE.RouteID IS '线路的ID主键'
;

COMMENT ON COLUMN BR_ROUTE.AmsoID IS '所属供电所的ID'
;

COMMENT ON COLUMN BR_ROUTE.RouteNAME IS '线路名称'
;

COMMENT ON COLUMN BR_ROUTE.SVERSION IS '备注'
;


CREATE TABLE BR_CONCENTRATOR
(
	ConcentratorID                INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	RouteID         	      INTEGER  NOT NULL,
	ConcentratorNAME              VARCHAR(128)  NOT NULL,
	DestAddr                      VARCHAR(128)  NOT NULL,
	DestPort                      VARCHAR(128)  NOT NULL,
	ConnectType		      VARCHAR(128)  NOT NULL,
	InstallPlace		      VARCHAR(128)  NOT NULL,
	APName		              VARCHAR(128)  NOT NULL,
	APProtocol		      VARCHAR(128)  NOT NULL,
	GetTerminalTimer              INTEGER  NOT NULL,
	GetConcentratorTimer          INTEGER  NOT NULL,
	HeartTimer          	      INTEGER  NOT NULL,
	SimCardNumber		      VARCHAR(128)  NOT NULL,
	GPRSReConnectTimer	      INTEGER  NOT NULL,
	GPRSSignalStrength 	      INTEGER  NOT NULL,
	SaveTimer		      INTEGER  NOT NULL,
        wirelessSearchTimer	      INTEGER  NOT NULL,
	ConcentratorAddr	      INTEGER  NOT NULL,
	ConcentratorCurrentTime       BIGINT  NOT NULL,
	SelfReportOnOff               INTEGER  NOT NULL,
	intRev1               	      INTEGER,
	intRev2               	      INTEGER,
	intRev3               	      INTEGER,
	intRev4               	      INTEGER,
	CONSTRAINT  PK_BR_CONCENTRATOR  PRIMARY KEY (ConcentratorID),
	CONSTRAINT FK_BR_CONCENTRATOR FOREIGN KEY (RouteID) REFERENCES BR_ROUTE(RouteID)
)
;



COMMENT ON TABLE BR_CONCENTRATOR IS '集中器表'
;


COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorID IS '集中器的ID主键'
;

COMMENT ON COLUMN BR_CONCENTRATOR.RouteID IS '所属线路的ID'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorNAME IS '集中器名称'
;

COMMENT ON COLUMN BR_CONCENTRATOR.DestAddr IS '目的地址'
;

COMMENT ON COLUMN BR_CONCENTRATOR.DestPort IS '目的端口'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConnectType IS '连接类型'
;

COMMENT ON COLUMN BR_CONCENTRATOR.InstallPlace IS '安装地点'
;

COMMENT ON COLUMN BR_CONCENTRATOR.APName IS '接入点名称'
;

COMMENT ON COLUMN BR_CONCENTRATOR.APProtocol IS '接入点协议'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GetTerminalTimer IS '获取终端间隔（分钟）'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GetConcentratorTimer IS '获取集中器间隔（分钟）'
;

COMMENT ON COLUMN BR_CONCENTRATOR.HeartTimer IS '心跳间隔'
;

COMMENT ON COLUMN BR_CONCENTRATOR.SimCardNumber IS 'sim卡号'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GPRSReConnectTimer IS 'Gprs掉线重拨间隔-分'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GPRSSignalStrength IS 'GPRS信号强度'
;

COMMENT ON COLUMN BR_CONCENTRATOR.SaveTimer IS '存储间隔'
;

COMMENT ON COLUMN BR_CONCENTRATOR.wirelessSearchTimer IS '无线模组轮寻间隔'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorAddr IS '集中器地址'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorCurrentTime IS '集中器当前时间'
;

COMMENT ON COLUMN BR_CONCENTRATOR.SelfReportOnOff IS '间隔自报开关'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev1 IS '整形备用1'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev2 IS '整形备用2'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev3 IS '整形备用3'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev4 IS '整形备用4'
;



CREATE TABLE BR_LINE
(
	LineID                        INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	ConcentratorID         	      INTEGER  NOT NULL,
	LineNAME                      VARCHAR(128)  NOT NULL,
	LineType                         INTEGER  NOT NULL,
	LineAddr		      VARCHAR(128)  NOT NULL,
	PreAddr		      VARCHAR(128)  NOT NULL,
	NextAddr		      VARCHAR(128)  NOT NULL,
	WorkerID		      INTEGER  NOT NULL,
	Ratio		      FLOAT  NOT NULL,
	CONSTRAINT  PK_BR_LINE  PRIMARY KEY (LineID),
	CONSTRAINT FK_BR_LINE FOREIGN KEY (ConcentratorID) REFERENCES BR_CONCENTRATOR(ConcentratorID)
)
;



COMMENT ON TABLE BR_LINE IS '线段表'
;



COMMENT ON COLUMN BR_LINE.LineID IS '线段的ID主键'
;


COMMENT ON COLUMN BR_LINE.ConcentratorID IS '所属集中器的ID'
;


COMMENT ON COLUMN BR_LINE.LineNAME IS '线段名称'
;


COMMENT ON COLUMN BR_LINE.LineType IS '线段类型'
;

COMMENT ON COLUMN BR_LINE.LineAddr IS '线段地址'
;

COMMENT ON COLUMN BR_LINE.PreAddr IS '前一线段地址'
;

COMMENT ON COLUMN BR_LINE.NextAddr IS '后一线段地址'
;

COMMENT ON COLUMN BR_LINE.WorkerID IS '接警人员ID'
;


CREATE TABLE BR_MONITOR
(
	MonitorID                        INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	LineID         	              INTEGER  NOT NULL,
	MonitorNAME                      VARCHAR(128)  NOT NULL,
	MonitorAddr	            VARCHAR(128)  NOT NULL,
	PreMonitorID         	              INTEGER  NOT NULL,
	CONSTRAINT  PK_BR_MONITOR  PRIMARY KEY (MonitorID),
	CONSTRAINT FK_BR_MONITOR FOREIGN KEY (LineID) REFERENCES BR_LINE(LineID)
)
;



COMMENT ON TABLE BR_MONITOR IS '监测点表'
;



COMMENT ON COLUMN BR_MONITOR.MonitorID IS '监测点的ID主键'
;


COMMENT ON COLUMN BR_MONITOR.LineID IS '所属线段名称'
;


COMMENT ON COLUMN BR_MONITOR.MonitorNAME IS '监测点名称'
;

COMMENT ON COLUMN BR_MONITOR.MonitorAddr IS '监测点地址'
;


CREATE TABLE BR_TERMINAL
(
	TerminalID                    INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	MonitorID         	              INTEGER  NOT NULL,
	TerminalNAME                  VARCHAR(128)  NOT NULL,
	TerminalTYPE                  VARCHAR(128)  NOT NULL,
	TerminalIndex		      INTEGER  NOT NULL,
	TerminalInstallTime           BIGINT  NOT NULL,
	TerminalAddr		      INTEGER  NOT NULL,
	TerminalPreAddr		      INTEGER  NOT NULL,
	TerminalNextAddr	      INTEGER  NOT NULL,
	ConcentratorAddr	      INTEGER  NOT NULL,
	TerminalCurrentTime           BIGINT  NOT NULL,
	RouteState1	      	      INTEGER  NOT NULL,
	RouteState2	      	      INTEGER  NOT NULL,
	RouteState3	      	      INTEGER  NOT NULL,
	RouteState4	      	      INTEGER  NOT NULL,
	RouteState5	      	      INTEGER  NOT NULL,
	RouteState6	      	      INTEGER  NOT NULL,
	HighValue	      	     	      INTEGER,
	HighOffset		      FLOAT,
	HighSymbol		      INTEGER,
	LowValue			      INTEGER,
	CONSTRAINT  PK_BR_TERMINAL  PRIMARY KEY (TerminalID),
	CONSTRAINT FK_BR_TERMINAL FOREIGN KEY (MonitorID) REFERENCES BR_MONITOR(MonitorID)
)
;



COMMENT ON TABLE BR_TERMINAL IS '终端表'
;


COMMENT ON COLUMN BR_TERMINAL.TerminalID IS '终端的ID主键'
;

COMMENT ON COLUMN BR_TERMINAL.MonitorID IS '所属监测点的ID'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalNAME IS '终端名称'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalTYPE IS '终端类型'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalIndex IS '终端索引'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalInstallTime IS '终端安装时间'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalAddr IS '终端地址'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalPreAddr IS '相邻前一个终端地址'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalNextAddr IS '相邻后一个终端地址'
;

COMMENT ON COLUMN BR_TERMINAL.ConcentratorAddr IS '集中器地址'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalCurrentTime IS '终端当前时间'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState1 IS '路由节点1'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState2 IS '路由节点2'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState3 IS '路由节点3'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState4 IS '路由节点4'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState5 IS '路由节点5'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState6 IS '路由节点6'
;

COMMENT ON COLUMN BR_TERMINAL.HighValue IS '高压值'
;

COMMENT ON COLUMN BR_TERMINAL.HighOffset IS '高压浮动值'
;

COMMENT ON COLUMN BR_TERMINAL.HighSymbol IS '高压浮动符号'
;

COMMENT ON COLUMN BR_TERMINAL.LowValue IS '低压值'
;


CREATE TABLE BR_DATA
(
	DataID                        INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	TerminalAddr		      INTEGER  NOT NULL,
	ConcentratorAddr	      INTEGER  NOT NULL,
	CollectTime                   BIGINT  NOT NULL,
	relaycnt                      INTEGER  NOT NULL,
	relayPosition                 INTEGER  NOT NULL,
	GetherUnitAddr                INTEGER  NOT NULL,
	vValue                 	      FLOAT  NOT NULL,
	vAngValue                     FLOAT  NOT NULL,
	iValue                 	      FLOAT  NOT NULL,
	iAngValue                     FLOAT  NOT NULL,
	intRev1                       INTEGER,
	intRev2                       INTEGER,
	intRev3                       INTEGER,
	intRev4                       INTEGER,
	intRev5                       INTEGER,
	intRev6                       INTEGER,
	CONSTRAINT  PK_BR_DATA  PRIMARY KEY (DataID)
)
;



COMMENT ON TABLE BR_DATA IS '采集数据表'
;


COMMENT ON COLUMN BR_DATA.DataID IS '数据的ID主键'
;

COMMENT ON COLUMN BR_DATA.TerminalAddr IS '所属终端的ID'
;

COMMENT ON COLUMN BR_DATA.ConcentratorAddr IS '所属集中器ID'
;

COMMENT ON COLUMN BR_DATA.CollectTime IS '采集时间'
;

COMMENT ON COLUMN BR_DATA.relaycnt IS '中继次数'
;

COMMENT ON COLUMN BR_DATA.relayPosition IS '中继位置'
;

COMMENT ON COLUMN BR_DATA.GetherUnitAddr IS '采集单位地址'
;

COMMENT ON COLUMN BR_DATA.vValue IS '采集电压值'
;

COMMENT ON COLUMN BR_DATA.vAngValue IS '采集电压角度'
;

COMMENT ON COLUMN BR_DATA.iValue IS '采集电流值'
;

COMMENT ON COLUMN BR_DATA.iAngValue IS '采集电流角度'
;

COMMENT ON COLUMN BR_DATA.intRev1 IS '备用1'
;

COMMENT ON COLUMN BR_DATA.intRev2 IS '备用2'
;

COMMENT ON COLUMN BR_DATA.intRev3 IS '备用3'
;

COMMENT ON COLUMN BR_DATA.intRev4 IS '备用4'
;

COMMENT ON COLUMN BR_DATA.intRev5 IS '备用5'
;

COMMENT ON COLUMN BR_DATA.intRev6 IS '备用6'
;


CREATE TABLE BR_WORKER
(
	WorkerID                 INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	WorkerName               VARCHAR(128)  NOT NULL,
	AmsoID                	 INTEGER  NOT NULL,
	WorkerPosition           VARCHAR(128)  NOT NULL,
	WorkerTEL                VARCHAR(128)  NOT NULL,
	CONSTRAINT  PK_BR_WORKER  PRIMARY KEY (WorkerID)
)
;


COMMENT ON TABLE BR_WORKER IS '接警人员表'
;


COMMENT ON COLUMN BR_WORKER.WorkerID IS '接警人员的ID主键'
;

COMMENT ON COLUMN BR_WORKER.AmsoID IS '所属部门'
;

COMMENT ON COLUMN BR_WORKER.WorkerPosition IS '职务'
;

COMMENT ON COLUMN BR_WORKER.WorkerTEL IS '电话号'
;


CREATE TABLE BR_USER
(
	UserID                 INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	UserName               VARCHAR(128)  NOT NULL,
	UserCode               VARCHAR(128)  NOT NULL,
	AmsoID                 INTEGER,
	UserPri                INTEGER  NOT NULL,
	CONSTRAINT  PK_BR_USER  PRIMARY KEY (UserID)
)
;


COMMENT ON TABLE BR_USER IS '用户人员表'
;


COMMENT ON COLUMN BR_USER.UserID IS '用户人员的ID主键'
;

COMMENT ON COLUMN BR_USER.UserName IS '用户名'
;

COMMENT ON COLUMN BR_USER.UserCode IS '密码'
;

COMMENT ON COLUMN BR_USER.UserPri IS '权限'
;

COMMENT ON COLUMN BR_USER.AmsoID IS '所属部门'
;


CREATE TABLE BR_WARNING
(
	WarningID                        INTEGER  NOT NULL
				GENERATED ALWAYS
				AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	WarningTime	          BIGINT  NOT NULL,
	WarningLine	          INTEGER  NOT NULL,
	MonitorAddr1	          VARCHAR(128)  NOT NULL,
	MonitorAddr2	          VARCHAR(128)  NOT NULL,
	Type                                   INTEGER  NOT NULL,
	iValue1                 	         FLOAT  NOT NULL,
	iValue2                              FLOAT  NOT NULL,
	WorkerName                     VARCHAR(128)  NOT NULL,
	WarningInfo                      VARCHAR(128)  NOT NULL,
	SendTime		         BIGINT  NOT NULL,
	SendState                          INTEGER  NOT NULL,
	Popuped		         INTEGER  NOT NULL,
	CONSTRAINT  PK_BR_WARNING  PRIMARY KEY (WarningID)
)
;



COMMENT ON TABLE BR_WARNING IS '报警信息表'
;


COMMENT ON COLUMN BR_WARNING.WarningID IS '报警信息的ID主键'
;

COMMENT ON COLUMN BR_WARNING.WarningTime IS '异常时间'
;

COMMENT ON COLUMN BR_WARNING.WarningLine IS '报警线段'
;

COMMENT ON COLUMN BR_WARNING.MonitorAddr1 IS '监测点1'
;

COMMENT ON COLUMN BR_WARNING.MonitorAddr2 IS '监测点2'
;

COMMENT ON COLUMN BR_WARNING.Type IS '相别'
;

COMMENT ON COLUMN BR_WARNING.iValue1 IS '电流值1'
;

COMMENT ON COLUMN BR_WARNING.iValue2 IS '电流值2'
;

COMMENT ON COLUMN BR_WARNING.WorkerName IS '接警员名字'
;

COMMENT ON COLUMN BR_WARNING.WarningInfo IS '报警信息'
;

COMMENT ON COLUMN BR_WARNING.SendTime IS '发送时间'
;

COMMENT ON COLUMN BR_WARNING.SendState IS '推送状态'
;

COMMENT ON COLUMN BR_WARNING.Popuped IS '是否已弹窗'
;


CREATE TABLE BR_DATA_REAL
(
	DataID                        INTEGER  NOT NULL
			GENERATED ALWAYS
			AS IDENTITY (
				START WITH 1
				INCREMENT BY 1
				),
	TerminalAddr		      INTEGER  NOT NULL,
	ConcentratorAddr	      INTEGER  NOT NULL,
	CollectTime                   BIGINT  NOT NULL,
	relaycnt                      INTEGER  NOT NULL,
	relayPosition                 INTEGER  NOT NULL,
	GetherUnitAddr                INTEGER  NOT NULL,
	vValue                 	      FLOAT  NOT NULL,
	vAngValue                     FLOAT  NOT NULL,
	iValue                 	      FLOAT  NOT NULL,
	iAngValue                     FLOAT  NOT NULL,
	intRev1                       INTEGER,
	intRev2                       INTEGER,
	intRev3                       INTEGER,
	intRev4                       INTEGER,
	intRev5                       INTEGER,
	intRev6                       INTEGER,
	CONSTRAINT  PK_BR_DATA_REAL  PRIMARY KEY (DataID)
)
;

COMMENT ON TABLE BR_DATA_REAL IS '实时采集数据表'
;


COMMENT ON COLUMN BR_DATA_REAL.DataID IS '数据的ID主键'
;

COMMENT ON COLUMN BR_DATA_REAL.TerminalAddr IS '所属终端的ID'
;

COMMENT ON COLUMN BR_DATA_REAL.ConcentratorAddr IS '所属集中器ID'
;

COMMENT ON COLUMN BR_DATA_REAL.CollectTime IS '采集时间'
;

COMMENT ON COLUMN BR_DATA_REAL.relaycnt IS '中继次数'
;

COMMENT ON COLUMN BR_DATA_REAL.relayPosition IS '中继位置'
;

COMMENT ON COLUMN BR_DATA_REAL.GetherUnitAddr IS '采集单位地址'
;

COMMENT ON COLUMN BR_DATA_REAL.vValue IS '采集电压值'
;

COMMENT ON COLUMN BR_DATA_REAL.vAngValue IS '采集电压角度'
;

COMMENT ON COLUMN BR_DATA_REAL.iValue IS '采集电流值'
;

COMMENT ON COLUMN BR_DATA_REAL.iAngValue IS '采集电流角度'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev1 IS '备用1'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev2 IS '备用2'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev3 IS '备用3'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev4 IS '备用4'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev5 IS '备用5'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev6 IS '备用6'
;


CREATE TABLE BR_COMMAND
(
	UserID                 	     INTEGER,
	ConcentratorAddr         INTEGER, 
	TerminalAddr                INTEGER,
	Commandtype              INTEGER
)
;

COMMENT ON TABLE BR_COMMAND IS '实时指令表'
;


COMMENT ON COLUMN BR_COMMAND.UserID IS '指令发出用户'
;

COMMENT ON COLUMN BR_COMMAND.ConcentratorAddr IS '集中器地址'
;

COMMENT ON COLUMN BR_COMMAND.TerminalAddr IS '终端地址'
;

COMMENT ON COLUMN BR_COMMAND.Commandtype IS '指令类型'
;
