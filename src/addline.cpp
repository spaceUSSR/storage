#include "addline.h"
#include "ui_addline.h"
#include <QMessageBox>

AddLine::AddLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLine),
    changed(false)
{
    ui->setupUi(this);
}

AddLine::~AddLine()
{
    delete ui;
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
