#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QString>

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

//    void on_listView_itemClicked(QListWidgetItem *item);

    void on_actionAdd_new_line_triggered();

    void slotCustomMenuRequested(QPoint pos);

    void slotDeleteActiveTable();

    void slotRenameActiveTable();

    void slotUpdateViews();

    void on_listView_clicked(const QModelIndex &index);

    void slotChangeActiveTable(QString&); //change active table for model
signals:
    void databaseChanged();

    void activeTableChanged(QString&);

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QStringListModel* lmodel;
    QSqlDatabase database;
    QStringList tablesList;
    QString activeTable;
private:
    QStringList getTablesList(); //get tables list for curent database
    void createUI();
};
#endif // MAINWINDOW_H
