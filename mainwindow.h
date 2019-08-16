#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStringList strList;
    QStandardItemModel * standardItemModel;

private slots:

    void startBtnClicked();
    void srcConnectBtnClicked();
    void targetConnectBtnClicked();
    void finishBtnClicked();
    void exitBtnClicked();

};

#endif // MAINWINDOW_H
