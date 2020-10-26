#include "addline.h"
#include "ui_addline.h"

AddLine::AddLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLine)
{
    ui->setupUi(this);
}

AddLine::~AddLine()
{
    delete ui;
}
