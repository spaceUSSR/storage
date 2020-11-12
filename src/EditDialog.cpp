#include "EditDialog.h"
#include "ui_EditDialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QPushButton>

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog),
    changed(false)
{
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

EditDialog::~EditDialog()
{
    delete ui;
}

QString EditDialog::Name()
{
    return name;
}

int EditDialog::Price()
{
    return price;
}

int EditDialog::Weight()
{
    return weight;
}

QDate EditDialog::Date()
{
    return date;
}

QString EditDialog::Provider()
{
    return provider;
}

QString EditDialog::Description()
{
    return description;
}

void EditDialog::setName(QString name)
{
    ui->nameEdit->setText(name);
}

void EditDialog::setPrice(int price)
{
    ui->priceSpinBox->setValue(price);
}

void EditDialog::setWeight(int weight)
{
    ui->weightSpinBox->setValue(weight);
}

void EditDialog::setDate(QDate date)
{
    ui->dateEdit->setDate(date);
}

void EditDialog::setProveder(QString provider)
{
    ui->providerEdit->setText(provider);
}

void EditDialog::setDescription(QString description)
{
    ui->plainTextEdit->setPlainText(description);
}

void EditDialog::on_nameEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!arg1.isEmpty() & !ui->providerEdit->text().isEmpty());
    changed = true;
}

void EditDialog::on_providerEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!arg1.isEmpty() & !ui->nameEdit->text().isEmpty());
    changed = true;
}

void EditDialog::closeEvent(QCloseEvent *event)
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

void EditDialog::accept()
{
    name = ui->nameEdit->text();
    price = ui->priceSpinBox->value();
    weight = ui->weightSpinBox->value();
    date = ui->dateEdit->date();
    provider = ui->providerEdit->text();
    description = ui->plainTextEdit->toPlainText();
    changed = false;
    setResult(QDialog::Accepted);
    close();
}

void EditDialog::reject()
{
    setResult(QDialog::Rejected);
      close();
}
