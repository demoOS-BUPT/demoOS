#ifndef LIST_OP_H
#define LIST_OP_H
#include "demo_process.h"
#include "RAM/firstfit.h"


enum ProcessAlg{
    RR=0,FCFSa=1,PRI_RR=2,
    DYNAMIC_PRI=3,PREEEM_PRI=4,NONPREEM_PRI=5,
    NSJF=6,SJF=7,MFQ=8,HRRN=9};

//优先级0~lowest_pri
#define lowest_pri 7

//创建新进程(空的),并存放在pcbPool中,若失败返回nullptr
Process* newProcess(QList<Process*> &pcbPool);

//在队列间移动 s_list -> d_list
void moveProcess(QList<Process*>& s_list, QList<Process*>& d_list, unsigned long PID);

//结束进程，在所有list中删除进程,然后释放Process对象 0不存在 1成功删除
int termiProcess(QList<Process*> &pcbPool,
                  QList<Process*> &readyQueue,
                  QList<Process*> &runningQueue,
                  QList<Process*> &waitQueue,
                 QList<Process*> &RR1,
                 QList<Process*> &RR2,
                 QList<Process*> &FCFS,
                 Firstfit &ram,unsigned long PID);

void ioDispatch(QList<Process*> &readyQueue,
                     QList<Process*> &waitQueue);

//list中查找PID,若没有返回nullptr
Process* find(QList<Process*> list, unsigned long PID);

//队列状态打印
QString printQue(QList<Process*> &q);

//进程调度函数
void processDispatch(QList<Process*> &pcbPool,
                     QList<Process*> &readyQueue,
                     QList<Process*> &runningQueue,
                     QList<Process*> &waitQueue,
                     QList<Process*> &RR1,
                     QList<Process*> &RR2,
                     QList<Process*> &FCFS,
                     Firstfit &ram,
                     ProcessAlg alg);

//进程执行
void execute(QList<Process*> &pcbPool,
             QList<Process*> &runningQueue,
             QList<Process*> &readyQueue,
             QList<Process*> &waitQueue,
             Firstfit &ram);

#endif // LIST_OP_H
