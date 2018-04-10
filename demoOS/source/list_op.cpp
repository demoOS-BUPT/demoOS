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
void chgState(QList<Process*>& s_list, QList<Process*>&d_list, unsigned long PID){
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

void termiProcess(QList<QList<Process*>> &lists, unsigned long PID)
{
    Process* p=nullptr;
    for(int i=0;i<lists.size();i++){
        QList<Process*> list=lists.at(i);
        p=find(list,PID);
        if(p!=nullptr)break;//找到进程指针
    }
    if(p==nullptr)return;//进程不存在
    for(int i=0;i<lists.size();i++){//从所有队列删除
        QList<Process*> list=lists.at(i);
        for(int j=0;j<list.size();j++)
            if(list.at(j)->getPid()==PID){//删除队列中进程,假设只有一个
                list.removeAt(j);
                break;
            }
    }
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
