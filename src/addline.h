#ifndef ADDLINE_H
#define ADDLINE_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class AddLine;
}

class AddLine : public QDialog
{
    Q_OBJECT

public:
    explicit AddLine(QSqlDatabase& db, QString& table, QWidget *parent = nullptr);
    ~AddLine();

private slots:
    void on_nameEdit_textChanged(const QString &arg1);

    void on_providerEdit_textChanged(const QString &arg1);

    void on_okButton_clicked();

private:
    Ui::AddLine *ui;
    bool changed;
    QSqlDatabase db;
    QString table;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ADDLINE_H
