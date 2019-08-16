#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include"dbmanager.h"
#include <QDebug>

void MainWindow::finishBtnClicked(){
   this->close();
}


void MainWindow::exitBtnClicked(){
   this->close();
}



void MainWindow::srcConnectBtnClicked(){
    QString srcName=ui->srcNameLine->text();
    QString srcUsername=ui->srcUsernameLine->text();
    QString srcPassword=ui->srcPasswordLine->text();
    QString srcURL=ui->srcUrlLine->text();
    bool srcGB=ui->srcGbBox->isChecked();
    bool srcUTF=ui->srcUtfBox->isChecked();
    bool srcLatin=ui->srcLatinBox->isChecked();
    QString srcCharset;

    if(srcURL==""||srcURL.isNull()){
        QMessageBox::information(this,"","Please input Source DB URL.");
        return;
    }
    if(srcName==""||srcName.isNull()){
        QMessageBox::information(this,"","Please input Source DB Name.");
        return;
    }
    if(srcUsername==""||srcUsername.isNull()){
        QMessageBox::information(this,"","Please input Source DB Username.");
        return;
    }
    if(srcPassword==""||srcPassword.isNull()){
        QMessageBox::information(this,"","Please input Source DB Password.");
        return;
    }
    if(srcGB==false&&srcUTF==false&&srcLatin==false){
        QMessageBox::information(this,"","Please check one of the Source DB Charset.");
        return;
    }

    if(srcGB){
        srcCharset="GB2312";

    }
    if(srcUTF){
        srcCharset="UTF8";

    }
    if(srcLatin){
        srcCharset="LATIN1";

    }
    if(dbManager::connectSource(srcURL,srcName,srcUsername,srcPassword,srcCharset)==false){
        QMessageBox::information(this,"","Fail to connect source DB.");
    }
    ui->srcConnectBtn->setEnabled(false);
    ui->srcUrlLine->setEnabled(false);
    ui->srcNameLine->setEnabled(false);
    ui->srcUsernameLine->setEnabled(false);
    ui->srcPasswordLine->setEnabled(false);
    ui->srcGbBox->setEnabled(false);
    ui->srcLatinBox->setEnabled(false);
    ui->srcUtfBox->setEnabled(false);

}

void MainWindow::targetConnectBtnClicked(){
    QString targetName=ui->targetNameLine->text();
    QString targetUsername=ui->targetUsernameLine->text();
    QString targetPassword=ui->targetPasswordLine->text();
    QString targetURL=ui->targetUrlLine->text();
    QString targetCharset;


    bool targetGB=ui->targetGbBox->isChecked();
    bool targetUTF=ui->targetUtfBox->isChecked();
    bool targetLatin=ui->targetLatinBox->isChecked();

    if(targetURL==""||targetURL.isNull()){
        QMessageBox::information(this,"","Please input Target DB URL.");
        return;
    }
    if(targetName==""||targetName.isNull()){
        QMessageBox::information(this,"","Please input Target DB Name.");
        return;
    }
    if(targetUsername==""||targetUsername.isNull()){
        QMessageBox::information(this,"","Please input Target DB Username.");
        return;
    }
    if(targetPassword==""||targetPassword.isNull()){
        QMessageBox::information(this,"","Please input Target DB Password.");
        return;
    }
    if(targetGB==false&&targetUTF==false&&targetLatin==false){
        QMessageBox::information(this,"","Please check one of the Target DB Charset.");
        return;
    }
    if(targetGB){
        targetCharset="GB2312";

    }
    if(targetUTF){
        targetCharset="UTF8";

    }
    if(targetLatin){
        targetCharset="LATIN1";

    }
    if(dbManager::connectDestination(targetURL,targetName,targetUsername,targetPassword,targetCharset)==false){
        QMessageBox::information(this,"","Fail to connect Target DB.");
    }
    ui->targetConnectBtn->setEnabled(false);
    ui->targetUrlLine->setEnabled(false);
    ui->targetNameLine->setEnabled(false);
    ui->targetUsernameLine->setEnabled(false);
    ui->targetPasswordLine->setEnabled(false);
    ui->targetGbBox->setEnabled(false);
    ui->targetLatinBox->setEnabled(false);
    ui->targetUtfBox->setEnabled(false);

}


void MainWindow::startBtnClicked(){
    if(ui->srcConnectBtn->isEnabled()==true){
        QMessageBox::information(this,"","Please connect Source DB.");
        return;
    }
    if(ui->targetConnectBtn->isEnabled()==true){
        QMessageBox::information(this,"","Please connect Target DB.");
        return;
    }
    ui->configWidget->hide();
    ui->logWidget->show();
    dbManager::start(standardItemModel,ui->finishBtn);
    ui->logList->setModel(standardItemModel);

}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    standardItemModel=new QStandardItemModel(this);
    ui->srcUrlLine->setValidator(new QRegExpValidator(QRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$")));
    ui->targetUrlLine->setValidator(new QRegExpValidator(QRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$")));
    ui->configWidget->show();
    ui->logWidget->hide();
    connect(ui->startBtn,SIGNAL(clicked()),this,SLOT(startBtnClicked()));
    connect(ui->srcConnectBtn,SIGNAL(clicked()),this,SLOT(srcConnectBtnClicked()));
    connect(ui->targetConnectBtn,SIGNAL(clicked()),this,SLOT(targetConnectBtnClicked()));
    connect(ui->exitBtn,SIGNAL(clicked()),this,SLOT(exitBtnClicked()));
    connect(ui->finishBtn,SIGNAL(clicked()),this,SLOT(finishBtnClicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}
