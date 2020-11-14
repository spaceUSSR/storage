#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(w.isOpened()) {
        w.showMaximized();
        a.exec();
    }
    return 0;
}
