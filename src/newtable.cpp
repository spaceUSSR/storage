#include "newtable.h"
#include "ui_newtable.h"
#include <QSqlDatabase>
#include <QRegExpValidator>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

newtable::newtable(QSqlDatabase *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newtable)
{
    this->db = db;
    ui->setupUi(this);
    QValidator *validator  = new QRegularExpressionValidator(QRegularExpression("^[a-zA-Z0-9]+$"), this);
    ui->lineEdit->setValidator(validator);
}

newtable::~newtable()
{
    delete ui;
}

void newtable::on_lineEdit_textChanged(const QString &arg1)
{
     ui->okButton->setEnabled(!arg1.isEmpty());
}

void newtable::on_okButton_clicked()
{
    QSqlQuery query(*db);
    QString querystr = "CREATE TABLE " + ui->lineEdit->text() +
            " ("
            "Name VARCHAR(30), "
            "Price INT, "
            "Weight INT, "
            "Data VARCHAR(11),"
            "Provider VARCHAR(30), "
            "Description TEXT "
            ")";
    if(!query.exec(querystr))
    {
        qDebug() << "Error::" << query.lastError().databaseText();
    }
    close();
}
