#ifndef NEWTABLE_H
#define NEWTABLE_H

#include <QDialog>

namespace Ui {
class newtable;
}

class newtable : public QDialog
{
    Q_OBJECT

public:
    explicit newtable(QWidget *parent = nullptr);
    ~newtable();

private:
    Ui::newtable *ui;
};

#endif // NEWTABLE_H
