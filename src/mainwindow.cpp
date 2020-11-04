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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lmodel(new QStringListModel())
{
    ui->setupUi(this);
    createUI();
}

MainWindow::~MainWindow()
{
    database.close();
    delete lmodel;
    delete ui;
}

/* Get tables list in curent database */
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
    ui->tableView->setModel(tableMap[active]);
}

void MainWindow::slotTableChanged()
{

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

    //create context menu for the tables list
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCustomMenuRequested(QPoint)));

    //to update views data
    connect(this, SIGNAL(tableListChange()),
            this, SLOT(slotUpdateListView()));
    connect(this, SIGNAL(activeTableChange(QString&)),
            this, SLOT(slotChangeActiveTable(QString&)));
    connect(this, SIGNAL(tableChange()),
            this, SLOT(slotTableChanged()));
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
        else
        {
            tablesList << table;
        }
        emit tableListChange();
    }
}


/* Add new line to model */
void MainWindow::on_actionAdd_new_line_triggered()
{
    AddLine dialog;
    dialog.show();
    dialog.exec();

    QSqlTableModel* model = tableMap[activeTable];
    int row = model->rowCount();
    model->insertRows(row, 1);
    model->setData(model->index(row, 0), dialog.Name());
    model->setData(model->index(row, 1), dialog.Price());
    model->setData(model->index(row, 2), dialog.Weight());
    model->setData(model->index(row, 3), dialog.Date());
    model->setData(model->index(row, 4), dialog.Provider());
    model->setData(model->index(row, 5), dialog.Description());
}

/* Open context menu for listView */
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
    if(query.exec(QString("DROP TABLE " + activeTable)) && tablesList.contains(activeTable))
    {
        //remove table from list and map
        tablesList.removeAll(activeTable);
        delete tableMap[activeTable];
        tableMap.remove(activeTable);

        emit tableListChange();
    }
    else
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

        if(query.exec(QString("RENAME TABLE " + activeTable + " TO " + table)) && tableMap.contains(activeTable))
        {
            //rename table in the list
            int index = tablesList.indexOf(activeTable);
            tablesList[index] = table;

            //rename key in the map
            QSqlTableModel* tmp = tableMap[activeTable];
            tableMap.remove(activeTable);
            tableMap.insert(table, tmp);

            activeTable = table;

            emit tableListChange();
        }
        else
        {
            qDebug() << "Error::" << query.lastError().databaseText();
        }
    }
}
/* Update tables list view */
void MainWindow::slotUpdateListView()
{
    lmodel->setStringList(tablesList);
}

/* Selected other table */
void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QString table = tablesList[index.row()];
    //if table hasn't yet added to map, to add and select data
    if(!tableMap.contains(table))
    {
        tableMap.insert(table, new QSqlTableModel(this, database));
        tableMap[table]->setEditStrategy(QSqlTableModel::OnManualSubmit);
        tableMap[table]->setTable(table);
        tableMap[table]->select();
    }
    emit activeTableChange(table);
}

/* Save changes for active table */
void MainWindow::on_actionSave_triggered()
{
    if(tableMap[activeTable]->submitAll())
    {
        tableMap[activeTable]->database().commit();
    }
}

/* Get information about program */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msg;
    msg.setText("Authors: Dyakov Daniil, Borisov Alexander");
    msg.setButtonText(QMessageBox::Ok, "OK, I understand!");
    msg.exec();
}
