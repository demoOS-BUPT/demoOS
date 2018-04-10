#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QList>
#include "demo_process.h"
#include "list_op.h"
//#include <QString>
#include <QtCore>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;

    QList<Process> pcbPool;

    QList<Process*> pnewProQue;     //创建新进程队列 存进程PCB
    unsigned long lastPID;   //便于分配新pid

    QList<Process*> pwaitProQue;     //等待队列 存进程PCB指针
    QList<Process*> preadyProQue;    //就绪队列 存进程PCB指针

    void cmdPrint(QString newLine);


private slots:
    void kernel();

};

#endif // MAINWINDOW_H
