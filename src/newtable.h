#ifndef NEWTABLE_H
#define NEWTABLE_H

#include <QDialog>
class QSqlDatabase;

namespace Ui {
class newtable;
}

class newtable : public QDialog
{
    Q_OBJECT

public:
    explicit newtable(QSqlDatabase* db, QWidget *parent = nullptr);
    ~newtable();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_okButton_clicked();

private:
    Ui::newtable *ui;
    QSqlDatabase *db;
};

#endif // NEWTABLE_H
