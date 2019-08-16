#include "dbmanager.h"
#include <QDebug>
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

void dbManager::start(){
    qDebug()<<"Start transfering ...";
    qDebug()<<"Closing idx  ...";
    //close index
    QSqlQuery query(QSqlDatabase::database("Destination"));
    QString q1="select rdb$index_name "
               " from rdb$indices"
               " where (rdb$system_flag is null or rdb$system_flag = 0)"
               " order by rdb$foreign_key nulls first";
    //find idx
    if(!query.prepare(q1)){
        qDebug()<<query.lastError().text();
    }else{
        query.exec();
        while(query.next()){
            QSqlQuery query2(QSqlDatabase::database("Destination"));
            QString q2=QString("ALTER INDEX %1 INACTIVE").arg(query.value(0).toString());
            if(!query2.exec(q2)){
                qDebug()<<"disactive index "<<query.value(0).toString()<<" succefully " ;
            }else{
                qDebug()<<"disactive index "<<query.value(0).toString()<<" succefully " ;
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
    }else{
        query3.exec();
        int totalData=0;
        //A table
        while(query3.next()){
            QString tabName=query3.value(0).toString();
            int data=0;
            qDebug()<<"transfering table "<< tabName;
            QString sourceData=QString("select * from %1 ").arg(tabName);
            //get datas
            QSqlQuery query4(QSqlDatabase::database("Source"));
            if(!query4.exec(sourceData)){
                qDebug()<<"can't find data from "<<tabName;
            }else{
                // A data
                while(query4.next()){
                    //copy to destination
                    QSqlRecord rec=query4.record();
                    //insert into tabNmae values(
                    //xi,
                    //xfinal)
                    QString insert=QString(" insert into %1 values( ").arg(tabName);
                    for(int i=0;i<rec.count()-1;i++){
                        insert.append(QString("%1,").arg(query4.value(i).toString()));
                    }
                    if(rec.count()-1!=0){
                        insert.append(query4.value(rec.count()-1).toString());
                        insert.append(")");
                    }
                    if(!query.prepare(insert)){
                        qDebug()<< insert<<query.lastError().text();
                    }else{
                        if(query.exec())
                            data++;
                    }
                }
            }
            qDebug()<<data<<" datas transfer successfully from table "<<tabName;
            totalData+=data;
        }
        qDebug()<<"total "<<totalData << "pieces of data transferered . Finished";


        //active idx
        if(!query.prepare(q1)){
            qDebug()<<query.lastError().text();
        }else{
            query.exec();
            while(query.next()){
                QSqlQuery query2(QSqlDatabase::database("Destination"));
                QString q2=QString("ALTER INDEX %1 ACTIVE").arg(query.value(0).toString());
                if(!query2.exec(q2)){
                    qDebug()<<"active index "<<query.lastError().text();
                }else{
                    qDebug()<<"active index "<<query.value(0).toString()<<" succefully " ;
                }
            }
        }

    }



}


