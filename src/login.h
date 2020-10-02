#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QSqlDatabase>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QSqlDatabase* database, QWidget *parent = nullptr);
    ~Login();
    QString getUsername();
    QString getPassword();

    QSqlDatabase getDatabase();

private slots:
    void on_cancelButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
    QString username;
    QString password;
    QSqlDatabase* database;
};

#endif // LOGIN_H
