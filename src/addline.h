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
    explicit AddLine(QWidget *parent = nullptr);
    ~AddLine();

    QString Name();
    int Price();
    int Weight();
    QDate Date();
    QString Provider();
    QString Description();

    void setName(QString);
    void setPrice(int);
    void setWeight(int);
    void setDate(QDate);
    void setProveder(QString);
    void setDescription(QString);

private slots:
    void on_nameEdit_textChanged(const QString &arg1);

    void on_providerEdit_textChanged(const QString &arg1);

private:
    Ui::AddLine *ui;
    bool changed;

    QString name;
    int price;
    int weight;
    QDate date;
    QString provider;
    QString description;


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

    // QDialog interface
public slots:
    void accept();
    void reject();
};

#endif // ADDLINE_H
