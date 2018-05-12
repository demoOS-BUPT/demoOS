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
    /*
    age=0;
    /*
    unsigned long base;     //页表基地址
    unsigned long size;     //内存大小
    QString path;           //程序路径
    unsigned long PC;       //程序计数器
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
