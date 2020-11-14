#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString Login();
    QString Password();
    enum
    {
        Aborted,
        Logined
    };
private slots:
    void on_loginLine_textChanged(const QString &arg1);

    void on_passwordLine_textChanged(const QString &arg1);

    void on_loginButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString m_login;
    QString m_password;
};

#endif // LOGINDIALOG_H
