#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QList>
#include <QString>
#include <QMap>

class EditDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionAbout_triggered();

    void on_actionNew_table_triggered();

    void on_actionAdd_new_line_triggered();

    void slotCustomMenuRequested(QPoint pos);

    void slotDeleteActiveTable();

    void slotRenameActiveTable();

    void slotUpdateListView();

    void on_listView_clicked(const QModelIndex &index);

    void slotChangeActiveTable(QString&); //change active table for model

    void slotTableChanged();


    void on_actionSave_triggered();

    void on_actionEdit_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void on_actionRemove_triggered();

    void on_actionrevert_triggered();

signals:
    void tableListChange(); //In database was added/deleted/renamed table

    void activeTableChange(QString&);

    void tableChange();    //In active table was added/deleted/reanamed line

private:
    Ui::MainWindow *ui;
    QStringListModel* lmodel;
    QSqlDatabase database;
    QStringList tablesList;
    QString activeTable;
    QMap<QString, QSqlTableModel*> tableMap; // key- table name, value - table model

    QModelIndex active;
    QAction* deleteTable;
    QAction* renameTable;
    QAction* addTable;

private:
    QStringList getTablesList(); //get tables list for curent database
    void setDataToDialog(QSqlTableModel* model, EditDialog * dialog, QModelIndex& index);
    void getDataFromDialog(QSqlTableModel* model, EditDialog * dialog, QModelIndex& index);
    void createUI();
};
#endif // MAINWINDOW_H
