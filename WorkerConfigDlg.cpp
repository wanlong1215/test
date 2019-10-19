#include "WorkerConfigDlg.h"
#include "ui_WorkerConfigDlg.h"
#include "DatabaseProxy.h"
#include "AddWorkerDlg.h"
#include <QMessageBox>

WorkerConfigDlg::WorkerConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkerConfigDlg)
{
    ui->setupUi(this);

    init();
}

WorkerConfigDlg::~WorkerConfigDlg()
{
    delete ui;
}

void WorkerConfigDlg::init()
{
    _company = DatabaseProxy::instance().getOrganizations();

    // init combBox
    QStringList lstSub, lstAmso;
    lstSub.append(QStringLiteral("全部"));
    lstAmso.append(QStringLiteral("全部"));
    auto companys = _company;
    foreach (auto o1, companys) {
        foreach (auto o2, o1->lst) {
            lstSub.append(o2->name);
            foreach (auto o3, o2->lst) {
                lstAmso.append(o3->name);
            }
        }
    }
    ui->cbSubCompany->addItems(lstSub);
    ui->cbAsmo->addItems(lstAmso);

    // 设置属性
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget->setShowGrid(false);//不显示内部的格子线条

    // 设置列名
    QStringList lstHeader;
    lstHeader << QStringLiteral("ID") << QStringLiteral("名字") << QStringLiteral("电话") << QStringLiteral("分公司") << QStringLiteral("供电所") << QStringLiteral("备注");
    ui->tableWidget->setHorizontalHeaderLabels( lstHeader );
    ui->tableWidget->setColumnWidth(0, 30);

    connect(ui->cbSubCompany, SIGNAL(currentIndexChanged(int)), this, SLOT(onSubCompayChanged()));
    connect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    onQuery();
}

void WorkerConfigDlg::on_btnAdd_clicked()
{
    AddWorkerDlg *dlg = new AddWorkerDlg(this);
    if (dlg->exec() == QDialog::Accepted)
    {
        auto usr = dlg->worker();
        if (nullptr != usr)
        {

            // 添加界面
            int i = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(i+1);

            auto itemId = new QTableWidgetItem(QString::number(usr->id));
            itemId->setFlags(itemId->flags() & (~Qt::ItemIsEditable));

            ui->tableWidget->setItem(i, 0, itemId);
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(usr->name));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(usr->phone));

            auto amso = DatabaseProxy::instance().amso(usr->amsoId);
            auto itemAm = new QTableWidgetItem( (nullptr == amso) ? "" : amso->name );
            itemAm->setFlags(itemAm->flags() & (~Qt::ItemIsEditable));
            ui->tableWidget->setItem(i, 4, itemAm);

            auto itemSub = new QTableWidgetItem( (nullptr == amso) ? "" : amso->parent->name );
            itemSub->setFlags(itemAm->flags() & (~Qt::ItemIsEditable));
            ui->tableWidget->setItem(i, 3, itemSub);

            ui->tableWidget->setItem(i, 5, new QTableWidgetItem( usr->remarks ));
        }
    }
}

void WorkerConfigDlg::on_btnModify_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (-1 == row)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选中要修改的接警员!"));
    }

    proWorker worker;
    if (!DatabaseProxy::instance().worker(ui->tableWidget->item(row, 0)->text().toInt(), worker))
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("查无此人，当前用户可能已经被其他管理员删除，请刷新数据后再试!"));
    }

    AddWorkerDlg *dlg = new AddWorkerDlg(&worker, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        auto usr = dlg->worker();
        if (nullptr != usr)
        {
            ui->tableWidget->item(row, 1)->setText(worker.name);
            ui->tableWidget->item(row, 2)->setText(worker.phone);
            auto amso = DatabaseProxy::instance().amso(worker.amsoId);
            ui->tableWidget->item(row, 3)->setText((nullptr == amso) ? "" : amso->name);
            ui->tableWidget->item(row, 4)->setText(worker.remarks);
        }
    }
}

void WorkerConfigDlg::on_btnDelete_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (-1 == row)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选中要删除的接警员!"));
    }

    auto id = ui->tableWidget->item(row, 0)->text().toInt();
    if (DatabaseProxy::instance().delWorker(id))
    {
        ui->tableWidget->removeRow(row);
    }
}

void WorkerConfigDlg::on_leFilter_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        auto item = ui->tableWidget->item(i, 1);
        if (nullptr != item)
        {
            ui->tableWidget->setRowHidden(i, !item->text().contains(arg1));
        }
    }
}

void WorkerConfigDlg::onQuery()
{
    // 添加数据
    auto workers = DatabaseProxy::instance().workers();

    // 如果是空直接返回
    if (workers.isEmpty())
        return;

    ui->tableWidget->setRowCount( workers.count() );

    int i = 0;
    foreach (auto usr, workers) {
        auto amso = DatabaseProxy::instance().amso(usr.amsoId);
        if (nullptr != amso) {
            // filter subcompany
            if (ui->cbSubCompany->currentIndex() == 0) {
                //
            } else if (ui->cbSubCompany->currentText() != amso->parent->name) {
                continue;
            }

            // filter amso
            if (ui->cbAsmo->currentIndex() == 0) {
                //
            } else if (ui->cbAsmo->currentText() != amso->name) {
                continue;
            }
        } else {
            //
        }

        auto itemId = new QTableWidgetItem(QString::number(usr.id));
        itemId->setFlags(itemId->flags() & (~Qt::ItemIsEditable));

        ui->tableWidget->setItem(i, 0, itemId);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(usr.name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(usr.phone));

        auto itemAm = new QTableWidgetItem( (nullptr == amso) ? "" : amso->name );
        itemAm->setFlags(itemAm->flags() & (~Qt::ItemIsEditable));
        ui->tableWidget->setItem(i, 3, itemAm);

        ui->tableWidget->setItem(i, 4, new QTableWidgetItem( usr.remarks ));
        i++;
    }

    ui->tableWidget->setRowCount( i );
}


void WorkerConfigDlg::onSubCompayChanged()
{
    disconnect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));

    QStringList lstAmso;
    QList<int> lstAmsoId;
    lstAmso.append(QStringLiteral("全部"));
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
                lstAmso.append(o3->name);
            }
        }
    }
    ui->cbAsmo->clear();
    ui->cbAsmo->addItems(lstAmso);

    connect(ui->cbAsmo, SIGNAL(currentIndexChanged(int)), this, SLOT(onAsmoChanged()));
    onQuery();
}

void WorkerConfigDlg::onAsmoChanged()
{
    onQuery();
}
#include <QFileDialog>
#include "xlsxdocument.h"
void WorkerConfigDlg::on_btnExport_clicked()
{
    if (0 == ui->tableWidget->rowCount()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("没有数据，无法导出!"));
        return;
    }

    // 获取保存文件路径
    auto fileDlg = new QFileDialog(this);
    fileDlg->setWindowTitle(QStringLiteral("保存文件"));
    fileDlg->setAcceptMode(QFileDialog::AcceptSave);
    fileDlg->selectFile(QStringLiteral("接警人员.xls"));
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
        lstTitle << QStringLiteral("ID") << QStringLiteral("名字") << QStringLiteral("电话") << QStringLiteral("分公司") << QStringLiteral("供电所") << QStringLiteral("备注");

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
