#include "demo_process.h"

Process::Process()
{
    this->state=UNLOAD;
    this->pid=1;
}

void ProcessList::newProcess(){
    while(this->find(lastPID+1)){
        lastPID=lastPID+1;
    }
    Process* newP=new Process();
    this->list.append(*newP);
}

bool ProcessList::find(unsigned long PID){

}
