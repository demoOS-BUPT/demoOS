#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QList>
#include "demo_process.h"
#include "list_op.h"
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <random>
#include "RAM/firstfit.h"
#include "FS/other.h"
#include<string>
#include<new>
#include<cstdlib>
#include<cstring>

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
    QMovie *runGif;
    QMovie *stopGif;

    QList<Process*> pcbPool;    //所有进程

    QList<Process*> waitQueue;     //等待队列
    QList<Process*> readyQueue;    //就绪队列
    QList<Process*> runningQueue;   //在运行

    //多级反馈队列
    QList<Process*> RR1;
    QList<Process*> RR2;
    QList<Process*> FCFS;

    std::mt19937 rand;//用 rand()得到随机数

    Firstfit ram;
    const size_t ramSize=0x10000;//128KB

    void cmdPrint(QString newLine);

    void createProcess(int cpuTime,int priority,int ramSize);//创建一个新进程，若成功放入就绪队列。

    void printQueue();

    void FS_init();//FS子系统初始化

private slots:
    void kernel();

    void on_pushButton_clicked();
    void on_pauseButton_clicked();
    void on_processAlgComboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
