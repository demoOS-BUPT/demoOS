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
