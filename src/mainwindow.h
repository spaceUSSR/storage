#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
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

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    bool opened = false;
    QSqlDatabase database;
    QStringList tablesList;

private:
    void getTables();
};
#endif // MAINWINDOW_H
