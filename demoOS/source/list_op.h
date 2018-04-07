#ifndef LIST_OP_H
#define LIST_OP_H
#include "mainwindow.h"

void newProcess(QList<Process> list,unsigned long lastPID);//unload队列中创建进程
bool find(QList<Process> list, unsigned long PID);

void addProcess(QList<Process> s_list, QList<Process*> d_list, unsigned long PID);//等待、就绪队列中添加进程

#endif // LIST_OP_H
