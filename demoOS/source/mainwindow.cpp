#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->processAlgComboBox->addItems(QStringList()
                                           <<"RR 时间片轮转"
                                           <<"FCFS 批处理"
                                           <<"优先级+RR"
                                           <<"动态优先级"
                                           <<"抢占式优先级"
                                           <<"非抢占式优先级"
                                          );
    ui->cmd->setTextColor(QColor(80,0,0));
    ui->cmd->setText("DemoOS 正在启动\n");

    ui->readyTable->setColumnCount(3);
    ui->readyTable->setHorizontalHeaderLabels(QStringList()
                                              <<"PID"
                                              <<"CPU时间"
                                              <<"优先级");
    ui->runTable->setColumnCount(3);
    ui->runTable->setHorizontalHeaderLabels(QStringList()
                                              <<"PID"
                                              <<"CPU时间"
                                              <<"优先级");

    this->pcbPool.clear();
    this->waitQueue.clear();
    this->readyQueue.clear();
    this->runningQueue.clear();//清空进程

    FS_init();//磁盘子系统初始化

    connect(&timer,SIGNAL(timeout()),this,SLOT(kernel()));
    timer.start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printQueue(){
    ui->readyTable->setRowCount(readyQueue.size());
    for(int i=0;i<readyQueue.size();i++){
        ui->readyTable->setItem(i,0
           ,new QTableWidgetItem(QString::number(readyQueue.at(i)->getPid())));
        ui->readyTable->setItem(i,1
           ,new QTableWidgetItem(QString::number(readyQueue.at(i)->getCPUtime())));
        ui->readyTable->setItem(i,2
           ,new QTableWidgetItem(QString::number(readyQueue.at(i)->getPriority())));
    }
    ui->runTable->setRowCount(runningQueue.size());
    for(int i=0;i<runningQueue.size();i++){
        ui->runTable->setItem(i,0
           ,new QTableWidgetItem(QString::number(runningQueue.at(i)->getPid())));
        ui->runTable->setItem(i,1
           ,new QTableWidgetItem(QString::number(runningQueue.at(i)->getCPUtime())));
        ui->runTable->setItem(i,2
           ,new QTableWidgetItem(QString::number(runningQueue.at(i)->getPriority())));
    }
}

void MainWindow::createProcess(int cpuTime,int priority){
    Process* p=newProcess(this->pcbPool);
    if(p!=nullptr){//创建成功
        p->setCPUtime(cpuTime);
        p->setPriority(priority);
        this->readyQueue.append(p);
        cmdPrint(QString("新建进程：PID %1 CPU时间： %2 优先级：%3")
                    .arg(p->getPid())
                    .arg(p->getCPUtime())
                    .arg(p->getPriority())
                 );
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
    //cmdPrint("---------------------------------------");
    if(rand()%(pcbPool.size()+1)==0)
        this->createProcess(rand()%10+1,rand()%5);

    ProcessAlg alg=static_cast<ProcessAlg>(ui->processAlgComboBox->currentIndex());
    processDispatch(pcbPool,readyQueue,runningQueue,waitQueue,alg);//进程调度函数
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
    QStringList args =op.split(" ");
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
                 "\tcd: cd [路径] 进入目录\n");
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
        cmdPrint("输入文件内容:");
        QString fileContent;
        //cin >> fileContent;
        Directory*fileDir=lastDir;
        for (int i = 0; i < lastDir->get_fileListNum(); i++) {
           if (fileName == lastDir->get_fileList(i)->get_fileName()) {
               fileDir = lastDir->get_fileList(i);
               break;
           }
        }
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
        if(!termiProcess(this->pcbPool,this->readyQueue,this->waitQueue,this->runningQueue,args.at(1).toInt()))
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
        if(argc < 2){
            cmdPrint("用法: npro [-t](CPU时间 默认随机) [-p]（优先级0-7 默认7）");
            return;
        }

        int cpu_time = rand()%10+1;
        int prior = 7;

        for(int i = 1; i < argc; ++i)
        {
            if(args.at(i) == "-t")
            {
                if(!strIsDigit(args.at(++i)))
                {
                    cmdPrint("cpu时间需为整数型");
                    return;
                }
                else
                {
                    cpu_time = args.at(i).toInt();
                }
            }
            else if(args.at(i) == "-p")
            {
                if(!strIsDigit(args.at(++i)))
                {
                    cmdPrint("优先级需为0-7间的整数");
                    return;
                }
                else
                {   int temp = args.at(i).toInt();
                    if(temp >= 0 && temp <= 7)
                    {
                        prior = temp;
                    }
                    else
                    {
                        cmdPrint("优先级需为0至7间的整数");
                    }
                }
            }
            else
            {    cmdPrint("用法: npro [-t](CPU时间 默认随机) [-p]（优先级0-7 默认7）");
                 return;
            }
        }
        this->createProcess(cpu_time,prior);
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


int strIsDigit(QString str)
{
    QByteArray t = str.toLatin1();
    const char *s = t.data();

    while(*s && *s >= '0' && *s <= '9')s++;

    if(*s)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
