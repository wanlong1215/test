#include "AlarmConfigDlg.h"
#include "ui_AlarmConfigDlg.h"
#include "DatabaseProxy.h"
#include "AppSession.h"

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
    QMap<int, QString> map;
    map.insert(0, QStringLiteral("A相"));
    map.insert(1, QStringLiteral("B相"));
    map.insert(2, QStringLiteral("C相"));

    int i = 0;
    foreach (auto data, datas) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(AppSession::instance().toQDateTime(data.WarningTime).toString("yyyy-MM-dd hh:MM")));
        auto line = DatabaseProxy::instance().line(data.WarningLine);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(nullptr == line ? "" : line->name));

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(data.MonitorAddr1)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(data.MonitorAddr2)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(map.value(data.Type)));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(data.iValue1)));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(data.iValue2)));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(data.WorkerName));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(data.WarningInfo));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(AppSession::instance().toQDateTime(data.SendTime).toString("yyyy-MM-dd hh:MM")));
        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(data.SendState == 1 ? QStringLiteral("已发送") : QStringLiteral("未发送")));
        i++;
    }
}

void AlarmConfigDlg::on_leFilter_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        auto item = ui->tableWidget->item(i, 8);
        if (nullptr != item)
        {
            ui->tableWidget->setRowHidden(i, !item->text().contains(arg1));
        }
    }
}
