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
    _company = DatabaseProxy::instance().getOrganizations();

    // init time
    ui->dtBegin->setDateTime(QDateTime::currentDateTime().addDays(-3));
    ui->dtEnd->setDateTime(QDateTime::currentDateTime());

    // init combBox
    QStringList lstSub, lstAmso, lstLine, lstWorker;
    lstSub.append(QStringLiteral("全部"));
    lstAmso.append(QStringLiteral("全部"));
    lstLine.append(QStringLiteral("全部"));
    lstWorker.append(QStringLiteral("全部"));
    auto companys = _company;
    foreach (auto o1, companys) {
        foreach (auto o2, o1->lst) {
            lstSub.append(o2->name);
            foreach (auto o3, o2->lst) {
                lstAmso.append(o3->name);
                foreach (auto o4, o3->lst) {
                    lstLine.append(o4->name);
                }
            }
        }
    }
    ui->cbSubCompany->addItems(lstSub);
    ui->cbAsmo->addItems(lstAmso);
    ui->cbLine->addItems(lstLine);

    auto workers = DatabaseProxy::instance().workers();
    foreach (auto worker, workers) {
        lstWorker.append(worker.name);
    }
    ui->cbWorker->addItems(lstWorker);

    // 设置属性
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableWidget->setShowGrid(false);//不显示内部的格子线条

    // 设置列名
    QStringList lstHeader;
    lstHeader << QStringLiteral("序号") << QStringLiteral("异常时间") << QStringLiteral("报警信息")
              << QStringLiteral("分公司") << QStringLiteral("供电所") << QStringLiteral("线路")
              << QStringLiteral("监测点1") << QStringLiteral("监测点2") << QStringLiteral("相别")
              << QStringLiteral("电流值1")
              << QStringLiteral("电流值2")
              << QStringLiteral("报警员")
              << QStringLiteral("发送时间")
              << QStringLiteral("推送状态");
    ui->tableWidget->setHorizontalHeaderLabels( lstHeader );

    connect(ui->dtBegin, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onQuery()));
    connect(ui->dtEnd, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onQuery()));
    connect(ui->cbSubCompany, SIGNAL(currentIndexChanged(int)), this, SLOT(onSubCompayChanged()));
    connect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    connect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    connect(ui->cbWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(onWorkerChanged()));

    onQuery();
}

bool AlarmConfigDlg::getName(int lineId, QString &subCompany, QString &asmo, QString &lineName)
{
    auto lst = _company;
    foreach (auto o1, lst) {
        foreach (auto o2, o1->lst) {
            foreach (auto o3, o2->lst) {
                foreach (auto o4, o3->lst) {
                    foreach (auto o5, o4->lst) {
                        foreach (auto o6, o5->lst) {
                            if (o6->id == lineId) {
                                subCompany = o2->name;
                                asmo = o3->name;
                                lineName = o4->name;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

void AlarmConfigDlg::onQuery()
{
    // 添加数据
    QList<proWarning> datas;
    if (!DatabaseProxy::instance().historyWarningAll(datas))
    {
        return;
    }

    ui->tableWidget->setRowCount( datas.count() );

    // 如果是空直接返回
    if (datas.isEmpty())
        return;

    QMap<int, QString> map;
    map.insert(0, QStringLiteral("A相"));
    map.insert(1, QStringLiteral("B相"));
    map.insert(2, QStringLiteral("C相"));

    int i = 0;
    foreach (auto data, datas) {
        QString subCompany, asmoName, lineName;
        if (!getName(data.WarningLine, subCompany, asmoName, lineName)) {
            continue;
        }

        // filter subcompany
        if (ui->cbSubCompany->currentIndex() == 0) {
            //
        } else if (ui->cbSubCompany->currentText() != subCompany) {
            continue;
        }

        // filter amso
        if (ui->cbAsmo->currentIndex() == 0) {
            //
        } else if (ui->cbAsmo->currentText() != asmoName) {
            continue;
        }

        // filter line
        if (ui->cbLine->currentIndex() == 0) {
            //
        } else if (ui->cbLine->currentText() != lineName) {
            continue;
        }

        // filter worker
        if (ui->cbWorker->currentIndex() == 0) {
            //
        } else if (ui->cbWorker->currentText() != data.WorkerName) {
            continue;
        }

        // filter time
        qint64 btime = AppSession::instance().toInt64Time(ui->dtBegin->dateTime());
        qint64 etime = AppSession::instance().toInt64Time(ui->dtEnd->dateTime());
        if (btime > data.WarningTime || etime < data.WarningTime) {
            continue;
        }

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(AppSession::instance().toQDateTime(data.WarningTime).toString("yyyy-MM-dd hh:MM")));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(data.WarningInfo));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(subCompany));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(asmoName));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(lineName));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(data.MonitorAddr1));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(data.MonitorAddr2));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(map.value(data.Type)));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::number(data.iValue1)));
        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(QString::number(data.iValue2)));
        ui->tableWidget->setItem(i, 11, new QTableWidgetItem(data.WorkerName));
        ui->tableWidget->setItem(i, 12, new QTableWidgetItem(AppSession::instance().toQDateTime(data.SendTime).toString("yyyy-MM-dd hh:MM")));
        ui->tableWidget->setItem(i, 13, new QTableWidgetItem(data.SendState == 1 ? QStringLiteral("已发送") : QStringLiteral("未发送")));
        i++;
    }
    ui->tableWidget->setRowCount( i );
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

void AlarmConfigDlg::onSubCompayChanged()
{
    disconnect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    disconnect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    disconnect(ui->cbWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(onWorkerChanged()));

    QStringList lstSub, lstAmso, lstWorker;
    QList<int> lstAmsoId;
    lstAmso.append(QStringLiteral("全部"));
    lstWorker.append(QStringLiteral("全部"));
    auto companys = _company;
    foreach (auto o1, companys) {
        foreach (auto o2, o1->lst) {
            if (ui->cbSubCompany->currentIndex() == 0) {
                //
            } else if (ui->cbSubCompany->currentText() != o2->name) {
                continue;
            }
            lstSub.append(o2->name);
            foreach (auto o3, o2->lst) {
                lstAmsoId.append(o3->id);
                lstAmso.append(o3->name);
            }
        }
    }
    ui->cbAsmo->clear();
    ui->cbAsmo->addItems(lstAmso);

    auto workers = DatabaseProxy::instance().workers();
    foreach (auto worker, workers) {
        if (lstAmsoId.contains(worker.amsoId)) {
               lstWorker.append(worker.name);
        }
    }
    ui->cbWorker->clear();
    ui->cbWorker->addItems(lstWorker);

    connect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    connect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    connect(ui->cbWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(onWorkerChanged()));
    onQuery();
}

void AlarmConfigDlg::onAsmoChanged()
{
    disconnect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    disconnect(ui->cbWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(onWorkerChanged()));

    QStringList lstLine, lstWorker;
    QList<int> lstAmsoId;
    lstLine.append(QStringLiteral("全部"));
    lstWorker.append(QStringLiteral("全部"));
    auto companys = _company;
    foreach (auto o1, companys) {
        foreach (auto o2, o1->lst) {
            if (ui->cbSubCompany->currentIndex() == 0) {
                //
            } else if (ui->cbSubCompany->currentText() != o2->name) {
                continue;
            }
            foreach (auto o3, o2->lst) {
                lstAmsoId.append(o3->id);
                foreach (auto o4, o3->lst) {
                    lstLine.append(o4->name);
                }
            }
        }
    }
    ui->cbLine->clear();
    ui->cbLine->addItems(lstLine);

    auto workers = DatabaseProxy::instance().workers();
    foreach (auto worker, workers) {
        if (lstAmsoId.contains(worker.amsoId)) {
               lstWorker.append(worker.name);
        }
    }
    ui->cbWorker->clear();
    ui->cbWorker->addItems(lstWorker);

    connect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    connect(ui->cbWorker, SIGNAL(currentIndexChanged(int)), this, SLOT(onWorkerChanged()));
    onQuery();
}

void AlarmConfigDlg::onLineChanged()
{
    onQuery();
}

void AlarmConfigDlg::onWorkerChanged()
{
    onQuery();
}
#include <QFileDialog>
#include <QMessageBox>
#include "xlsxdocument.h"
void AlarmConfigDlg::on_btnExport_clicked()
{
    if (0 == ui->tableWidget->rowCount()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("没有数据，无法导出!"));
        return;
    }

    // 获取保存文件路径
    auto fileDlg = new QFileDialog(this);
    fileDlg->setWindowTitle(QStringLiteral("保存文件"));
    fileDlg->setAcceptMode(QFileDialog::AcceptSave);
    fileDlg->selectFile(QStringLiteral("报警信息.xls"));
    fileDlg->setNameFilter("Excel Files(*.xls *.xlsx)");
    fileDlg->setDefaultSuffix("xls");

    if (fileDlg->exec() == QDialog::Accepted)
    {
        auto fileName = fileDlg->selectedFiles().first();

        // 保存文件添加后缀名
        auto fInfo = QFileInfo(fileName);
        if (fInfo.suffix() != "xls" && fInfo.suffix() != "xlsx")
        {
            fileName += ".xls";
        }

        QXlsx::Document xlsx;

        QXlsx::Format forTitle;/*设置该单元的样式*/
        forTitle.setFontColor(QColor(Qt::white));/*文字为红色*/
        forTitle.setPatternBackgroundColor(QColor(25,25,112));/*背景颜色*/
        forTitle.setFontSize(20);/*设置字体大小*/
        forTitle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);/*横向居中*/
        forTitle.setBorderStyle(QXlsx::Format::BorderThin);/*边框样式*/

        QXlsx::Format forContent = forTitle;
        forContent.setFontColor(QColor(Qt::black));/*文字为红色*/
        forContent.setFillPattern(QXlsx::Format::PatternNone);/*背景颜色*/
        forTitle.setFontSize(18);/*设置字体大小*/

        // 设置excel任务标题
        QStringList lstTitle;
        lstTitle << QStringLiteral("序号") << QStringLiteral("异常时间") << QStringLiteral("报警信息")
                  << QStringLiteral("分公司") << QStringLiteral("供电所") << QStringLiteral("线路")
                  << QStringLiteral("监测点1") << QStringLiteral("监测点2") << QStringLiteral("相别")
                  << QStringLiteral("电流值1")
                  << QStringLiteral("电流值2")
                  << QStringLiteral("报警员")
                  << QStringLiteral("发送时间")
                  << QStringLiteral("推送状态");

        for (int i = 0; i < lstTitle.size(); i++)
        {
            xlsx.write(1, i+1, lstTitle.at(i), forTitle);
            xlsx.setColumnWidth(i+1, 20);
        }

        // 获取表格内容设置excel
        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < lstTitle.count(); j++)
            {
                if (ui->tableWidget->item(i, j) == nullptr) {
                    continue;
                }
                xlsx.write(i+2, j+1 , ui->tableWidget->item(i, j)->text(), forContent);
            }
        }

        // 保存文件
        if (xlsx.saveAs(fileName)) {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导出成功!"));
        }
    }
}
