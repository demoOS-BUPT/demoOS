#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ram(ramSize)
{
    ui->setupUi(this);
    runGif=new QMovie(":/run1.gif");
    stopGif=new QMovie(":/stop3.gif");
    runGif->start();
    stopGif->start();
    ui->timerIcon->setMovie(runGif);
    this->ui->processAlgComboBox->addItems(QStringList()
                                           <<"RR 时间片轮转"
                                           <<"FCFS 批处理"
                                           <<"优先级+RR"
                                           <<"动态优先级"
                                           <<"抢占式优先级"
                                           <<"非抢占式优先级"
                                           <<"非抢占短作业优先"
                                           <<"短作业优先"
                                           <<"多级反馈队列"
                                           <<"最高响应比"
                                          );
    ui->cmd->setTextColor(QColor(80,0,0));
    ui->cmd->setText("DemoOS 正在启动\n");

#define INIT_TABLE(TABLE_NAME) TABLE_NAME->setColumnCount(5);\
    TABLE_NAME->setHorizontalHeaderLabels(QStringList()<<"PID"<<"CPU时间"<<"优先级"<<"内存基地址"<<"内存大小")
    INIT_TABLE(ui->runTable);
    INIT_TABLE(ui->readyTable);
    INIT_TABLE(ui->RR1T);
    INIT_TABLE(ui->RR2T);
    INIT_TABLE(ui->FCFST);
    ui->RR1T->hide();
    ui->RR2T->hide();
    ui->FCFST->hide();

    this->pcbPool.clear();
    this->waitQueue.clear();
    this->readyQueue.clear();
    this->runningQueue.clear();//清空进程
    RR1.clear();RR2.clear();FCFS.clear();

    cmdPrint(QString("内存大小：%1B").arg(ramSize));

    FS_init();//磁盘子系统初始化


    //connect(ui->pauseButton,SIGNAL(clicked()),this,SLOT(on_pauseButton_clicked()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(kernel()));
    timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printQueue(){
#define PRINT_QUEUE(TABLE,QUEUE)    TABLE->setRowCount(QUEUE.size());\
    for(int i=0;i<QUEUE.size();i++){\
        TABLE->setItem(i,0\
           ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getPid())));\
        TABLE->setItem(i,1\
           ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getCPUtime())));\
        TABLE->setItem(i,2\
           ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getPriority())));\
        TABLE->setItem(i,3\
            ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getBase())));\
        TABLE->setItem(i,4\
            ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getSize())+" B"));\
    }
    PRINT_QUEUE(ui->readyTable,readyQueue)
    PRINT_QUEUE(ui->runTable,runningQueue)
    PRINT_QUEUE(ui->RR1T,RR1)
    PRINT_QUEUE(ui->RR2T,RR2)
    PRINT_QUEUE(ui->FCFST,FCFS)
}

void MainWindow::createProcess(int cpuTime,int priority,int ramSize){
    Process* p=newProcess(this->pcbPool);
    if(p!=nullptr){//创建成功
        p->setCPUtime(cpuTime);
        p->setPriority(priority);
        int base=-1,size=-1;
        //分配内存
        if(ram.push(p->getPid(),ramSize,0)){
            base=ram.PcbMem_base(p->getPid());
            size=ram.PcbMem_size(p->getPid());
        }
        else{
            cmdPrint(QString("创建进程失败：内存分配失败"));
            return;
        }

        p->setBase(base);
        p->setSize(size);
        this->readyQueue.append(p);
        cmdPrint(QString("新建进程：PID %1 CPU时间：%2 优先级：%3 内存：%4 B 内存基地址:%5")
                    .arg(p->getPid())
                    .arg(p->getCPUtime())
                    .arg(p->getPriority())
                    .arg(p->getSize())
                    .arg(p->getBase())
                 );
        return;
    }
    else{
        cmdPrint(QString("无法新建进程"));
        return;
    }
}

void MainWindow::kernel(){
    qDebug()<<"kernel";
    timer.stop();

    //cmdPrint("1000ms CYCLE");
    //cmdPrint("---------------------------------------");
    //if(rand()%(pcbPool.size()+1)==0)
    //    this->createProcess(rand()%10+1,rand()%5);

    ProcessAlg alg=static_cast<ProcessAlg>(ui->processAlgComboBox->currentIndex());
    processDispatch(pcbPool,readyQueue,runningQueue,
                    waitQueue,RR1,RR2,FCFS,ram,alg);//进程调度函数
    execute(pcbPool,runningQueue);//进程执行

    printQueue();

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

void MainWindow::on_pushButton_clicked()//用户指令
{
    QString op=ui->input->toPlainText();

    cmdPrint(">"+op);//回显
    ui->input->clear();
    //磁盘交互
    QStringList args =op.split(QRegularExpression("\\s+"),QString::SkipEmptyParts);
    qDebug()<<args;
    //		string args[] = op.split(" ");
             //这里加多空格容错
    int argc=args.size();
    /*cmdPrint(QString("argc=%1").arg(argc));
    for(int i=0;i<args.size();i++){
        cmdPrint(args.at(i));
    }
    cmdPrint("------------------------");*/
    if(argc==0)return;
    if(args.at(0)=="?" || args.at(0)=="help"){
        cmdPrint("帮助：\n"
                 "\t表示说明：[参数] [可选参数]*\n"
                 "\t指令帮助：\n"
                 "\thelp 或 ?: 打印帮助\n"
                 "\tls 或 ll: ls [路径]* 列出当前路径或指定路径下目录树\n"
                 "\tmkdir: mkdir [路径] 创建目录\n"
                 "\ttouch: touch [文件] 创建文件\n"
                 "\tvim: vim [文件] 编辑文件\n"
                 "\tcat: cat [文件] 显示文件内容\n"
                 "\tcd: cd [路径] 进入目录\n"
                 "\tnpro: npro [-t] (CPU时间 默认随机1~10) [-p] (优先级0-7 默认7) 创建进程\n"
                 "\tkill: kill [pid] 杀死进程\n");
        return;
    }
    if (args.at(0) == "ls" || args.at(0) == "ll") {
        //列出目录
        //  ll  || ls ||  ll /zxh/a
        if (argc == 1) {
            cmdPrint(dirOp->list_all_directory_qt(root_directory));
        }
        else {
            dirOp->change_directory(args.at(1).toStdString());
            cmdPrint(dirOp->list_all_directory_qt(lastDir));
        }
    }
    else if (args.at(0) == "mkdir") {
        if(argc<2){
            cmdPrint("usage: mkdir [path]");
            return;
        }
       string dirName = path_to_filename(args.at(1).toStdString());
        //创建的文件是文件
       if (-1 == dirOp->create_file(path_to_directory(args.at(1).toStdString()),dirName,'0')) {
       cmdPrint("创建文件失败");
       }
    }
    else if (args.at(0) == "touch") {
       //创建file
        if(argc<2){
            cmdPrint("用法: touch [文件]");
            return;
        }
        string fileName = path_to_filename(args.at(1).toStdString());
        if (-1 == dirOp->create_file(path_to_directory(args.at(1).toStdString()), fileName, '1')) {
            cmdPrint("创建文件失败");
        }
    }
    else if (args.at(0) == "vim") {
        if(argc<2){
            cmdPrint("用法: vim [文件]");
            return;
        }
        string fileName = args.at(1).toStdString();
        //cin >> fileContent;
        Directory*fileDir=lastDir;
        for (int i = 0; i < lastDir->get_fileListNum(); i++) {
           if (fileName == lastDir->get_fileList(i)->get_fileName()) {
               fileDir = lastDir->get_fileList(i);
               break;
           }
        }
        QString fileContent=QString::fromStdString(dirOp->cat_file(fileDir->get_FCBptr(), diskOP));
        qDebug()<<"reached file:"+fileContent;
        cmdPrint("输入文件内容...");
        bool ok;
        fileContent=QInputDialog::getMultiLineText(this,"输入文件内容...","",fileContent,&ok);
        qDebug()<<"write to file:"+fileContent;
        if(!ok)return;
        dirOp->write_file(fileDir->get_FCBptr(), diskOP, fileContent.toStdString());
          //写文件
    }
    else if (args.at(0) == "cat") {
        if(argc<2){
            cmdPrint("用法: cat [文件]");
            return;
        }
       string fileName = args.at(1).toStdString();
       Directory*fileDir = lastDir;
       for (int i = 0; i < lastDir->get_fileListNum(); i++) {
            if (fileName == lastDir->get_fileList(i)->get_fileName()) {
                fileDir = lastDir->get_fileList(i);
                break;
            }
        }
        cmdPrint(QString::fromStdString(
                     dirOp->cat_file(fileDir->get_FCBptr(), diskOP)));
        //查看文件信息
    }
    else if (args.at(0) == "cd") {
        //cd /home/zxh
        //dirName = args[1];
        if(argc<2){
            cmdPrint("用法: cd [路径]");
            return;
        }
       dirOp->change_directory(args.at(1).toStdString());
        //switch dir
    }
    else if (args.at(0) == "kill") {
        //kill PID
        if(argc != 2){
            cmdPrint("用法: kill [PID]");
            return;
        }
        if(!termiProcess(this->pcbPool,this->readyQueue,this->waitQueue,this->runningQueue,
                         RR1,RR2,FCFS,ram,args.at(1).toInt()))
        {
            cmdPrint("该进程不存在，请重新输入");
        }
        else
        {
            cmdPrint("终止进程成功！");
        }
    }
    else if (args.at(0) == "npro") {
        //create process
        if(argc >5){
            cmdPrint("用法: npro [-t]* (CPU时间 默认随机1~10) [-p]* (优先级0-7 默认7)");
            return;
        }

        int cpu_time = -1;
        int prior = -1;
        int size=-1;

        int i = 1;
        while(i < argc)
        {
            if(args.at(i) == "-t")
            {
                bool ok;
                cpu_time=args.at(i+1).toInt(&ok);
                if(!ok||cpu_time<=0)
                {
                    cmdPrint(QString("参数 \"%1\"无效，cpu时间需为正整数")
                             .arg(args.at(i+1)));
                    return;
                }
                i+=2;
            }
            else if(args.at(i) == "-p")
            {
                bool ok;
                prior=args.at(i+1).toInt(&ok);
                if(!ok||prior<0||prior>7)
                {
                    cmdPrint(QString("参数 \"%1\"无效，优先级为0~7的整数")
                             .arg(args.at(i+1)));
                    return;
                }
                i+=2;
            }
            else if(args.at(i)== "-s"){
                bool ok;
                size=args.at(i+1).toInt(&ok);
                if(!ok||size<0){
                    cmdPrint(QString("参数 \"%1\"无效，内存大小为正整数")
                             .arg(args.at(i+1)));
                    return;
                }
                i+=2;
            }
            else{
                cmdPrint(QString("无法识别参数 \"%1\"").arg(args.at(i)));
                cmdPrint("用法: npro [-t]* (CPU时间 默认随机1~10) [-p]* (优先级0-7 默认7) [-s]* (内存大小 默认4KB)");
                return;
            }
        }
        if(cpu_time<=0)cpu_time=this->rand()%10+1;
        if(prior<0)prior=7;
        if(size<0) size=4096;
        this->createProcess(cpu_time,prior,size);
        cmdPrint("创建进程成功！");
    }
    else {
        cmdPrint(QString("%1 不是有效的命令。").arg(args.at(0)));
    }

}


void MainWindow::FS_init() {
    dirOp = new DirOperate();
    diskOP=new DiskOperate();
    systemStartAddr = (char*)malloc(system_size * sizeof(char));
    cmdPrint(QString("磁盘容量:%1 B").arg(system_size));
    cmdPrint(QString("块尺寸:%1 B").arg(block_size));
    cmdPrint(QString("块数目:%1 块").arg(block_count));
    //初始化盘块的位示图
    memset(systemStartAddr, 0, system_size * sizeof(char));
    //前三块分别是 bit图，目录项，fcb
    bitmap = systemStartAddr;
    for(int i=0;i<init_blockMap_block_num+5;i++)
        bitmap[i] = 1;
    //void*buf= systemStartAddr + block_size * init_directory_block_num;
    //cout <<( systemStartAddr + block_size * init_directory_block_num )<< endl;
    //cout << buf << endl;
    //root_directory = new(systemStartAddr + block_size * init_directory_block_num)Directory;
    directory_count = 0;
    FCB_count = 0;
    //root_directory = (Directory *)systemStartAddr + block_size * init_directory_block_num;
    root_directory = new(systemStartAddr + block_size * init_directory_block_num)Directory;
    directory_count++;
    root_directory->set_fileName("/");
    root_directory->set_type('0');
    lastDir = root_directory;
    //currentDir = "/";
    //cout << root_directory << endl;
    //root_fcb = (FCB*)systemStartAddr + block_size * init_FCB_block_num;
    root_fcb = new(systemStartAddr + block_size * init_FCB_block_num)FCB;
    FCB_count++;
    BlockMap = new(systemStartAddr + block_size * init_blockMap_block_num)int[block_count+1];
    //root_directory = new Directory();//不知道物理地址会不会变
    //cout << root_directory << endl;
    init_blockMap();
    //directory FCB物理上顺序存储 逻辑上链式存储
    //创建目录 /home
    //创建目录 /home/www
    //创建文件 /home/www/in.c
    //创建文件 /home/out.c
}

void MainWindow::on_pauseButton_clicked()
{
    qDebug()<<"pause";
    if(timer.isActive()){
        qDebug()<<"stop";
        ui->timerIcon->setMovie(stopGif);
        ui->pauseButton->setText("继续");
        timer.stop();
    }
    else{
        qDebug()<<"start";
        ui->timerIcon->setMovie(runGif);
        ui->pauseButton->setText("时停");
        timer.start(1000);
    }
}

void MainWindow::on_processAlgComboBox_currentIndexChanged(int index)
{
    if(index==8){
        ui->readyTable->hide();
        ui->RR1T->show();
        ui->RR2T->show();
        ui->FCFST->show();
    }
    else{
        ui->readyTable->show();
        ui->RR1T->hide();
        ui->RR2T->hide();
        ui->FCFST->hide();
    }
}
