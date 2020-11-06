#include "addline.h"
#include "ui_addline.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QPushButton>

AddLine::AddLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLine),
    changed(false)
{
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
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

void AddLine::setName(QString name)
{
    ui->nameEdit->setText(name);
}

void AddLine::setPrice(int price)
{
    ui->priceSpinBox->setValue(price);
}

void AddLine::setWeight(int weight)
{
    ui->weightSpinBox->setValue(weight);
}

void AddLine::setDate(QDate date)
{
    ui->dateEdit->setDate(date);
}

void AddLine::setProveder(QString provider)
{
    ui->providerEdit->setText(provider);
}

void AddLine::setDescription(QString description)
{
    ui->plainTextEdit->setPlainText(description);
}

void AddLine::on_nameEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!arg1.isEmpty() & !ui->providerEdit->text().isEmpty());
    changed = true;
}

void AddLine::on_providerEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!arg1.isEmpty() & !ui->nameEdit->text().isEmpty());
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

void AddLine::accept()
{
    name = ui->nameEdit->text();
    price = ui->priceSpinBox->value();
    weight = ui->weightSpinBox->value();
    date = ui->dateEdit->date();
    provider = ui->providerEdit->text();
    description = ui->plainTextEdit->toPlainText();
    changed = false;
    close();
}

void AddLine::reject()
{
      close();
}
