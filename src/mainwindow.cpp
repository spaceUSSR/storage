#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "EditDialog.h"
#include "InputDialog.h"
#include "logindialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QFile>

#define DB_CONNECTION_ADRES "127.0.0.1"

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

bool MainWindow::isOpened()
{
    return opened;
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
    QSqlTableModel* model = tableMap[active];

    ui->actionEdit->setEnabled(model->rowCount());
    ui->actionRemove->setEnabled(model->rowCount());
    ui->actionAdd_new_line->setEnabled(true);
    ui->tableView->setModel(model);

}


void MainWindow::createUI()
{
    //open database
    if(!(opened = openConnection()))
        qDebug() << database.lastError();

    //init the tables list
    tablesList = tables();
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

    //Default disable action
    ui->actionEdit->setEnabled(false);
    ui->actionAdd_new_line->setEnabled(false);
    ui->actionRemove->setEnabled(false);
}
/* Open connection with reading of login and password from logfile,
 * if it exists, or  from login dialog */
bool MainWindow::openConnection()
{

    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(DB_CONNECTION_ADRES);
    database.setDatabaseName("wms");

    QString login;
    QString password;

    QFile logFile("wms.log");

    //Open exists connection logfile
    if(logFile.exists())
    {
        if(logFile.open(QFile::ReadOnly | QFile::Text))
        {

          login = static_cast<QString>(logFile.readLine()).remove('\n');
          password = static_cast<QString>(logFile.readLine()).remove('\n');
          logFile.close();

        }
    }
    else
    {
        //if there wasn't connection
        LoginDialog dialog;
        dialog.show();

        switch(dialog.exec())
        {
        case LoginDialog::Logined:
            login = dialog.Login();
            password = dialog.Password();
            break;

            default:
                break;
        }
    }

    bool opened = database.open(login, password);

    if(opened)
    {

        //Create last connection log file
        if(logFile.open(QFile::WriteOnly | QIODevice::Text))
        {
            QTextStream fout(&logFile);
            fout << "Login: " << login << "\n";
            fout << "Password: " << password << "\n";
        }
    }
        return opened;
    }

QStringList MainWindow::tables()
{
    QSqlQuery query("show tables", database);
    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}


void MainWindow::on_actionNew_table_triggered()
{
    InputDialog dialog("Название новой таблицы:", QRegExp("^[a-zA-Z0-9]+$"));
    dialog.show();

    if(dialog.exec() == QDialog::Accepted)
    {
        QString table = dialog.getText();
        QSqlQuery query(database);
        QString querystr = "create table %1 ( Название VARCHAR(30), Цена INT, Вес INT, Дата DATE, Поставщик VARCHAR(30), Описание TEXT)";
        querystr.arg(table);

        if(!query.exec(querystr))
        {
            qDebug() << "Error::" << query.lastError().databaseText();
        }
        else
        {
            tablesList << table;
            renameTable->setEnabled(true);
            deleteTable->setEnabled(true);
        }
        emit tableListChange();
    }
}


/* Add new line to model */
void MainWindow::on_actionAdd_new_line_triggered()
{
    EditDialog dialog;
    dialog.show();
    switch(dialog.exec())
    {
    case QDialog::Accepted:
    {
        QSqlTableModel* model = tableMap[activeTable];
        int row = model->rowCount();
        model->insertRows(row, 1);
        model->setData(model->index(row, 0), dialog.Name());
        model->setData(model->index(row, 1), dialog.Price());
        model->setData(model->index(row, 2), dialog.Weight());
        model->setData(model->index(row, 3), dialog.Date());
        model->setData(model->index(row, 4), dialog.Provider());
        model->setData(model->index(row, 5), dialog.Description());

        //Lines is more than 0, it allow to delete or edit line
        ui->actionEdit->setEnabled(true);
        ui->actionRemove->setEnabled(true);
        break;
    }

    default:
        break;
    }
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

        //Default disable action
        ui->actionEdit->setEnabled(false);
        ui->actionAdd_new_line->setEnabled(false);
        ui->actionRemove->setEnabled(false);

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
        QSqlTableModel* model= tableMap[activeTable];
        model->removeRows(active.row(), 1);

        //Disable actions if lines is less then 1
        ui->actionEdit->setEnabled(model->rowCount());
        ui->actionRemove->setEnabled(model->rowCount());
    }
}

void MainWindow::on_actionrevert_triggered()
{
    tableMap[activeTable]->revertAll();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
