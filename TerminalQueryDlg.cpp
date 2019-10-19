#include "TerminalQueryDlg.h"
#include "ui_TerminalQueryDlg.h"

TerminalQueryDlg::TerminalQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalQueryDlg)
{
    ui->setupUi(this);

    init();
}

TerminalQueryDlg::~TerminalQueryDlg()
{
    delete ui;
}

void TerminalQueryDlg::init()
{
    _company = DatabaseProxy::instance().getOrganizations();

    // init combBox
    QStringList lstSub, lstAmso, lstLine;
    lstSub.append(QStringLiteral("全部"));
    lstAmso.append(QStringLiteral("全部"));
    lstLine.append(QStringLiteral("全部"));
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

    // 设置属性
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableWidget->setShowGrid(false);//不显示内部的格子线条

    // 设置列名
    QStringList lstHeader;
    lstHeader << QStringLiteral("序号")
              << QStringLiteral("公司")
              << QStringLiteral("供电分公司")
              << QStringLiteral("供电所")
              << QStringLiteral("线路")
              << QStringLiteral("集中器")
              << QStringLiteral("线段")
              << QStringLiteral("监测点")
              << QStringLiteral("终端")
              << QStringLiteral("通讯地址");

    ui->tableWidget->setHorizontalHeaderLabels( lstHeader );

    connect(ui->cbSubCompany, SIGNAL(currentIndexChanged(int)), this, SLOT(onSubCompayChanged()));
    connect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    connect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));

    onQuery();
}

int TerminalQueryDlg::getTerminalCount()
{
    int count = 0;
    foreach (auto o1, _company) {
        foreach (auto o2, o1->lst) {
            foreach (auto o3, o2->lst) {
                foreach (auto o4, o3->lst) {
                    foreach (auto o5, o4->lst) {
                        foreach (auto o6, o5->lst) {
                            foreach (auto o7, o6->lst) {
                                count += o7->lst.count();
                            }
                        }
                    }
                }
            }
        }
    }

    return count;
}

void TerminalQueryDlg::onQuery()
{
    ui->tableWidget->setRowCount( getTerminalCount() );

    int i = 0;
    foreach (auto o1, _company) {
        foreach (auto o2, o1->lst) {
            foreach (auto o3, o2->lst) {
                foreach (auto o4, o3->lst) {
                    foreach (auto o5, o4->lst) {
                        foreach (auto o6, o5->lst) {
                            foreach (auto o7, o6->lst) {
                                foreach (auto o8, o7->lst) {
                                    // filter subcompany
                                    if (ui->cbSubCompany->currentIndex() == 0) {
                                        //
                                    } else if (ui->cbSubCompany->currentText() != o2->name) {
                                        continue;
                                    }

                                    // filter amso
                                    if (ui->cbAsmo->currentIndex() == 0) {
                                        //
                                    } else if (ui->cbAsmo->currentText() != o3->name) {
                                        continue;
                                    }

                                    // filter line
                                    if (ui->cbLine->currentIndex() == 0) {
                                        //
                                    } else if (ui->cbLine->currentText() != o4->name) {
                                        continue;
                                    }

                                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
                                    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(o1->name));
                                    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(o2->name));
                                    ui->tableWidget->setItem(i, 3, new QTableWidgetItem(o3->name));
                                    ui->tableWidget->setItem(i, 4, new QTableWidgetItem(o4->name));
                                    ui->tableWidget->setItem(i, 5, new QTableWidgetItem(o5->name));
                                    ui->tableWidget->setItem(i, 6, new QTableWidgetItem(o6->name));
                                    ui->tableWidget->setItem(i, 7, new QTableWidgetItem(o7->name));
                                    ui->tableWidget->setItem(i, 8, new QTableWidgetItem(o8->name));
                                    ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::number(o8->addr)));
                                    i++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ui->tableWidget->setRowCount( i );
}

void TerminalQueryDlg::onSubCompayChanged()
{
    disconnect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    disconnect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));

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

    connect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    connect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    onQuery();
}

void TerminalQueryDlg::onAsmoChanged()
{
    disconnect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));

    QStringList lstLine;
    QList<int> lstAmsoId;
    lstLine.append(QStringLiteral("全部"));
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

    connect(ui->cbLine, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineChanged()));
    onQuery();
}

void TerminalQueryDlg::onLineChanged()
{
    onQuery();
}

void TerminalQueryDlg::onWorkerChanged()
{
    onQuery();
}
#include <QFileDialog>
#include <QMessageBox>
#include "xlsxdocument.h"
void TerminalQueryDlg::on_btnExport_clicked()
{
    if (0 == ui->tableWidget->rowCount()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("没有数据，无法导出!"));
        return;
    }

    // 获取保存文件路径
    auto fileDlg = new QFileDialog(this);
    fileDlg->setWindowTitle(QStringLiteral("保存文件"));
    fileDlg->setAcceptMode(QFileDialog::AcceptSave);
    fileDlg->selectFile(QStringLiteral("终端信息.xls"));
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
        lstTitle << QStringLiteral("序号")
                  << QStringLiteral("公司")
                  << QStringLiteral("供电分公司")
                  << QStringLiteral("供电所")
                  << QStringLiteral("线路")
                  << QStringLiteral("集中器")
                  << QStringLiteral("线段")
                  << QStringLiteral("监测点")
                  << QStringLiteral("终端")
                  << QStringLiteral("通讯地址");
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
