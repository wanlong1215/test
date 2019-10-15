#include "AlarmConfigDlg.h"
#include "ui_AlarmConfigDlg.h"
#include "DatabaseProxy.h"

AlarmConfigDlg::AlarmConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmConfigDlg)
{
    ui->setupUi(this);

    init();
}

AlarmConfigDlg::~AlarmConfigDlg()
{
    delete ui;
}


void AlarmConfigDlg::init()
{
    // 设置属性
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableWidget->setShowGrid(false);//不显示内部的格子线条

    // 设置列名
    QStringList lstHeader;
    lstHeader << QStringLiteral("异常时间") << QStringLiteral("报警线段") << QStringLiteral("监测点1") << QStringLiteral("监测点2") << QStringLiteral("相别")
              << QStringLiteral("电流值1")
              << QStringLiteral("电流值2")
              << QStringLiteral("报警员")
              << QStringLiteral("报警信息")
              << QStringLiteral("发送时间")
              << QStringLiteral("推送状态");
    ui->tableWidget->setHorizontalHeaderLabels( lstHeader );

    // 添加数据
    QList<proWarning> datas;
    if (!DatabaseProxy::instance().historyWarningAll(datas))
    {
        return;
    }

    // 如果是空直接返回
    if (datas.isEmpty())
        return;

    ui->tableWidget->setRowCount( datas.count() );

    int i = 0;
    foreach (auto data, datas) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(data.WarningTime).toString("yyyy-MM-dd hh:MM")));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(data.WarningLine));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(data.MonitorAddr1));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(data.MonitorAddr1));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(data.Type));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(data.iValue1));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(data.iValue1));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(data.WorkerName));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(data.WarningInfo));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QDateTime::fromSecsSinceEpoch(data.SendTime).toString("yyyy-MM-dd hh:MM")));
        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(data.SendState));
        i++;
    }
}
