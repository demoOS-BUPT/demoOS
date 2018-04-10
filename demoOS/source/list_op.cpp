#include "list_op.h"

//新建一个进程还需要其他信息吧
void newProcess(QList<Process>& pcbPool, QList<Process*> & list,unsigned long lastPID)
{
    while(find(pcbPool,lastPID+1)){
        lastPID=lastPID+1;
    }
    Process* newP=new Process(lastPID+1);
    pcbPool.append(*newP);
    list.append(newP);
}

//状态转移 s_list -> d_list
void chgState(QList<Process*>& s_list, QList<Process*>&d_list, unsigned long PID){
    for ( int i=0; i!=s_list.size(); ++i )
    {
       if ( s_list.at(i)->getPid() == PID)
       {
           d_list.append(s_list.at(i));
           s_list.removeAt(i);
            //删除元素这有问题##############
       }
    }
}

void termiProcess(QList<Process> &pcbPool, QList<Process*> &list, unsigned long PID)
{
    for ( int i=0; i!=list.size(); ++i )
    {
       if ( list.at(i)->getPid() == PID)
       {   delete(list.at(i));//指针的释放可能有问题
           list.removeAt(i);
       }
    }

    for ( int i=0; i!=pcbPool.size(); ++i )
    {
       if ( pcbPool.at(i).getPid() == PID)
       {
           pcbPool.removeAt(i);
       }
    }
}

bool find(QList<Process> pcbPool, unsigned long PID){
    for ( int i=0; i!=pcbPool.size(); ++i )
    {
       if ( pcbPool.at(i).getPid() == PID)
       {
           return true;
       }
    }
    return false;
}

void printQue(QList<Process*> q)
{   for ( int i=0; i!=q.size(); ++i )
    {
       qDebug()<<QString::number(q.at(i)->getPid());
    }
}
