#ifndef PROCESS_H
#define PROCESS_H

#include <QtCore>

class Process
{
public:
    Process();
    Process(unsigned long PID);
    enum PState{WAIT,RUN,READY,UNLOAD,TERMINATED};

    //打印进程参数
    void printInfo();
    unsigned long getPid() const{return pid;}

    void setState(PState s){state=s;}
    PState getState() {return this->state;}

    void setCPUtime(int t) {this->CPUtime=t;}
    int getCPUtime() {return this->CPUtime;}

    void setPriority(int p) {this->priority=p;}
    int getPriority() {return this->priority;}

    void setAge(int a){age=a;}
    int getAge(){return age;}

    void setBase(int b){base=b;}
    int getBase(){return base;}

    void setSize(int s){size=s;}
    int getSize(){return size;}

    void setIo(int t){io=t;}
    int getIo(){return io;}

    void setPc(int p){PC=p;}
    int getPc(){return PC;}


    void setPpid(int p){ppid=pid;}
    int getPpid(){return ppid;}
private:
    unsigned long pid;
    PState state;
    unsigned long base;     //页表基地址
    unsigned long size;     //内存大小
    QString path;           //程序路径
    unsigned long PC;       //程序计数器
    int priority;           //优先级 数小优先级高
    int CPUtime;            //运行时间（剩余）
    int age;
    int io;                 //io时间
    unsigned long ppid;      //父进程id
};



#endif // PROCESS_H

