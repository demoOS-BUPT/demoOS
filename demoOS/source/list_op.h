#ifndef LIST_OP_H
#define LIST_OP_H
#include "mainwindow.h"

//pcb池、new队列添加进程
void newProcess(QList<Process>& pcbpool, QList<Process*> & list,unsigned long lastPID);

//状态转移 s_list -> d_list
void chgState(QList<Process*>& s_list, QList<Process*>& d_list, unsigned long PID);

//结束进程，所处队列和pcb池中删除进程
void termiProcess(QList<Process>& pcbPool, QList<Process*>& list,unsigned long PID);

//在pcb池中是否存在进程Pid
bool find(QList<Process> pcbpool, unsigned long PID);

//查看队列内容
void printQue(QList<Process*> q);

#endif // LIST_OP_H
