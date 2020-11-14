#include <QRegExpValidator>
#include "InputDialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>

InputDialog::InputDialog(QString labelText, QRegExp regExpression, QWidget *parent) :
    QDialog(parent)
{
    createUI(labelText, new QRegExpValidator(regExpression, this));
}

InputDialog::~InputDialog()
{
}

QString InputDialog::getText()
{
    return lineEdit->text();
}

void InputDialog::createUI(QString lableText, QRegExpValidator* validator)
{
    lineEdit = new QLineEdit(this);
    lineEdit->setValidator(validator);

    QLabel* label = new QLabel(lableText, this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                       QDialogButtonBox::Cancel, this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(buttonBox);
    this->setLayout(layout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}
