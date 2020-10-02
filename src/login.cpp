#include "login.h"
#include "ui_login.h"
#include <QDebug>
Login::Login(QSqlDatabase* database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{

    ui->setupUi(this);
    this->database = database;
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
    *database = QSqlDatabase::addDatabase("QMYSQL");
    database->setDatabaseName("test");
    database->setHostName("127.0.0.1");
    database->setUserName(username);
    database->setPassword(password);
    if(database->open())
        this->close();
    else
    {
        ui->errortextLabel->setText("Error!");
    }
}

QSqlDatabase Login::getDatabase()
{
    return *database;
}

QString Login::getPassword()
{
    return password;
}

QString Login::getUsername()
{
    return username;
}
