#include "newtable.h"
#include "ui_newtable.h"
#include <QRegExpValidator>

newtable::newtable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newtable)
{
    ui->setupUi(this);
    QValidator *validator  = new QRegularExpressionValidator(QRegularExpression("^[a-zA-Z0-9]+$"), this);
    ui->lineEdit->setValidator(validator);
}

newtable::~newtable()
{
    delete ui;
}
