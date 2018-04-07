#ifndef PROCESS_H
#define PROCESS_H

#include <QtCore>

class Process
{
public:
    Process();
    Process(unsigned long PID);
    enum PState{WAIT,RUN,READY,UNLOAD,TERMINATED};
    unsigned long getPid() const{return pid;}
private:
    unsigned long pid;
    PState state;
    unsigned long base;     //页表基地址
    unsigned long size;     //内存大小
    QString path;           //程序路径
    unsigned long PC;       //程序计数器
    int priority;           //优先级
    int CPUtime;            //运行时间（剩余）
};

class ProcessList{
public:
    ProcessList();
    void newProcess();
    void addProcess(unsigned long PID);
    bool find(unsigned long PID);
private:
    QList<Process> list;
    unsigned long lastPID;
};

#endif // PROCESS_H
