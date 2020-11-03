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
    , lmodel(new QStringListModel(this))
{
    ui->setupUi(this);
    createUI();
}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}

QStringList MainWindow::getTablesList()
{
    QSqlQuery query = QSqlQuery(database);
    QStringList list;
    if(!query.exec("SHOW TABLES;"))
    {
        qDebug() << query.lastError().databaseText();
        qDebug() << query.lastError().driverText();
    }
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}

void MainWindow::slotChangeActiveTable(QString& active)
{
    activeTable = active;
    model->setTable(activeTable);
    model->select();
}

void MainWindow::slotTableChanged()
{
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
    tablesList = getTablesList();
    lmodel->setStringList(tablesList);
    ui->listView->setModel(lmodel);

    model = new QSqlTableModel(this, database);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    //create context menu for the tables list
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequested(QPoint)));

    //to update views data
    connect(this, SIGNAL(databaseChange()),
            this, SLOT(slotUpdateViews()));
    connect(this, SIGNAL(activeTableChange(QString&)),
            this, SLOT(slotChangeActiveTable(QString&)));
    connect(this, SIGNAL(tableChange()),
            this, SLOT(slotTableChanged()));
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
        emit databaseChange();
    }
}

void MainWindow::on_actionAdd_new_line_triggered()
{
    AddLine addlineWindow(database, activeTable);
    addlineWindow.show();
    addlineWindow.exec();
    emit tableChange();
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

    menu->popup(ui->listView->viewport()->mapToGlobal(pos));
}

void MainWindow::slotDeleteActiveTable()
{
    QSqlQuery query(database);
    if(!query.exec(QString("DROP TABLE " + activeTable)))
    {
        qDebug() << query.lastError().text();
    }
    emit databaseChange();
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
        emit databaseChange();
    }
}

void MainWindow::slotUpdateViews()
{
//    model->select();
    tablesList = getTablesList();
    lmodel->setStringList(tablesList);
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QString table = tablesList.at(index.row());
    emit activeTableChange(table);
}

/* Save changes for active table */
void MainWindow::on_actionSave_triggered()
{
    if(model->submitAll())
    {
        model->database().commit();
    }
}
