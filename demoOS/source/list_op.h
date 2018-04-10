#ifndef LIST_OP_H
#define LIST_OP_H
#include "mainwindow.h"

//创建新进程(空的),并存放在pcbPool中,若失败返回nullptr
Process* newProcess(QList<Process*> &pcbPool);

//在队列间移动 s_list -> d_list
void move(QList<Process*>& s_list, QList<Process*>& d_list, unsigned long PID);

//结束进程，在所有list中删除进程,然后释放Process对象
void termiProcess(QList<QList<Process*>> &lists, unsigned long PID);

//list中查找PID,若没有返回nullptr
Process* find(QList<Process*> list, unsigned long PID);

//队列状态打印
QString printQue(QList<Process*> q);

#endif // LIST_OP_H
