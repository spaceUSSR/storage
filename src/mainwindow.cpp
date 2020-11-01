#include "mainwindow.h"
#include "InpuDialog.h"
#include "addline.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createUI();
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

void MainWindow::createUI()
{
    //open database
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("127.0.0.1");
    database.setDatabaseName("wms");
    database.setUserName("root");
    database.setPassword("cvjktycrjt22");
    if(!database.open())
        qDebug() << database.lastError();

    //init the tables list
    getTables();
    ui->listWidget->addItems(tablesList);

    model = new QSqlTableModel(this, database);
    ui->tableView->setModel(model);

    //create context menu for the tables list
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequested(QPoint)));
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
    InputDialog dialog("Название новой таблицы:", QRegExp("^[a-zA-Z0-9]+$"));
    dialog.show();
    if(dialog.exec() == QDialog::Accepted)
    {
        QString table = dialog.getText();
        QSqlQuery query(database);
        QString querystr = "CREATE TABLE " + table +
                " ("
                "Name VARCHAR(30), "
                "Price INT, "
                "Weight INT, "
                "Data VARCHAR(11),"
                "Provider VARCHAR(30), "
                "Description TEXT "
                ")";
        if(!query.exec(querystr))
        {
            qDebug() << "Error::" << query.lastError().databaseText();
        }
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString table = item->text();
    changeActiveTable(table);
}

void MainWindow::on_actionAdd_new_line_triggered()
{
    AddLine addlineWindow(database, activeTable);
    addlineWindow.show();
    addlineWindow.exec();
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    QMenu* menu = new QMenu(this);

    QAction* deleteTable = new QAction("Удалить", this);
    connect(deleteTable, SIGNAL(triggered()), this, SLOT(slotDeleteActiveTable()));

    QAction* renameTable = new QAction("Переименовать", this);
    connect(renameTable, SIGNAL(triggered()), this, SLOT(slotRenameActiveTable()));

    QAction* addTable = new QAction("Добавить", this);
    connect(addTable, SIGNAL(triggered()), this, SLOT(on_actionNew_table_triggered()));

    menu->addAction(addTable);
    menu->addAction(deleteTable);
    menu->addAction(renameTable);

    menu->popup(ui->listWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::slotDeleteActiveTable()
{
    QSqlQuery query(database);
    if(!query.exec(QString("DROP TABLE " + activeTable)))
    {
        qDebug() << query.lastError().text();
    }
}

void MainWindow::slotRenameActiveTable()
{
    InputDialog dialog("Новое название таблицы:", QRegExp("^[a-zA-Z0-9]+$"));
    dialog.show();
    if(dialog.exec() == QDialog::Accepted)
    {
        QString table = dialog.getText();
        QSqlQuery query(database);
        if(!query.exec(QString("RENAME TABLE " + activeTable + " TO " + table)))
        {
            qDebug() << "Error::" << query.lastError().databaseText();
        }
    }
}

