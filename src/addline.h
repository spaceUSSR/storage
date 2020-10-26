#ifndef ADDLINE_H
#define ADDLINE_H

#include <QDialog>

namespace Ui {
class AddLine;
}

class AddLine : public QDialog
{
    Q_OBJECT

public:
    explicit AddLine(QWidget *parent = nullptr);
    ~AddLine();

private:
    Ui::AddLine *ui;
};

#endif // ADDLINE_H
