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
    // 设置属性
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget->setShowGrid(false);//不显示内部的格子线条

    // 设置列名
    QStringList lstHeader;
    lstHeader << QStringLiteral("ID") << QStringLiteral("名字") << QStringLiteral("电话") << QStringLiteral("供电所") << QStringLiteral("备注");
    ui->tableWidget->setHorizontalHeaderLabels( lstHeader );
    ui->tableWidget->setColumnWidth(0, 30);

    // 添加数据
    auto workers = DatabaseProxy::instance().workers();

    // 如果是空直接返回
    if (workers.isEmpty())
        return;

    ui->tableWidget->setRowCount( workers.count() );

    int i = 0;
    foreach (auto usr, workers) {
        auto itemId = new QTableWidgetItem(QString::number(usr.id));
        itemId->setFlags(itemId->flags() & (~Qt::ItemIsEditable));

        ui->tableWidget->setItem(i, 0, itemId);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(usr.name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(usr.phone));

        auto amso = DatabaseProxy::instance().amso(usr.amsoId);
        auto itemAm = new QTableWidgetItem( (nullptr == amso) ? "" : amso->name );
        itemAm->setFlags(itemAm->flags() & (~Qt::ItemIsEditable));
        ui->tableWidget->setItem(i, 3, itemAm);

        ui->tableWidget->setItem(i, 4, new QTableWidgetItem( usr.remarks ));
        i++;
    }
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
            ui->tableWidget->setItem(i, 3, itemAm);

            ui->tableWidget->setItem(i, 4, new QTableWidgetItem( usr->remarks ));
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
