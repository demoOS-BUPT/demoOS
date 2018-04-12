#include "list_op.h"

//获取新PID,返回0表示失败
unsigned long getNewPID(QList<Process*>& pcbPool)
{
    if (pcbPool.isEmpty()) return 1;
    unsigned long max=0;
    for(int i=0;i<pcbPool.size();i++){
        unsigned long thisPID=pcbPool.at(i)->getPid();
        if(thisPID>max) max=thisPID;
    }
    max=max+1;

    if(max!=0) return max;

    //max上溢到0,pid可能满了
    max=1;
    while (find(pcbPool,max)!=nullptr && max!=0)
        max++;
    if(max==0) return 0;//所有pid都被占用了。
    else return max;
}

//新建一个进程
Process* newProcess(QList<Process*>& pcbPool)
{
    unsigned long newPID=getNewPID(pcbPool);
    if(newPID==0) return nullptr;//没有可用PID 失败

    Process* newP=new Process(newPID);
    pcbPool.append(newP);
    return newP;
}

//状态转移 s_list -> d_list
void moveProcess(QList<Process*>& s_list, QList<Process*>&d_list, unsigned long PID){
    Process* p=nullptr;
    for ( int i=0; i!=s_list.size(); ++i ){//在s_list中查找PID
       Process* pcbi=s_list.at(i);
       if (pcbi->getPid() == PID){//找到
           p = pcbi;
           s_list.removeAt(i);
           break;
       }
    }
    if(p==nullptr) return;//s_list中没有这个进程，失败。
    //假设进程在d_list不存在
    d_list.append(p);
    return;
}

void termiProcess(QList<Process*> &pcbPool,
                  QList<Process*> &readyQueue,
                  QList<Process*> &runningQueue,
                  QList<Process*> &waitQueue, unsigned long PID)
{
    Process* p=nullptr;
    p=find(pcbPool,PID);
    if(p==nullptr)return;//进程不存在

    //从所有队列删除 (用了宏 对不起！)
    #define REMOVE_FROM_LIST(LISTNAME) do{\
        for(int j=0;j<LISTNAME.size();j++)\
            if(LISTNAME.at(j)->getPid()==PID){\
                LISTNAME.removeAt(j);\
                break;\
            }\
    }\
    while(false);

    REMOVE_FROM_LIST(pcbPool)
    REMOVE_FROM_LIST(readyQueue)
    REMOVE_FROM_LIST(runningQueue)
    REMOVE_FROM_LIST(waitQueue)


    delete p;//释放
    return;
}

Process* find(QList<Process*> list, unsigned long PID)
{
    for ( int i=0; i!=list.size(); ++i ){
       Process* pcbi=list.at(i);
       if ( pcbi->getPid() == PID){
           return pcbi;
       }
    }
    return nullptr;
}

QString printQue(QList<Process*> &q)
{
    QString ret;
    for ( int i=0; i!=q.size(); ++i ){
       ret+=QString::number(q.at(i)->getPid())+" ";
    }
    return ret;
}

//进程调度函数
void processDispatch(QList<Process*> &pcbPool,
                     QList<Process*> &readyQueue,
                     QList<Process*> &runningQueue,
                     QList<Process*> &waitQueue,
                     ProcessAlg alg){
    //终结进程
    QList<Process*> killList;
    for(int i=0;i<runningQueue.size();i++){
        Process* p=runningQueue.at(i);
        if(p->getCPUtime()<=0) killList.append(p);
    }
    for(int i=0;i<killList.size();i++){
        termiProcess(pcbPool,readyQueue,runningQueue,waitQueue,
                     killList.at(i)->getPid());
    }

    //进程调度
    switch (alg) {
        case RR:{
            Process* in=nullptr,* out=nullptr;
            if(!readyQueue.isEmpty())
                in=readyQueue.at(0);
            if(!runningQueue.isEmpty())
                out=runningQueue.at(0);
            if(out!=nullptr&&in!=nullptr){
                moveProcess(runningQueue,readyQueue,out->getPid());
                moveProcess(readyQueue,runningQueue,in->getPid());
            }
            else if(out==nullptr&&in!=nullptr){
                moveProcess(readyQueue,runningQueue,in->getPid());
            }
            else break;//这个分支in==nullptr,没有可以换入的。
            break;
        }
        case FCFS:
        default:{
            if(runningQueue.isEmpty()&&
                    !readyQueue.isEmpty()){
                Process* in=readyQueue.at(0);
                moveProcess(readyQueue,runningQueue,in->getPid());
            }
            break;
        }
    }

}

//进程执行
void execute(QList<Process*> &pcbPool,QList<Process*> &runningQueue){
    for(int i=0;i<runningQueue.size();i++){
        Process* p=runningQueue.at(i);
        p->setCPUtime(p->getCPUtime()-1);
    }
}