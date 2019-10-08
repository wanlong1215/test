#include "BasicEditDlg.h"
#include "ui_BasicEditDlg.h"

BasicEditDlg::BasicEditDlg(int addType, int pid, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicEditDlg)
{
    ui->setupUi(this);

    setWindowTitle("添加");
    _type = addType;
    _parentId = pid;
    switch (_type)
    {
    case 1:
        ui->leName->setText("公司");
        break;
    case 2:
        ui->leName->setText("分公司");
        break;
    case 3:
        ui->leName->setText("供电所");
        break;
    case 4:
        ui->leName->setText("线路");
        break;
    default:
        break;
    }

    _o1 = NULL;
    _o2 = NULL;
    _o3 = NULL;
    _o4 = NULL;
}

BasicEditDlg::BasicEditDlg(proCompany *o, QWidget *parent) :
    QDialog(parent), _type(0),
    ui(new Ui::BasicEditDlg)
{
    ui->setupUi(this);

    _o1 = o;
    _o2 = NULL;
    _o3 = NULL;
    _o4 = NULL;
    ui->leName->setText(o->name);
    ui->teDesc->setText(o->desc);
}

BasicEditDlg::BasicEditDlg(proSubCompany *o, QWidget *parent) :
    QDialog(parent), _type(0),
    ui(new Ui::BasicEditDlg)
{
    ui->setupUi(this);

    _o1 = NULL;
    _o2 = o;
    _o3 = NULL;
    _o4 = NULL;
    ui->leName->setText(o->name);
    ui->teDesc->setText(o->desc);
}

BasicEditDlg::BasicEditDlg(proAmso *o, QWidget *parent) :
    QDialog(parent), _type(0),
    ui(new Ui::BasicEditDlg)
{
    ui->setupUi(this);

    _o1 = NULL;
    _o2 = NULL;
    _o3 = o;
    _o4 = NULL;
    ui->leName->setText(o->name);
    ui->teDesc->setText(o->desc);
}

BasicEditDlg::BasicEditDlg(proRoute *o, QWidget *parent) :
    QDialog(parent), _type(0),
    ui(new Ui::BasicEditDlg)
{
    ui->setupUi(this);

    _o1 = NULL;
    _o2 = NULL;
    _o3 = NULL;
    _o4 = o;
    ui->leName->setText(o->name);
    ui->teDesc->setText(o->desc);
}

BasicEditDlg::~BasicEditDlg()
{
    delete ui;
}

proCompany *BasicEditDlg::company()
{
    return _o1;
}

proSubCompany *BasicEditDlg::subCompany()
{
    return _o2;
}

proAmso *BasicEditDlg::amso()
{
    return _o3;
}

proRoute *BasicEditDlg::route()
{
    return _o4;
}

QString BasicEditDlg::name()
{
    return ui->leName->text();
}

void BasicEditDlg::on_pushButton_clicked()
{
    switch (_type)
    {
    case 1:
        _o1 = new proCompany();
        break;
    case 2:
        _o2 = new proSubCompany(DatabaseProxy::instance().company(_parentId));
        break;
    case 3:
        _o3 = new proAmso(DatabaseProxy::instance().subCompany(_parentId));
        break;
    case 4:
        _o4 = new proRoute(DatabaseProxy::instance().amso(_parentId));
        break;
    default:
        break;
    }

    if (NULL != _o1)
    {
        _o1->name = ui->leName->text();
        _o1->desc = ui->teDesc->toPlainText();
    }
    if (NULL != _o2)
    {
        _o2->name = ui->leName->text();
        _o2->desc = ui->teDesc->toPlainText();
    }
    if (NULL != _o3)
    {
        _o3->name = ui->leName->text();
        _o3->desc = ui->teDesc->toPlainText();
    }
    if (NULL != _o4)
    {
        _o4->name = ui->leName->text();
        _o4->desc = ui->teDesc->toPlainText();
    }

    accept();
}

void BasicEditDlg::on_pushButton_2_clicked()
{
    reject();
}
