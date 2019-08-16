#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include<QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStandardItem>
#include<QStandardItemModel>
#include <QPushButton>

class dbManager
{
public:
    dbManager();
    static bool connectDestination(QString url,QString database,QString username,QString password,QString charset);
    static bool connectSource(QString url,QString database,QString username,QString password,QString charset);
    static void start(QStandardItemModel* model,QPushButton * button);

};

#endif // DBMANAGER_H
