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

private slots:
    void on_nameEdit_textChanged(const QString &arg1);

    void on_providerEdit_textChanged(const QString &arg1);

private:
    Ui::AddLine *ui;
    bool changed;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // ADDLINE_H
