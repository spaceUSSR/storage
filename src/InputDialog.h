#ifndef NEWTABLE_H
#define NEWTABLE_H

#include <QDialog>
#include <QLineEdit>
class QRegExpValidator;

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QString labelText, QRegExp regEx, QWidget *parent = nullptr);
    ~InputDialog();

    QString getText();
private:
    QLineEdit* lineEdit;

//private slots:
//    void slotAccepted();
//    void slotRejected();

private:
    void createUI(QString label, QRegExpValidator* validator);
};

#endif // NEWTABLE_H
