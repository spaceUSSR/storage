#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::Login()
{
    return m_login;
}

QString LoginDialog::Password()
{
    return m_password;
}

void LoginDialog::on_loginLine_textChanged(const QString &arg1)
{
    ui->loginButton->setEnabled(!arg1.isEmpty()
                                && !ui->passwordLine->text().isEmpty());
}

void LoginDialog::on_passwordLine_textChanged(const QString &arg1)
{
    ui->loginButton->setEnabled(!arg1.isEmpty()
                                && !ui->loginLine->text().isEmpty());
}

void LoginDialog::on_loginButton_clicked()
{
    m_login = ui->loginLine->text();
    m_password = ui->passwordLine->text();
    close();
    setResult(Logined);
}

void LoginDialog::on_closeButton_clicked()
{
    close();
    setResult(Aborted);
}
