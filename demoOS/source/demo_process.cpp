#include "demo_process.h"

Process::Process()
{
    this->state=UNLOAD;
    this->pid=1;
}

Process::Process(unsigned long PID)
{
    this->state=UNLOAD;
    this->pid=PID;
}

ProcessList::ProcessList(){
    this->list.clear();
    this->lastPID = -1;
}

void ProcessList::newProcess(){
    while(this->find(lastPID+1)){
        lastPID=lastPID+1;
    }
    Process* newP=new Process(lastPID+1);
    this->list.append(*newP);
}

void ProcessList::addProcess(unsigned long PID){
    //从unload队列中找到pid进程加入队列
}

bool ProcessList::find(unsigned long PID){
    for ( int i=0; i!=this->list.size(); ++i )
    {
       if ( this->list.at(i).getPid() == PID)
       {
           return true;
       }
    }
    return false;
}
