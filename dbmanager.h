#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include<QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class dbManager
{
public:
    dbManager();
    static void connectDestination(QString url,QString database,QString username,QString password,QString charset);
    static void connectSource(QString url,QString database,QString username,QString password,QString charset);
    static void start();

};

#endif // DBMANAGER_H
