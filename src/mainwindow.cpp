#include "mainwindow.h"
#include "newtable.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("127.0.0.1");
    database.setDatabaseName("wms");
    database.setUserName("root");
    database.setPassword("cvjktycrjt22");


    if(!database.open())
        qDebug() << database.lastError();

    getTables();
    ui->listWidget->addItems(tablesList);

    QSqlQuery query = QSqlQuery(database);
    if(!query.exec("select * from " + tablesList[0]))
    {
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().driverText();
        return;
    }
    model = new QSqlTableModel(this, database);
    model->setTable("product");
    model->select();

    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}

void MainWindow::getTables()
{
    QSqlQuery query = QSqlQuery(database);
    if(!query.exec("SHOW TABLES;"))
    {
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().driverText();
        return;
    }

    for (int i = 0; query.next(); i++)
    {
        QString table = query.value(i).toString();
        tablesList << table;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msg;
    msg.setText("Authors: Dyakov Daniil, Borisov Alexander");
    msg.setButtonText(QMessageBox::Ok, "OK, I understand!");
    msg.exec();
}

void MainWindow::on_actionNew_table_triggered()
{
    newtable ntWindow;
    ntWindow.show();
    ntWindow.exec();
}
