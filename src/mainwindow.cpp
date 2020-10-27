#include "mainwindow.h"
#include "newtable.h"
#include "addline.h"
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

    model = new QSqlTableModel(this, database);
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

    while(query.next())
    {
        QString table = query.value(0).toString();
        tablesList << table;
    }
}

void MainWindow::changeActiveTable(QString& active)
{
    activeTable = active;
    model->setTable(activeTable);
    model->select();
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
    newtable ntWindow(&database);
    ntWindow.show();
    ntWindow.exec();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString table = item->text();
    changeActiveTable(table);
}

void MainWindow::on_actionAdd_new_line_triggered()
{
    AddLine addlineWindow;
    addlineWindow.show();
    addlineWindow.exec();
}
