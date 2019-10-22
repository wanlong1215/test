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



COMMENT ON TABLE BR_COMPANY IS '��˾��'
;


COMMENT ON COLUMN BR_COMPANY.CompanyID IS '��˾��ID����'
;

COMMENT ON COLUMN BR_COMPANY.CNAME IS '��˾����'
;

COMMENT ON COLUMN BR_COMPANY.SVERSION IS '��ע'
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



COMMENT ON TABLE BR_SUBCOMPANY IS '�ӹ�˾��'
;


COMMENT ON COLUMN BR_SUBCOMPANY.SubCompanyID IS '�ӹ�˾��ID����'
;

COMMENT ON COLUMN BR_SUBCOMPANY.CompanyID IS '�ܹ�˾��ID'
;

COMMENT ON COLUMN BR_SUBCOMPANY.SubNAME IS '�ӹ�˾����'
;

COMMENT ON COLUMN BR_SUBCOMPANY.SVERSION IS '��ע'
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



COMMENT ON TABLE BR_AMSO IS '��������'
;


COMMENT ON COLUMN BR_AMSO.AmsoID IS '��������ID����'
;

COMMENT ON COLUMN BR_AMSO.SubCompanyID IS '�����ֹ�˾��ID'
;

COMMENT ON COLUMN BR_AMSO.AmsoNAME IS '����������'
;

COMMENT ON COLUMN BR_AMSO.SVERSION IS '��ע'
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



COMMENT ON TABLE BR_ROUTE IS '��·��'
;


COMMENT ON COLUMN BR_ROUTE.RouteID IS '��·��ID����'
;

COMMENT ON COLUMN BR_ROUTE.AmsoID IS '������������ID'
;

COMMENT ON COLUMN BR_ROUTE.RouteNAME IS '��·����'
;

COMMENT ON COLUMN BR_ROUTE.SVERSION IS '��ע'
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



COMMENT ON TABLE BR_CONCENTRATOR IS '��������'
;


COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorID IS '��������ID����'
;

COMMENT ON COLUMN BR_CONCENTRATOR.RouteID IS '������·��ID'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorNAME IS '����������'
;

COMMENT ON COLUMN BR_CONCENTRATOR.DestAddr IS 'Ŀ�ĵ�ַ'
;

COMMENT ON COLUMN BR_CONCENTRATOR.DestPort IS 'Ŀ�Ķ˿�'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConnectType IS '��������'
;

COMMENT ON COLUMN BR_CONCENTRATOR.InstallPlace IS '��װ�ص�'
;

COMMENT ON COLUMN BR_CONCENTRATOR.APName IS '���������'
;

COMMENT ON COLUMN BR_CONCENTRATOR.APProtocol IS '�����Э��'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GetTerminalTimer IS '��ȡ�ն˼�������ӣ�'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GetConcentratorTimer IS '��ȡ��������������ӣ�'
;

COMMENT ON COLUMN BR_CONCENTRATOR.HeartTimer IS '�������'
;

COMMENT ON COLUMN BR_CONCENTRATOR.SimCardNumber IS 'sim����'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GPRSReConnectTimer IS 'Gprs�����ز����-��'
;

COMMENT ON COLUMN BR_CONCENTRATOR.GPRSSignalStrength IS 'GPRS�ź�ǿ��'
;

COMMENT ON COLUMN BR_CONCENTRATOR.SaveTimer IS '�洢���'
;

COMMENT ON COLUMN BR_CONCENTRATOR.wirelessSearchTimer IS '����ģ����Ѱ���'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorAddr IS '��������ַ'
;

COMMENT ON COLUMN BR_CONCENTRATOR.ConcentratorCurrentTime IS '��������ǰʱ��'
;

COMMENT ON COLUMN BR_CONCENTRATOR.SelfReportOnOff IS '����Ա�����'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev1 IS '���α���1'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev2 IS '���α���2'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev3 IS '���α���3'
;

COMMENT ON COLUMN BR_CONCENTRATOR.intRev4 IS '���α���4'
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



COMMENT ON TABLE BR_LINE IS '�߶α�'
;



COMMENT ON COLUMN BR_LINE.LineID IS '�߶ε�ID����'
;


COMMENT ON COLUMN BR_LINE.ConcentratorID IS '������������ID'
;


COMMENT ON COLUMN BR_LINE.LineNAME IS '�߶�����'
;


COMMENT ON COLUMN BR_LINE.LineType IS '�߶�����'
;

COMMENT ON COLUMN BR_LINE.LineAddr IS '�߶ε�ַ'
;

COMMENT ON COLUMN BR_LINE.PreAddr IS 'ǰһ�߶ε�ַ'
;

COMMENT ON COLUMN BR_LINE.NextAddr IS '��һ�߶ε�ַ'
;

COMMENT ON COLUMN BR_LINE.WorkerID IS '�Ӿ���ԱID'
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



COMMENT ON TABLE BR_MONITOR IS '�����'
;



COMMENT ON COLUMN BR_MONITOR.MonitorID IS '�����ID����'
;


COMMENT ON COLUMN BR_MONITOR.LineID IS '�����߶�����'
;


COMMENT ON COLUMN BR_MONITOR.MonitorNAME IS '��������'
;

COMMENT ON COLUMN BR_MONITOR.MonitorAddr IS '�����ַ'
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



COMMENT ON TABLE BR_TERMINAL IS '�ն˱�'
;


COMMENT ON COLUMN BR_TERMINAL.TerminalID IS '�ն˵�ID����'
;

COMMENT ON COLUMN BR_TERMINAL.MonitorID IS '���������ID'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalNAME IS '�ն�����'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalTYPE IS '�ն�����'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalIndex IS '�ն�����'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalInstallTime IS '�ն˰�װʱ��'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalAddr IS '�ն˵�ַ'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalPreAddr IS '����ǰһ���ն˵�ַ'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalNextAddr IS '���ں�һ���ն˵�ַ'
;

COMMENT ON COLUMN BR_TERMINAL.ConcentratorAddr IS '��������ַ'
;

COMMENT ON COLUMN BR_TERMINAL.TerminalCurrentTime IS '�ն˵�ǰʱ��'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState1 IS '·�ɽڵ�1'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState2 IS '·�ɽڵ�2'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState3 IS '·�ɽڵ�3'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState4 IS '·�ɽڵ�4'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState5 IS '·�ɽڵ�5'
;

COMMENT ON COLUMN BR_TERMINAL.RouteState6 IS '·�ɽڵ�6'
;

COMMENT ON COLUMN BR_TERMINAL.HighValue IS '��ѹֵ'
;

COMMENT ON COLUMN BR_TERMINAL.HighOffset IS '��ѹ����ֵ'
;

COMMENT ON COLUMN BR_TERMINAL.HighSymbol IS '��ѹ��������'
;

COMMENT ON COLUMN BR_TERMINAL.LowValue IS '��ѹֵ'
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



COMMENT ON TABLE BR_DATA IS '�ɼ����ݱ�'
;


COMMENT ON COLUMN BR_DATA.DataID IS '���ݵ�ID����'
;

COMMENT ON COLUMN BR_DATA.TerminalAddr IS '�����ն˵�ID'
;

COMMENT ON COLUMN BR_DATA.ConcentratorAddr IS '����������ID'
;

COMMENT ON COLUMN BR_DATA.CollectTime IS '�ɼ�ʱ��'
;

COMMENT ON COLUMN BR_DATA.relaycnt IS '�м̴���'
;

COMMENT ON COLUMN BR_DATA.relayPosition IS '�м�λ��'
;

COMMENT ON COLUMN BR_DATA.GetherUnitAddr IS '�ɼ���λ��ַ'
;

COMMENT ON COLUMN BR_DATA.vValue IS '�ɼ���ѹֵ'
;

COMMENT ON COLUMN BR_DATA.vAngValue IS '�ɼ���ѹ�Ƕ�'
;

COMMENT ON COLUMN BR_DATA.iValue IS '�ɼ�����ֵ'
;

COMMENT ON COLUMN BR_DATA.iAngValue IS '�ɼ������Ƕ�'
;

COMMENT ON COLUMN BR_DATA.intRev1 IS '����1'
;

COMMENT ON COLUMN BR_DATA.intRev2 IS '����2'
;

COMMENT ON COLUMN BR_DATA.intRev3 IS '����3'
;

COMMENT ON COLUMN BR_DATA.intRev4 IS '����4'
;

COMMENT ON COLUMN BR_DATA.intRev5 IS '����5'
;

COMMENT ON COLUMN BR_DATA.intRev6 IS '����6'
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


COMMENT ON TABLE BR_WORKER IS '�Ӿ���Ա��'
;


COMMENT ON COLUMN BR_WORKER.WorkerID IS '�Ӿ���Ա��ID����'
;

COMMENT ON COLUMN BR_WORKER.AmsoID IS '��������'
;

COMMENT ON COLUMN BR_WORKER.WorkerPosition IS 'ְ��'
;

COMMENT ON COLUMN BR_WORKER.WorkerTEL IS '�绰��'
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


COMMENT ON TABLE BR_USER IS '�û���Ա��'
;


COMMENT ON COLUMN BR_USER.UserID IS '�û���Ա��ID����'
;

COMMENT ON COLUMN BR_USER.UserName IS '�û���'
;

COMMENT ON COLUMN BR_USER.UserCode IS '����'
;

COMMENT ON COLUMN BR_USER.UserPri IS 'Ȩ��'
;

COMMENT ON COLUMN BR_USER.AmsoID IS '��������'
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



COMMENT ON TABLE BR_WARNING IS '������Ϣ��'
;


COMMENT ON COLUMN BR_WARNING.WarningID IS '������Ϣ��ID����'
;

COMMENT ON COLUMN BR_WARNING.WarningTime IS '�쳣ʱ��'
;

COMMENT ON COLUMN BR_WARNING.WarningLine IS '�����߶�'
;

COMMENT ON COLUMN BR_WARNING.MonitorAddr1 IS '����1'
;

COMMENT ON COLUMN BR_WARNING.MonitorAddr2 IS '����2'
;

COMMENT ON COLUMN BR_WARNING.Type IS '���'
;

COMMENT ON COLUMN BR_WARNING.iValue1 IS '����ֵ1'
;

COMMENT ON COLUMN BR_WARNING.iValue2 IS '����ֵ2'
;

COMMENT ON COLUMN BR_WARNING.WorkerName IS '�Ӿ�Ա����'
;

COMMENT ON COLUMN BR_WARNING.WarningInfo IS '������Ϣ'
;

COMMENT ON COLUMN BR_WARNING.SendTime IS '����ʱ��'
;

COMMENT ON COLUMN BR_WARNING.SendState IS '����״̬'
;

COMMENT ON COLUMN BR_WARNING.Popuped IS '�Ƿ��ѵ���'
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

COMMENT ON TABLE BR_DATA_REAL IS 'ʵʱ�ɼ����ݱ�'
;


COMMENT ON COLUMN BR_DATA_REAL.DataID IS '���ݵ�ID����'
;

COMMENT ON COLUMN BR_DATA_REAL.TerminalAddr IS '�����ն˵�ID'
;

COMMENT ON COLUMN BR_DATA_REAL.ConcentratorAddr IS '����������ID'
;

COMMENT ON COLUMN BR_DATA_REAL.CollectTime IS '�ɼ�ʱ��'
;

COMMENT ON COLUMN BR_DATA_REAL.relaycnt IS '�м̴���'
;

COMMENT ON COLUMN BR_DATA_REAL.relayPosition IS '�м�λ��'
;

COMMENT ON COLUMN BR_DATA_REAL.GetherUnitAddr IS '�ɼ���λ��ַ'
;

COMMENT ON COLUMN BR_DATA_REAL.vValue IS '�ɼ���ѹֵ'
;

COMMENT ON COLUMN BR_DATA_REAL.vAngValue IS '�ɼ���ѹ�Ƕ�'
;

COMMENT ON COLUMN BR_DATA_REAL.iValue IS '�ɼ�����ֵ'
;

COMMENT ON COLUMN BR_DATA_REAL.iAngValue IS '�ɼ������Ƕ�'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev1 IS '����1'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev2 IS '����2'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev3 IS '����3'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev4 IS '����4'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev5 IS '����5'
;

COMMENT ON COLUMN BR_DATA_REAL.intRev6 IS '����6'
;


CREATE TABLE BR_COMMAND
(
	UserID                 	     INTEGER,
	ConcentratorAddr         INTEGER, 
	TerminalAddr                INTEGER,
	Commandtype              INTEGER
)
;

COMMENT ON TABLE BR_COMMAND IS 'ʵʱָ���'
;


COMMENT ON COLUMN BR_COMMAND.UserID IS 'ָ����û�'
;

COMMENT ON COLUMN BR_COMMAND.ConcentratorAddr IS '��������ַ'
;

COMMENT ON COLUMN BR_COMMAND.TerminalAddr IS '�ն˵�ַ'
;

COMMENT ON COLUMN BR_COMMAND.Commandtype IS 'ָ������'
;
