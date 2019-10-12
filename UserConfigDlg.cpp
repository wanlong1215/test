#include "UserConfigDlg.h"
#include "ui_UserConfigDlg.h"
#include "DatabaseProxy.h"
#include <QMessageBox>
#include <QTreeWidgetItem>

class BCPerviewTreeWidgetItem : public QTreeWidgetItem
{
public:
    BCPerviewTreeWidgetItem(int type, int value, const QString &text, QTreeWidget *parent);
    BCPerviewTreeWidgetItem(int type, int value, const QString &text, QTreeWidgetItem *parent);

    int m_type;
    int m_value;
};

BCPerviewTreeWidgetItem::BCPerviewTreeWidgetItem(int type, int value, const QString &text, QTreeWidget *parent)
    :QTreeWidgetItem( parent )
{
    m_type = type;
    m_value = value;

    this->setText(0, text);
}

BCPerviewTreeWidgetItem::BCPerviewTreeWidgetItem(int type, int value, const QString &text, QTreeWidgetItem *parent)
    :QTreeWidgetItem( parent )
{
    m_type = type;
    m_value = value;

    this->setText(0, text);
}

// -----------------------------------------------------------------------------------------------------------------------------


UserConfigDlg::UserConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserConfigDlg)
{
    ui->setupUi(this);

    _map.insert(0, QStringLiteral("系统用户"));
    _map.insert(1, QStringLiteral("管理员"));
    _map.insert(2, QStringLiteral("操作员"));

    Init();
}

UserConfigDlg::~UserConfigDlg()
{
    delete ui;
}


void UserConfigDlg::Init()
{
    // 设置属性
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget->setShowGrid(false);//不显示内部的格子线条
    connect(ui->tableWidget, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(onCurrentCellChanged(int,int,int,int)));

    // 设置列名
    QStringList lstHeader;
    lstHeader << QStringLiteral("ID") << QStringLiteral("用户名") << QStringLiteral("密码") << QStringLiteral("权限");
    ui->tableWidget->setHorizontalHeaderLabels( lstHeader );
    ui->tableWidget->setColumnWidth(0, 30);

    // 添加数据
    auto users = DatabaseProxy::instance().users();

    // 如果是空直接返回
    if (users.isEmpty())
        return;

    ui->tableWidget->setRowCount( users.count() );

    int i = 0;
    foreach (auto usr, users) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(usr.id));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(usr.name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(usr.pwd));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem( _map.value(usr.level) ));
        i++;
    }
}

void UserConfigDlg::onCurrentCellChanged(int currentRow, int /*currentColumn*/, int previousRow, int /*previousColumn*/)
{
    if (currentRow == previousRow || -1 == currentRow)
        return;

    QString qsUser = ui->tableWidget->item(currentRow, 1)->text();
    QString qsPsw = ui->tableWidget->item(currentRow, 2)->text();
    QString qsLevel = ui->tableWidget->item(currentRow, 3)->text();
    int level = _map.key( qsLevel );

    ui->userNameEdit->setText( qsUser );
    ui->passwordEdit->setText( qsPsw );
    ui->m_pUserPreviewComboBox->setCurrentIndex( level-1 );
}

void UserConfigDlg::on_removeBut_clicked()
{
   if(ui->tableWidget->currentRow() == -1)
       return;

   int id = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text().toInt();

   if (DatabaseProxy::instance().delUser(id))
   {
       ui->tableWidget->removeRow( ui->tableWidget->currentRow() );
   }
}

void UserConfigDlg::on_addBut_clicked()
{
    if ( ui->userNameEdit->text().isEmpty() ) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("用户名不能为空!"));
        return;
    }

    QString usr = ui->userNameEdit->text();
    QString pwd = ui->passwordEdit->text();

    // 确定添加用户的ID
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QString qsExistUser = ui->tableWidget->item(i, 1)->text();
        if (qsExistUser == usr) {
            QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("用户名已存在!"));
            return;
        }
    }

    int level = ui->m_pUserPreviewComboBox->currentIndex()+1;
    int id = DatabaseProxy::instance().addUser(usr, pwd, level);
    if (-1 != id)
    {
        // 添加界面
        int nRowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(nRowCount+1);

        ui->tableWidget->setItem(nRowCount, 0, new QTableWidgetItem( QString::number(id) ));
        ui->tableWidget->setItem(nRowCount, 1, new QTableWidgetItem( usr ));
        ui->tableWidget->setItem(nRowCount, 2, new QTableWidgetItem( pwd ));
        ui->tableWidget->setItem(nRowCount, 3, new QTableWidgetItem( ui->m_pUserPreviewComboBox->currentText() ));
    }
}

void UserConfigDlg::on_m_pModifyBtn_clicked()
{
    if(ui->tableWidget->currentRow() == -1) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选中修改的用户!"));
        return;
    }

    if ( ui->userNameEdit->text().isEmpty() ) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("用户名不能为空!"));
        return;
    }

    QString usr = ui->userNameEdit->text();
    QString pwd = ui->passwordEdit->text();
    QString level = ui->m_pUserPreviewComboBox->currentText();

    int id = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text().toInt();

    if (DatabaseProxy::instance().modifyUser(id, usr, pwd, _map.key(level)))
    {
        ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->setText( usr );
        ui->tableWidget->item(ui->tableWidget->currentRow(), 2)->setText( pwd );
        ui->tableWidget->item(ui->tableWidget->currentRow(), 3)->setText( level );
    }
}
