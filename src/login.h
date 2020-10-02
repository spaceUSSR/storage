#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QString>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    QString getUsername();
    QString getPassword();

private slots:
    void on_cancelButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
    QString username;
    QString password;
};

#endif // LOGIN_H
