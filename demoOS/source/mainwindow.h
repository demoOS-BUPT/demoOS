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
#include <random>

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

    QList<Process*> pcbPool;    //所有进程

    QList<Process*> waitQueue;     //等待队列
    QList<Process*> readyQueue;    //就绪队列
    QList<Process*> runningQueue;   //在运行

    std::mt19937 rand;//用 rand()得到随机数

    void cmdPrint(QString newLine);

    void createProcess(int cpuTime);//创建一个新进程，若成功放入就绪队列。

private slots:
    void kernel();

    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
