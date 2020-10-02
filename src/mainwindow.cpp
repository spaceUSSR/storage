#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "login.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableView);

    Login loginWin(&database);
    loginWin.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    loginWin.show();
    loginWin.exec();

    QSqlQuery query = QSqlQuery(database);
    if(!query.exec("select * from goods"))
    {
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().driverText();
        return;
    }

    model = new QSqlTableModel(this, database);
    model->setTable("goods");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->showColumn(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openDatabase(QString userName, QString password, QString hostName, QString dbName)
{
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(hostName);
    database.setDatabaseName(dbName);
    database.setUserName(userName);
    database.setPassword(password);
    opened = database.open();
    if(!opened)
        qDebug() << database.lastError();
}
