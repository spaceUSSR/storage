#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "EditDialog.h"
#include "InpuDialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QAction>

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
    delete ui;
}

/* Get tables list in curent database */
QStringList MainWindow::getTablesList()
{
//    QSqlQuery query = QSqlQuery(database);
    QStringList list;
//    if(!query.exec("\\dt+;"))
//    {
//        qDebug() << "Get tables error::" << query.lastError().databaseText();
//        qDebug() << query.lastError().driverText();
//    }
//    while(query.next())
//    {
//        list << query.value(0).toString();
//    }
    list = database.tables();
//    qDebug() << list;
    return list;
}

void MainWindow::setDataToDialog(QSqlTableModel* model, EditDialog * dialog, QModelIndex& index)
{
    int selectedRow = index.row();
    dialog->setName(model->data(model->index(selectedRow, 0)).toString());
    dialog->setPrice(model->data(model->index(selectedRow, 1)).toInt());
    dialog->setWeight(model->data(model->index(selectedRow, 2)).toInt());
    dialog->setDate(model->data(model->index(selectedRow, 3)).toDate());
    dialog->setProveder(model->data(model->index(selectedRow, 4)).toString());
    dialog->setDescription(model->data(model->index(selectedRow, 5)).toString());
}

void MainWindow::getDataFromDialog(QSqlTableModel* model, EditDialog * dialog, QModelIndex& index)
{
    int selectedRow = index.row();
    model->setData(model->index(selectedRow, 0), dialog->Name());
    model->setData(model->index(selectedRow, 1), dialog->Price());
    model->setData(model->index(selectedRow, 2), dialog->Weight());
    model->setData(model->index(selectedRow, 3), dialog->Date());
    model->setData(model->index(selectedRow, 4), dialog->Provider());
    model->setData(model->index(selectedRow, 5), dialog->Description());
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
    database = QSqlDatabase::addDatabase("QPSQL");
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


    //create list view context menu action
    deleteTable = new QAction("Удалить таблицу", this);
    deleteTable->setEnabled(tablesList.length());
    connect(deleteTable, SIGNAL(triggered()), this, SLOT(slotDeleteActiveTable()));

   renameTable = new QAction("Переименовать", this);
   connect(renameTable, SIGNAL(triggered()), this, SLOT(slotRenameActiveTable()));
   renameTable->setEnabled(tablesList.length());

    addTable = new QAction("Добавить", this);
    connect(addTable, SIGNAL(triggered()), this, SLOT(on_actionNew_table_triggered()));

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
                "Название VARCHAR(30), "
                "Цена INT, "
                "Вес INT, "
                "Дата DATE,"
                "Поставщик VARCHAR(30), "
                "Описание TEXT "
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
    EditDialog dialog;
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

        if(query.exec(QString("ALTER TABLE " + activeTable + " RENAME TO " + table)) && tableMap.contains(activeTable))
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
    if(!tableMap[activeTable]->submitAll())
    {
        qDebug() << tableMap[activeTable]->lastError().text();
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

void MainWindow::on_actionEdit_triggered()
{
    EditDialog dialog;
    QSqlTableModel* model = tableMap[activeTable];
    setDataToDialog(model, &dialog, active);

    dialog.show();
    if(dialog.exec() == QDialog::Accepted)
    {
        getDataFromDialog(model, &dialog, active);
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    active = index;
}

void MainWindow::on_actionRemove_triggered()
{
    QMessageBox msg(QMessageBox::Question, "Удаление",
                    QString("Вы уверены, что хотите удалить строку: " + QString::number(active.row() + 1)),
                    QMessageBox::Yes | QMessageBox::No);
    msg.show();
    if(msg.exec() == QMessageBox::Yes)
    {
        tableMap[activeTable]->removeRows(active.row(), 1);
    }
}

void MainWindow::on_actionrevert_triggered()
{
    tableMap[activeTable]->revertAll();
}
