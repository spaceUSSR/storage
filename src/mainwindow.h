#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QListWidgetItem>
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

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_actionAdd_new_line_triggered();

    void slotCustomMenuRequested(QPoint pos);

    void slotDeleteActiveTable();

    void slotRenameActiveTable();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QSqlDatabase database;
    QStringList tablesList;
    QString activeTable;
private:
    void getTables(); //get tables list for curent database
    void changeActiveTable(QString& active); //change active table for model
    void createUI();
};
#endif // MAINWINDOW_H
