#include "list_op.h"

void newProcess(QList<Process> list,unsigned long lastPID)
{
    while(find(list,lastPID+1)){
        lastPID=lastPID+1;
    }
    Process* newP=new Process(lastPID+1);
    list.append(*newP);
}

bool find(QList<Process> list, unsigned long PID){
    for ( int i=0; i!=list.size(); ++i )
    {
       if ( list.at(i).getPid() == PID)
       {
           return true;
       }
    }
    return false;
}


void addProcess(QList<Process> s_list, QList<Process*> d_list, unsigned long PID){
    for ( int i=0; i!=s_list.size(); ++i )
    {
       if ( s_list.at(i).getPid() == PID)
       {
           d_list.append(const_cast<Process*>(&s_list.at(i)));
       }
    }
}

