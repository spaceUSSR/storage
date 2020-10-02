#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_cancelButton_clicked()
{
    this->close();
}

void Login::on_loginButton_clicked()
{
    username = ui->nameEdit->text();
    password = ui->passwordEdit->text();
    this->close();
}

QString Login::getPassword()
{
    return password;
}

QString Login::getUsername()
{
    return username;
}
