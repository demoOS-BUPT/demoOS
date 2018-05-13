#include "demo_process.h"

Process::Process()
{
    this->state=UNLOAD;
    this->pid=1;
    age=0;
}

Process::Process(unsigned long PID)
{
    this->state=UNLOAD;
    this->pid=PID;
    this->PC = 0; //
    /*
    age=0;
    /*
    QString path;           //程序路径
    unsigned long PC;
    */
}

void Process::printInfo()
{
    QString s;
    s = this->pid + " "+
        QString::number(this->priority) + " " +
        this->PC + " " +
        (enum PState)this->state + " "+
        QString::number(this->CPUtime);
    qDebug() << s;
}
