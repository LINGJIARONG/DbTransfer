#include "dbmanager.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QDateTime>
#include <QPushButton>
dbManager::dbManager()
{

}

void dbManager::connectSource(QString url, QString database, QString username, QString password, QString charset)
{

    QSqlDatabase fDb = QSqlDatabase::addDatabase("QIBASE", "Source");
    fDb.setHostName(url);
    fDb.setDatabaseName(database);
    fDb.setUserName(username);
    fDb.setPassword(password);
    fDb.setConnectOptions(QString("ISC_DPB_LC_CTYPE=%1").arg(charset));
    if(fDb.open()){
        qDebug()<<"connect to source successfully";
    }else{
        qDebug()<<"fail to connect to source ";

    }
}

void dbManager::connectDestination(QString url, QString database, QString username, QString password, QString charset)
{

    QSqlDatabase fDb = QSqlDatabase::addDatabase("QIBASE", "Destination");
    fDb.setHostName(url);
    fDb.setDatabaseName(database);
    fDb.setUserName(username);
    fDb.setPassword(password);
    fDb.setConnectOptions(QString("ISC_DPB_LC_CTYPE=%1").arg(charset));
    if(fDb.open()){
        qDebug()<<"connect to destination successfully";
    }else{
        qDebug()<<"fail to connect to destination ";

    }
}

void addLog(QStandardItemModel* model,QString type,QString text){
    QString log=QString("%1 [%2] %3 ").arg(QDateTime::currentDateTime().toString())
            .arg(type).arg(text);
    QStandardItem *item = new QStandardItem(log);
    item->setBackground(Qt::white);
    item->setEditable(false);
    model->appendRow(item);
}

void dbManager::start(QStandardItemModel* view,QPushButton* exit ){
    qDebug()<<"Start transfering ...";
    addLog(view,"info","Start transfering ...");

    //close index
    QSqlQuery query(QSqlDatabase::database("Destination"));
    QString q1="select rdb$index_name "
               " from rdb$indices"
               " where (rdb$system_flag is null or rdb$system_flag = 0)"
               " order by rdb$foreign_key nulls first";
    //find idx
    if(!query.prepare(q1)){
        qDebug()<<query.lastError().text();
        addLog(view,"Error",query.lastError().text());

    }else{
        query.exec();
        //every idx
        while(query.next()){
            QSqlQuery query2(QSqlDatabase::database("Destination"));
            QString q2=QString("ALTER INDEX %1 INACTIVE").arg(query.value(0).toString());
            if(!query2.exec(q2)){
                qDebug()<<"disactive index "<<query.value(0).toString()<<" Failed "<<query2.lastError().text() ;
                addLog(view,"ERROR","disactive index "+query.value(0).toString()+" Failed "+query2.lastError().text());
            }else{
                qDebug()<<"disactive index "<<query.value(0).toString()<<" successfully " ;
                addLog(view,"info","disactive index "+query.value(0).toString()+" successfully ");

            }
        }
    }

    // show tables from source
    QSqlQuery query3(QSqlDatabase::database("Source"));
    QString q3="select rdb$relation_name"
               " from rdb$relations"
               " where rdb$view_blr is null"
               " and (rdb$system_flag is null or rdb$system_flag = 0);";
    if(!query3.prepare(q3)){
        qDebug()<<"search all tables from source : " <<query3.lastError().text();
        addLog(view,"Error","search all the tables from source : " +query3.lastError().text());

    }else{
        query3.exec();
        int totalData=0;
        //A table
        while(query3.next()){
            QString tabName=query3.value(0).toString();
            int data=0;
            qDebug()<<"transfering table "<< tabName;
            addLog(view,"INFO","transfering table " +tabName);

            QSqlTableModel *model = new QSqlTableModel(nullptr,QSqlDatabase::database("source"));
            model->setTable(tabName);
            model->select();
            QSqlTableModel *model_des = new QSqlTableModel(nullptr,QSqlDatabase::database("source"));
            model_des->setTable(tabName);
            model_des->setEditStrategy(QSqlTableModel::OnManualSubmit);
            model_des->select();
            for(int i=0;i<model->rowCount();i++){
                data++;
                QSqlRecord rec=model->record(i);
                model_des->insertRecord(-1,rec);
            }
            if(model->rowCount()!=0){
                if(model_des->submitAll()){
                    qDebug()<<data<<" datas transfer successfully from table "<<tabName;
                    qDebug()<<"submit " << tabName<<" successfully";
                    addLog(view,"INFO"," datas transfer successfully from table " +tabName);
                    totalData+=data;
                }
            }
            qDebug()<<"total "<<totalData << "pieces of data transferered . Finished";
            addLog(view,"INFO","total "+QString::number(totalData) +"pieces of data transferered Finished");

        }


        //active idx
        if(!query.prepare(q1)){
            qDebug()<<query.lastError().text();
            addLog(view,"ERROR",query.lastError().text());

        }else{
            query.exec();
            while(query.next()){
                QSqlQuery query2(QSqlDatabase::database("Destination"));
                QString q2=QString("ALTER INDEX %1 ACTIVE").arg(query.value(0).toString());

                if(!query2.exec(q2)){
                    qDebug()<<"active index "<<query2.lastError().text();
                    addLog(view,"ERROR",query2.lastError().text());
                }else{
                    qDebug()<<"active index "<<query.value(0).toString()<<" successfully " ;
                    addLog(view,"INFO",query2.lastError().text());
                }
            }
        }

    }

}




