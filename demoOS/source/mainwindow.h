#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QList>
#include "demo_process.h"
#include "list_op.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <random>
#include "RAM/firstfit.h"
#include "FS/other.h"
//#include "FS/instructionOp.h"
#include<cmath>
#include<string>
#include<new>
#include<cstdlib>
#include<cstring>
using namespace  std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const size_t ramSize=8192;//0x10000;//64KB
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

    void cmdPrint(QString newLine);//输出新行
    void cmdPrint_noLn(QString s);//输出

    void createProcess(int cpuTime,int priority,int ramSize,
                       Directory *fileDir);//创建一个新进程，若成功放入就绪队列。

    void printQueue();

    //进程执行
    void execute();

    void FS_init();//FS子系统初始化

    void mkdir_instruction(string op);

    void touch_instruction(string op);

    void cp_instruction(string op);

    void mv_instruction(string op);

    void ln_instruction(string op);

    void rm_instruction(string op);

    void cat_instruction(string op);

    void vim_instruction(string op);

    void ls_instruction(string op);

    void su_instruction(string op);

    void chmod_instruction(string op);

    void chown_instruction(string op);

    void chgrp_instruction(string op);

    //获取文件内容
    QString getFileContent(QString fileName);
    QString getFileContent(Directory *fileDir);
private slots:
    void kernel();

    void on_pushButton_clicked();
    void on_pauseButton_clicked();
    void on_processAlgComboBox_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
