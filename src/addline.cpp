#include "addline.h"
#include "ui_addline.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

//#define INSERT_TO(table_name, values) ("INSERT " + (table_name) + " VALUES ( " + (values) + " );")
//#define INSERT_STRING(string) (" '" + (string) + "' ")
//#define INSERT_INT(num) (" " + (num) + " ")

AddLine::AddLine(/*QSqlDatabase& db, QString& table, */QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLine),
    changed(false)
{
//    this->db = db;
//    this->table = table;
    ui->setupUi(this);
}

AddLine::~AddLine()
{
    delete ui;
}

QString AddLine::Name()
{
    return name;
}

int AddLine::Price()
{
    return price;
}

int AddLine::Weight()
{
    return weight;
}

QDate AddLine::Date()
{
    return date;
}

QString AddLine::Provider()
{
    return provider;
}

QString AddLine::Description()
{
    return description;
}

void AddLine::on_nameEdit_textChanged(const QString &arg1)
{
    ui->okButton->setEnabled(!arg1.isEmpty() & !ui->providerEdit->text().isEmpty());
    changed = true;
}

void AddLine::on_providerEdit_textChanged(const QString &arg1)
{
    ui->okButton->setEnabled(!arg1.isEmpty() & !ui->nameEdit->text().isEmpty());
    changed = true;
}

void AddLine::closeEvent(QCloseEvent *event)
{
    (void)event;
    if(changed)
    {
        QMessageBox msg(QMessageBox::Question, "wms", "Вы уверены, что хотите закрыть?", QMessageBox::Yes | QMessageBox::No);
        msg.show();
        switch (msg.exec()) {
        case QMessageBox::Yes:
            close();
            break;
        case QMessageBox::No:
            event->ignore();
        default:
            break;
        }
    }
}

void AddLine::on_okButton_clicked()
{

    name = ui->nameEdit->text();
    price = ui->priceSpinBox->value();
    weight = ui->weightSpinBox->value();
    date = ui->dateEdit->date();
    provider = ui->providerEdit->text();
    description = ui->plainTextEdit->toPlainText();
//    QString queryStr = INSERT_TO(table, INSERT_STRING(ui->nameEdit->text()) + ", "
//                                      + INSERT_INT(ui->priceSpinBox->text()) + ", "
//                                      + INSERT_INT(ui->weightSpinBox->text()) + ", "
//                                      + INSERT_STRING(ui->dateEdit->text()) + ", "
//                                      + INSERT_STRING(ui->providerEdit->text()) + ", "
//                                      + INSERT_STRING(ui->plainTextEdit->toPlainText()));
//    QSqlQuery query(db);
//    if(!query.exec(queryStr))
//    {
//        qDebug() << query.lastError().text();
//    }
    close();
}
