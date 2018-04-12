#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->processAlgComboBox->addItems(QStringList()<<"RR 时间片轮转"<<"FCFS 批处理"<<"优先级+RR"<<"动态优先级"<<"抢占式优先级"<<"非抢占式优先级");
    ui->cmd->setTextColor(QColor(80,0,0));
    ui->cmd->setText("DemoOS 正在启动\n");

    this->pcbPool.clear();
    this->waitQueue.clear();
    this->readyQueue.clear();
    this->runningQueue.clear();//清空进程

    connect(&timer,SIGNAL(timeout()),this,SLOT(kernel()));
    timer.start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createProcess(int cpuTime){
    Process* p=newProcess(this->pcbPool);
    if(p!=nullptr){//创建成功
        p->setCPUtime(cpuTime);
        this->readyQueue.append(p);
        cmdPrint(QString("新建进程：PID %1 CPU时间： %2")
                    .arg(p->getPid()).arg(p->getCPUtime()));
        return;
    }
    else{
        cmdPrint(QString("新建进程：无法新建进程"));
        return;
    }
}

void MainWindow::kernel(){
    timer.stop();

    //cmdPrint("1000ms CYCLE");

    cmdPrint("---------------------------------------");
    if(rand()%(pcbPool.size()+1)==0)
        this->createProcess(rand()%10+1);

    ProcessAlg alg=static_cast<ProcessAlg>(ui->processAlgComboBox->currentIndex());
    processDispatch(pcbPool,readyQueue,runningQueue,waitQueue,alg);//进程调度函数
    execute(pcbPool,runningQueue);//进程执行

    QString s("所有进程：\n");
    s+=printQue(pcbPool);
    cmdPrint(s);
    s="就绪：\n";
    s+=printQue(readyQueue);
    cmdPrint(s);
    s="运行：\n";
    s+=printQue(runningQueue);
    cmdPrint(s);

    timer.start(1000);

}

void MainWindow::cmdPrint(QString newLine){
    QString text=ui->cmd->document()->toPlainText();
    QStringList lines=text.split('\n');
    while(lines.size()>255){
        lines.removeFirst();
    }
    text=lines.join('\n');
    text+="\n";
    text+=newLine;
    ui->cmd->setText(text);
    ui->cmd->moveCursor(QTextCursor::End);
}

