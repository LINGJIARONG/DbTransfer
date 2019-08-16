#include "mainwindow.h"
#include <QApplication>
#include <dbmanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    dbManager::connectSource("192.168.0.21","arcresto","SYSDBA","masterkey","NONE");
    w.show();

    return a.exec();
}
