#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AddTerminalDlg.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AddTerminalDlg *p = new AddTerminalDlg(1, 2, this);
    p->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
