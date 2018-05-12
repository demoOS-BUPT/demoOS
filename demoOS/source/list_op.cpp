#include "list_op.h"
#include "demo_process.h"

static bool boolMFQ=false;
static int timer=0;

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

int termiProcess(QList<Process*> &pcbPool,
                  QList<Process*> &readyQueue,
                  QList<Process*> &runningQueue,
                  QList<Process*> &waitQueue,
                 QList<Process*> &RR1,
                 QList<Process*> &RR2,
                 QList<Process*> &FCFS,
                 unsigned long PID)
{
    Process* p=nullptr;
    p=find(pcbPool,PID);
    if(p==nullptr)return 0;//进程不存在

    //从队列删除
    #define REMOVE_FROM_LIST(LISTNAME) do{\
        for(int j=0;j<LISTNAME.size();j++)\
            if(LISTNAME.at(j)->getPid()==PID){\
                LISTNAME.removeAt(j);\
                break;\
            }\
    }\
    while(false)

    REMOVE_FROM_LIST(pcbPool);
    REMOVE_FROM_LIST(readyQueue);
    REMOVE_FROM_LIST(runningQueue);
    REMOVE_FROM_LIST(waitQueue);
    REMOVE_FROM_LIST(RR1);
    REMOVE_FROM_LIST(RR2);
    REMOVE_FROM_LIST(FCFS);


    delete p;//释放
    return 1;
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
                     QList<Process*> &RR1,
                     QList<Process*> &RR2,
                     QList<Process*> &FCFS,
                     ProcessAlg alg){
    //终结进程
    QList<Process*> killList;
    for(int i=0;i<runningQueue.size();i++){
        Process* p=runningQueue.at(i);
        if(p->getCPUtime()<=0) killList.append(p);
    }
    for(int i=0;i<killList.size();i++){
        termiProcess(pcbPool,readyQueue,runningQueue,waitQueue,
                     RR1,RR2,FCFS,killList.at(i)->getPid());
    }

    if(alg==MFQ&&boolMFQ==false){//MFQ算法首次进入，加载readyQueue到RR1
        boolMFQ=true;
        RR1=readyQueue;
        readyQueue.clear();
        for(int i=0;i<RR1.size();i++) RR1.at(i)->setPriority(0);
    }
    else if(alg!=MFQ&&boolMFQ){//MFQ算法退出
        boolMFQ=false;
        readyQueue.append(RR1);
        readyQueue.append(RR2);
        readyQueue.append(FCFS);
        RR1.clear();
        RR2.clear();
        FCFS.clear();
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
        case PRI_RR:{
            Process* in=nullptr,* out=nullptr;
            if(!readyQueue.isEmpty())
            {   //优先级最高的来
                int min = lowest_pri+1;
                in = readyQueue.at(0);
                for(int i=0; i < readyQueue.size();i++){
                   if(readyQueue.at(i)->getPriority() < min)
                   {
                       in = readyQueue.at(i);
                       min = readyQueue.at(i)->getPriority();
                   }
                }
            }
            if(!runningQueue.isEmpty())
                out=runningQueue.at(0);//下一个了
            if(out!=nullptr&& in != nullptr){
                moveProcess(runningQueue,readyQueue,out->getPid());
            }
            if(in!=nullptr){
                moveProcess(readyQueue,runningQueue,in->getPid());
            }
            else break;//这个分支in==nullptr,没有可以换入的。
            break;
        }
        case DYNAMIC_PRI:{
            Process* in=nullptr,* out=nullptr;
            if(!readyQueue.isEmpty())
            {   //优先级最高的来
                int min = lowest_pri+1;
                in = readyQueue.at(0);
                for(int i=0; i < readyQueue.size();i++)
                {
                   if(readyQueue.at(i)->getPriority() < min)
                   {
                       in = readyQueue.at(i);
                       min = readyQueue.at(i)->getPriority();
                   }
                }
            }
            if(!runningQueue.isEmpty())
                out=runningQueue.at(0);//下一个了
            if(out!=nullptr&& in != nullptr){
                moveProcess(runningQueue,readyQueue,out->getPid());
                if( out->getPriority() < lowest_pri )
                {
                    out->setPriority( out->getPriority()+1 );
                }

            }
            if(in!=nullptr){
                moveProcess(readyQueue,runningQueue,in->getPid());
            }
            else break;//这个分支in==nullptr,没有可以换入的。
            break;
        }
        case PREEEM_PRI:{
            Process* in=nullptr,* out=nullptr;
            if(!readyQueue.isEmpty())
            {   //找到优先级最高的
                int min = lowest_pri+1;
                in = readyQueue.at(0);
                for(int i=0; i < readyQueue.size();i++)
                {
                    if(readyQueue.at(i)->getPriority() < min)
                    {
                        in = readyQueue.at(i);
                        min = readyQueue.at(i)->getPriority();
                    }
                }
            }
            if(!runningQueue.isEmpty())
            {   //比较需不需要抢占
                out=runningQueue.at(0);//多个运行队列的话还要挑一个最差的…………待补充

                if(out->getPriority() > in->getPriority())
                {   //高优先级抢占
                    moveProcess(runningQueue,readyQueue,out->getPid());
                    moveProcess(readyQueue,runningQueue,in->getPid());
                }
             }
             else if(in!=nullptr){
                 moveProcess(readyQueue,runningQueue,in->getPid());
             }
             else break;//这个分支in==nullptr,没有可以换入的。
             break;
        }
        case NONPREEM_PRI:{
            if(!runningQueue.isEmpty())
                return;

            //运行队列空了
            Process* in=nullptr;
            if(!readyQueue.isEmpty())
            {   //优先级最高的来
                int min = lowest_pri+1;
                in = readyQueue.at(0);
                for(int i=0; i < readyQueue.size();i++){
                    if(readyQueue.at(i)->getPriority() < min)
                    {
                           in = readyQueue.at(i);
                           min = readyQueue.at(i)->getPriority();

                    }
                }
                moveProcess(readyQueue,runningQueue,in->getPid());
            }
            else break;//这个分支in==nullptr,没有可以换入的。
            break;
        }
    case NSJF:{
                    int i;
                    int min=1000;
                    if(!runningQueue.isEmpty()) return;
                    Process * in=nullptr;
                    if(!readyQueue.isEmpty()){
                        for(i=0;i<readyQueue.size();i++){
                            if(readyQueue.at(i)->getCPUtime()<min){
                                in=readyQueue.at(i);
                                min=readyQueue.at(i)->getCPUtime();
                            }
                        }
                        moveProcess(readyQueue,runningQueue,in->getPid());
                    }
                    break;
                }
    case SJF:{
                        int i;
                        int min=1000;
                        Process * in=nullptr, * out=nullptr;

                        if(readyQueue.isEmpty()) return ;
                        else if(!readyQueue.isEmpty()){                      //找出就绪队列的最小运行时间
                            for(i=0;i<readyQueue.size();i++){
                                if(readyQueue.at(i)->getCPUtime()<min){
                                    in=readyQueue.at(i);
                                    min=readyQueue.at(i)->getCPUtime();
                                }
                            }

                            if(!runningQueue.isEmpty()){
                                if(min<runningQueue.at(0)->getCPUtime()){
                                    out=runningQueue.at(0);
                                    moveProcess(runningQueue,readyQueue,out->getPid());
                                    moveProcess(readyQueue,runningQueue,in->getPid());
                                }
                                else ;
                            }
                            else{
                                moveProcess(readyQueue,runningQueue,in->getPid());
                            }

                        }
                        break;
                    }
        case MFQ:{//多级反馈队列：RR1s，RR2s，FCFS
            if(!readyQueue.isEmpty()){//有新进程
                RR1.append(readyQueue);
                readyQueue.clear();
                for(int i=0;i<RR1.size();i++) RR1.at(i)->setPriority(0);
            }

            Process *in=nullptr,*out=nullptr;

            if(!RR1.isEmpty()){
                in=RR1.at(0);
            }
            else if(!RR2.isEmpty()){
                in=RR2.at(0);
            }
            else if(!FCFS.isEmpty()){
                in=FCFS.at(0);
            }
            if(!runningQueue.isEmpty()) out=runningQueue.at(0);
            else{
                if(in==nullptr)break;
                else{
                    switch (in->getPriority()){
                    case 0:
                        moveProcess(RR1,runningQueue,in->getPid());
                        break;
                    case 1:
                        moveProcess(RR2,runningQueue,in->getPid());
                        timer=0;
                        break;
                    case 2:
                    default:
                        moveProcess(FCFS,runningQueue,in->getPid());
                        break;
                    }
                    break;
                }
            }

            if(out->getPriority()==1) timer++;//时间片计数器
            if(in==nullptr)break;

            if(in->getPriority()<out->getPriority()){//立即抢占
                switch (out->getPriority()) {
                case 0:
                    out->setPriority(1);
                    moveProcess(runningQueue,RR2,out->getPid());
                    break;
                case 1:
                case 2:
                default:
                    out->setPriority(2);
                    moveProcess(runningQueue,FCFS,out->getPid());
                    break;
                }
                switch (in->getPriority()){
                case 0:
                    moveProcess(RR1,runningQueue,in->getPid());
                    break;
                case 1:
                    moveProcess(RR2,runningQueue,in->getPid());
                    timer=0;
                    break;
                case 2:
                default:
                    break;
                }
            }
            else if(in->getPriority()>out->getPriority()){
                ;//不能抢占更高级队列的进程
            }
            else if(in->getPriority()==out->getPriority()){
                switch (out->getPriority()) {
                case 0:
                    out->setPriority(1);
                    moveProcess(runningQueue,RR2,out->getPid());
                    moveProcess(RR1,runningQueue,in->getPid());
                    break;
                case 1:
                    if(timer>=2){
                        out->setPriority(2);
                        moveProcess(runningQueue,FCFS,out->getPid());
                        moveProcess(RR2,runningQueue,in->getPid());
                        timer=0;
                    }
                case 2:
                default:
                    break;
                }
            }
            break;
        }
    case HRRN:{

                int i;
                float R;
                float max=1;
                if(!runningQueue.isEmpty()) return;
                Process * in=nullptr;
                if(!readyQueue.isEmpty()){
                    for(int i=0; i < readyQueue.size();i++){
                        int A=readyQueue.at(i)->getAge()+1;
                        readyQueue.at(i)->setAge(A);
                    }
                    for(i=0;i<readyQueue.size();i++){
                        R=1+(readyQueue.at(i)->getAge())/(readyQueue.at(i)->getCPUtime())+(readyQueue.at(i)->getAge())%(readyQueue.at(i)->getCPUtime());
                        if(R>max){
                            in=readyQueue.at(i);
                            max=R;
                        }
                    }
                    moveProcess(readyQueue,runningQueue,in->getPid());
                }
                break;
            }
        case FCFSa:
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
