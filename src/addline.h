#ifndef ADDLINE_H
#define ADDLINE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QDate>

namespace Ui {
class AddLine;
}

class AddLine : public QDialog
{
    Q_OBJECT

public:
    explicit AddLine(/*QSqlDatabase& db, QString& table, */QWidget *parent = nullptr);
    ~AddLine();

    QString Name();
    int Price();
    int Weight();
    QDate Date();
    QString Provider();
    QString Description();

private slots:
    void on_nameEdit_textChanged(const QString &arg1);

    void on_providerEdit_textChanged(const QString &arg1);

    void on_okButton_clicked();

private:
    Ui::AddLine *ui;
    bool changed;
//    QSqlDatabase db;
//    QString table;

    QString name;
    int price;
    int weight;
    QDate date;
    QString provider;
    QString description;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ADDLINE_H
