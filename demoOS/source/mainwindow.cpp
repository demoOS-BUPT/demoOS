#include "mainwindow.h"
#include "ui_mainwindow.h"

#define CYCLE 1000
#include <QInputDialog>

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
                                           <<"弱化版多级反馈队列"
                                           <<"动态优先级"
                                           <<"抢占式优先级"
                                           <<"非抢占式优先级"
                                           <<"非抢占短作业优先"
                                           <<"短作业优先"
                                           <<"多级反馈队列"
                                           <<"最高响应比"
                                          );
    this->ui->ramAllocAlgComboBox->addItems(QStringList()
                                            <<"首次适配"
                                            <<"最佳适配"
                                            );
    ui->cmd->setTextColor(QColor(80,0,0));
    ui->cmd->setText("DemoOS 正在启动\n");

#define INIT_TABLE(TABLE_NAME) TABLE_NAME->setColumnCount(7);\
    TABLE_NAME->setHorizontalHeaderLabels(QStringList()<<"PID"<<"父进程PID"<<"CPU时间"<<"优先级"<<"内存基地址"<<"内存大小"<<"程序文件路径")
    INIT_TABLE(ui->runTable);
    INIT_TABLE(ui->readyTable);
    INIT_TABLE(ui->RR1T);
    INIT_TABLE(ui->RR2T);
    INIT_TABLE(ui->FCFST);
    ui->waitTable->setColumnCount(5);
    ui->waitTable->setHorizontalHeaderLabels(QStringList()<<"PID"<<"CPU时间"<<"I/O时间"<<"内存基地址"<<"内存大小");
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
    timer.start(CYCLE);

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
            ,new QTableWidgetItem(QUEUE.at(i)->getPpid()==0?"NULL":QString::number(QUEUE.at(i)->getPpid())));\
        TABLE->setItem(i,2\
           ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getCPUtime())));\
        TABLE->setItem(i,3\
           ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getPriority())));\
        TABLE->setItem(i,4\
            ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getBase())));\
        TABLE->setItem(i,5\
            ,new QTableWidgetItem(QString::number(QUEUE.at(i)->getSize())+" B"));\
        TABLE->setItem(i,6\
            ,new QTableWidgetItem(QUEUE.at(i)->getPath()));\
    }
    PRINT_QUEUE(ui->readyTable,readyQueue)
    PRINT_QUEUE(ui->runTable,runningQueue)
    PRINT_QUEUE(ui->RR1T,RR1)
    PRINT_QUEUE(ui->RR2T,RR2)
    PRINT_QUEUE(ui->FCFST,FCFS)

    ui->waitTable->setRowCount(waitQueue.size());
    for(int i=0;i<waitQueue.size();i++){
            ui->waitTable->setItem(i,0
               ,new QTableWidgetItem(QString::number(waitQueue.at(i)->getPid())));
            ui->waitTable->setItem(i,1
               ,new QTableWidgetItem(QString::number(waitQueue.at(i)->getCPUtime())));
            ui->waitTable->setItem(i,2
               ,new QTableWidgetItem(QString::number(waitQueue.at(i)->getIo())));
            ui->waitTable->setItem(i,3
                ,new QTableWidgetItem(QString::number(waitQueue.at(i)->getBase())));
            ui->waitTable->setItem(i,4
                ,new QTableWidgetItem(QString::number(waitQueue.at(i)->getSize())+" B"));
    }
}

void MainWindow::createProcess(int cpuTime,int priority,int ramSize,Directory *fileDir){
    Process* p=newProcess(this->pcbPool);
    if(p!=nullptr){//创建成功
        //p->setCPUtime(cpuTime);
        p->setPriority(priority);
        int base=-1,size=-1;
        //分配内存
        int alg=ui->ramAllocAlgComboBox->currentIndex();

        QString content;
        if(fileDir!=NULL)
        {
            content = QString::fromStdString(
                        dirOp->cat_file(fileDir->get_FCBptr(), diskOP));
            p->setPath(QString::fromStdString(fileDir->get_fileName()));
        }
        else return;

        /*if(fileDir == NULL || content.split(',').size() == 0)
        {
            content = "c,i|5,f,c,c,c";//从文件读出来的字符串们 【指令以,间隔 参数以|间隔
            qDebug()<<"默认程序哈哈："<<content;
        }

         /*CPUTIME的设置*/
        QStringList ins =content.split(',');//命令间以,间隔
        p->setCPUtime(ins.size());

        if(ram.push(p->getPid(),ramSize,alg,content.toLatin1().data())){
            base=ram.PcbMem_base(p->getPid());
            size=ram.PcbMem_size(p->getPid());
        }
        else{
            cmdPrint(QString("创建进程失败：内存分配失败"));
            return;
        }

        p->setBase(base);
        p->setSize(size);

        p->setPpid(0);//不是合法PID 表示没有父亲

        this->readyQueue.append(p);
        cmdPrint(QString("新建进程成功：PID %1 CPU时间：%2 优先级：%3 内存：%4 B 内存基地址:%5")
                    .arg(p->getPid())
                    .arg(p->getCPUtime())
                    .arg(p->getPriority())
                    .arg(p->getSize())
                    .arg(p->getBase())
                 );
        qDebug()<<p->getPpid();
        return;
    }
    else{
        cmdPrint(QString("无法新建进程"));
        return;
    }
}

void MainWindow::kernel(){
    //qDebug()<<"kernel";
    timer.stop();

    //cmdPrint("1000ms CYCLE");
    //cmdPrint("---------------------------------------");
    //if(rand()%(pcbPool.size()+1)==0)
    //    this->createProcess(rand()%10+1,rand()%5);

    ProcessAlg alg=static_cast<ProcessAlg>(ui->processAlgComboBox->currentIndex());
    processDispatch(pcbPool,readyQueue,runningQueue,
                    waitQueue,RR1,RR2,FCFS,ram,alg);//进程调度函数
    execute(pcbPool,runningQueue,readyQueue,waitQueue,this->ram,
            ui->ramAllocAlgComboBox->currentIndex());//进程执行
    ioDispatch(readyQueue,waitQueue);//使用I/0资源

    printQueue();

    timer.start(CYCLE);

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

void MainWindow::cmdPrint_noLn(QString s){
    QString text=ui->cmd->document()->toPlainText();
    QStringList lines=text.split('\n');
    while(lines.size()>255){
        lines.removeFirst();
    }
    text=lines.join('\n');
    //text+="\n";
    text+=s;
    ui->cmd->setText(text);
    ui->cmd->moveCursor(QTextCursor::End);
}

#define DIR_ECHO cmdPrint((QStringList()<<""<<QString::fromStdString(currentUser->get_username())\
            <<"@geek_linux:"<<QString::fromStdString(currentDir)<<"# ").join(""))
void MainWindow::on_pushButton_clicked()//用户指令
{
    QString op=ui->input->toPlainText();
    std::string op1=op.toStdString();

    cmdPrint_noLn(op);//回显
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
    if(argc==0){DIR_ECHO;return;}
    if(args.at(0)=="?" || args.at(0)=="help"){
        cmdPrint("帮助：\n"
                 "\t指令帮助：\n"
                 "\thelp 或 ?: 打印帮助\n"
                 "\tls: ls [路径]* 列出当前路径或指定路径下目录树\n"
                 "\tmkdir: mkdir [路径] 创建目录\n"
                 "\ttouch: touch [文件] 创建文件\n"
                 "\tvim: vim [文件] 编辑文件\n"
                 "\tcat: cat [文件] 显示文件内容\n"
                 "\tcd: cd [路径] 进入目录\n"
                 "\tnpro: npro [-e] (文件名) [-p]* (优先级0-7 默认7) [-s]* (内存大小默认4KB) 创建进程\n"
                 "\tkill: kill [pid] 杀死进程\n");
        DIR_ECHO;return;
    }
    /*if (args.at(0) == "ls" || args.at(0) == "ll") {
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
    }*/
    if (args[0] == "ls") {
                ls_instruction(op1);
    }
    else if (args[0] == "ll") {
        if (args.size() == 1){
            dirOp->ll_directory(curDir);
        }
    }
    else if (args[0] == "mkdir") {
        mkdir_instruction(op1);
    }
    else if (args[0] == "touch") {
        touch_instruction(op1);
    }
    else if (args[0] == "vim") {
        vim_instruction(op1);
    }
    else if (args[0] == "cat") {
        cat_instruction(op1);
    }
    else if (args[0] == "cd") {
        //cd /home/zxh
        //dirName = args[1];
        dirOp->change_directory(args.at(1).toStdString());
        //switch dir
    }
    else if (args[0] == "rm") {
        rm_instruction(op1);
    }
    else if (args[0] == "ln") {
        ln_instruction(op1);
    }
    else if (args[0]=="cp") {
        cp_instruction(op1);

    }
    else if (args[0]=="mv") {
        mv_instruction(op1);

    }
    //User
    else if (args[0] == "whoami") {
        cout << currentUser->get_username() << endl;
    }
    else if (args[0] == "whichgroup") {
        cout << currentUser->get_group() << endl;
    }
    else if (args[0] == "su") {
        su_instruction(op1);
    }
    else if (args[0] == "chmod"){
        chmod_instruction(op1);
    }
    else if (args[0] == "chown"){
        chown_instruction(op1);
    }
    else if (args[0] == "chgrp"){
        chgrp_instruction(op1);
    }
    else if (args.at(0) == "kill") {
        //kill PID
        if(argc != 2){
            cmdPrint("用法: kill [PID]");
            DIR_ECHO;return;
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
        if(argc >7){
            cmdPrint("用法: npro [-e] (文件名) [-p]* (优先级0-7 默认7) [-s]* (内存大小默认4KB)");
            DIR_ECHO;return;
        }

        int cpu_time = -1;
        int prior = -1;
        int size=-1;
        string fileName ="";
        Directory*fileDir =NULL;
        //QString fileContent="";

        int i = 1;

        while(i < argc)
        {
            if(args.at(i) == "-e")
            {
                bool ok;
                //cpu_time=args.at(i+1).toInt(&ok);

               fileName = path_to_filename(args.at(i+1).toStdString());
               fileDir= path_to_directory(args.at(i+1).toStdString());
               if (fileDir == NULL) {
                   cmdPrint(QString("Error! Can't find file %1")
                            .arg(args.at(i+1)));
                   DIR_ECHO;return;
               }
               else{
                   for (int i = 0; i < fileDir->get_fileListNum(); i++) {
                       if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                           fileDir = fileDir->get_fileList(i);
                           break;
                       }
                   }
               }
               if (fileDir->get_type() == '1') {
                   if (!fileDir->is_authority(currentUser->get_username(),
                                              currentUser->get_group(),
                                              "r")) {
                       cmdPrint("Permission denied.");
                       DIR_ECHO;return;
                   }
               }
               else {
                   cmdPrint("Error! Can't find the file.");
                   DIR_ECHO;return;
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
                    DIR_ECHO;return;
                }
                i+=2;
            }
            else if(args.at(i)== "-s"){
                bool ok;
                size=args.at(i+1).toInt(&ok);
                if(!ok||size<0){
                    cmdPrint(QString("参数 \"%1\"无效，内存大小为正整数")
                             .arg(args.at(i+1)));
                    DIR_ECHO;return;
                }
                i+=2;
            }
            else{
                cmdPrint(QString("无法识别参数 \"%1\"").arg(args.at(i)));
                cmdPrint("用法: npro [-e] (文件名称) [-p]* (优先级0-7 默认7) [-s]* (内存大小 默认4KB)");
                DIR_ECHO;return;
            }
        }
        if(cpu_time<=0)cpu_time=this->rand()%10+1;
        if(prior<0)prior=7;
        if(size<0) size=4096;
        if(fileDir==NULL){
            cmdPrint(QString("无法创建进程：没有指定程序文件 ")
                     +QString::fromStdString(fileName));//没有获取到文件
            DIR_ECHO;return;
        }
        this->createProcess(cpu_time,prior,size,fileDir);
    }
    else {
        cmdPrint(QString("%1 不是有效的命令。").arg(args.at(0)));
    }
    DIR_ECHO;
}


void MainWindow::FS_init() {
    string username = "root";
        string password = "toor";
        currentUser = new User();
        userArr = new User[20];
        currentUser->init(username, password, username);
        userArr[0].init(username, password, username);

        dirOp = new DirOperate();
        diskOP=new DiskOperate();
        systemStartAddr = (char*)malloc(system_size * sizeof(char));
        cmdPrint(QString("磁盘大小：%1").arg(system_size));
        //cout << "system size:" << system_size << endl;
        cmdPrint(QString("磁盘块大小：%1").arg(block_size));
        //cout << "block size:" << block_size << endl;
        cmdPrint(QString("磁盘块数目：%1").arg(block_count));
        //cout << "block count:" << block_count << endl;
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
        curDir = root_directory;
        lastDir = NULL;
        currentDir = "/";
        //cout << root_directory << endl;
        //root_fcb = (FCB*)systemStartAddr + block_size * init_FCB_block_num;
        root_fcb = new(systemStartAddr + block_size * init_FCB_block_num)FCB;
        FCB_count++;
        BlockMap = new(systemStartAddr + block_size * init_blockMap_block_num)int[block_count+1];
        bitmap[init_blockMap_block_num] = 1;
        //root_directory = new Directory();//不知道物理地址会不会变
        //cout << root_directory << endl;
        init_blockMap();
        //directory FCB物理上顺序存储 逻辑上链式存储
        //

        dirOp->create_file(path_to_directory("/tmp"),path_to_filename("/tmp"),'0');
        dirOp->create_file(path_to_directory("/root"),path_to_filename("/root"),'0');
        dirOp->create_file(path_to_directory("/home"),path_to_filename("/home"),'0');
        dirOp->create_file(path_to_directory("/home/a.c"), path_to_filename("/home/a.c"), '1');



        LoginResult re=currentUser->is_login_q();
        while(re!=Granted){
            if(re==Aborted) QCoreApplication::quit();
            else re=currentUser->is_login_q();
        }
        QMessageBox::information(this,"Access Granted","User access granted.");
        cmdPrint((QStringList()<<""<<QString::fromStdString(currentUser->get_username())
                      <<"@geek_linux:"<<QString::fromStdString(currentDir)<<"# ").join(""));
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

QString MainWindow::getFileContent(QString fileName){
    string fileName_ = fileName.toStdString();
    Directory*fileDir = lastDir;
    for (int i = 0; i < lastDir->get_fileListNum(); i++) {
         if (fileName_ == lastDir->get_fileList(i)->get_fileName()) {
             fileDir = lastDir->get_fileList(i);
             break;
         }
     }
     return QString::fromStdString(dirOp->cat_file(fileDir->get_FCBptr(), diskOP));
}

QString MainWindow::getFileContent(Directory *fileDir){
     return QString::fromStdString(dirOp->cat_file(fileDir->get_FCBptr(), diskOP));
}

void MainWindow::mkdir_instruction(string op) {
    vector<string> args = split(op, " ");
    string dirName = path_to_filename(args[1]);
    //创建的文件是文件
    Directory*lastDir = path_to_directory(args[1]);
    if (lastDir == NULL) {
        cmdPrint("Error! Can't find the path.");
        DIR_ECHO;return;
    }
    if (-1 == dirOp->create_file(lastDir, dirName, '0')) {
        cmdPrint("Failed in creating files.");
    }
}

void MainWindow::touch_instruction(string op) {
    //创建file
    vector<string> args = split(op, " ");
    Directory*lastDir = path_to_directory(args[1]);
    if (lastDir == NULL) {
        cmdPrint("Error! Can't find the path.");
        DIR_ECHO;return;
    }
    string fileName = path_to_filename(args[1]);
    if (-1 == dirOp->create_file(lastDir, fileName, '1')) {
       cmdPrint("Failed in creating files.");
    }
}

void MainWindow::cat_instruction(string op) {
    vector<string> args = split(op, " ");
    string fileContent;
    string fileName = path_to_filename(args[1]);
    Directory*fileDir = path_to_directory(args[1]);
    if (fileDir == NULL) {
        cmdPrint("Error! Can't find the file.");
        DIR_ECHO;return;
    }
    else {
        //fileName = args[1];
        //Directory*fileDir = curDir;
        for (int i = 0; i < fileDir->get_fileListNum(); i++) {
            if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                fileDir = fileDir->get_fileList(i);
                break;
            }
        }

        if (fileDir->get_type() == '1') {
            if (fileDir->is_authority(currentUser->get_username(), currentUser->get_group(), "r"))
                cmdPrint(QString::fromStdString(
                             dirOp->cat_file(fileDir->get_FCBptr(), diskOP)));
            else {
                //查看文件信息
                cmdPrint("Permission denied.");
                DIR_ECHO;return;
            }
        }
        else {
            cmdPrint("Error! Can't find the file.");
            DIR_ECHO;return;
        }

    }
}

void MainWindow::vim_instruction(string op) {
    vector<string> args = split(op, " ");
    string fileContent;
    //fileName = args[1];
    string fileName = path_to_filename(args[1]);
    Directory*fileDir;
    //fileDir = curDir;
    fileDir = path_to_directory(args[1]);
    if (fileDir == NULL) {
        cmdPrint("Error! Can't find the file.");
        DIR_ECHO;return;
    }
    else {
        for (int i = 0; i < fileDir->get_fileListNum(); i++) {
            if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                fileDir = fileDir->get_fileList(i);
                break;
            }
        }
        if (fileDir->get_type() == '1') {
            if (!fileDir->is_authority(currentUser->get_username(), currentUser->get_group(), "w"))
                cmdPrint("Permission denied." );
            else {
                QString fileContent_q;
                fileContent=dirOp->cat_file(fileDir->get_FCBptr(), diskOP);
                fileContent_q=QString::fromStdString(fileContent);
                cmdPrint("Please input file content:");
                //cin >> fileContent;
                bool ok;
                fileContent_q=QInputDialog::getMultiLineText(this,
                              "Please input file content:","",fileContent_q,&ok);
                if(!ok){DIR_ECHO;return;}
                //getline(cin, fileContent, '\n');
                dirOp->write_file(fileDir->get_FCBptr(), diskOP, fileContent_q.toStdString());
                //写文件
            }
        }
        else {
            cmdPrint("Error! Can't find the file.");
        }
    }
}

void MainWindow::ls_instruction(string op) {
    vector<string> args = split(op, " ");

    //列出目录
    //  ll  || ls ||  ll /zxh/a
    if (args.size() == 1) {
        //ls
        cmdPrint(dirOp->list_directory_q(curDir));
    }
    else if (args[1] == "/"){
        //ls /
        cmdPrint(dirOp->list_directory_q(root_directory));
    }
    else if (args[1] == "*" ) {
        // ls *
        //cout << "." << endl << ".." << endl;
        cmdPrint(dirOp->list_all_directory_q(curDir));
    }
    else if (args[1] == "-a") {
        //ls -a
        cmdPrint(QString(".\n..\n"));
        cmdPrint(dirOp->list_directory_q(curDir));
    }
    else if (args[1] == "-l") {
        //ls -l
        dirOp->ll_directory(curDir);
    }
    else {
        string fileName = path_to_filename(args[1]);
        Directory*fileDir = path_to_directory(args[1]);
        if (fileDir == NULL) {
            cmdPrint("Error! Can't find the file or directory.");
            DIR_ECHO;return;
        }
        for (int i = 0; i < fileDir->get_fileListNum(); i++) {
            if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                fileDir = fileDir->get_fileList(i);
                break;
            }
        }
        if (fileDir == NULL) {
            cmdPrint("Error! Can't find the file or directory.");
        }
        else {
            if (fileDir->get_type() == '1') {
                cmdPrint(QString::fromStdString(fileDir->get_fileName()));
            }
            else
            {
                if (args.size() == 2) {
                    cmdPrint(dirOp->list_directory_q(fileDir));
                }
                else {
                    if (args[2] == "*") {
                        cmdPrint(dirOp->list_all_directory_q(fileDir));
                    }
                    cmdPrint("unidentification!");
                }
            }
        }
    }
}

void MainWindow::cp_instruction(string op) {
    vector<string> args = split(op, " ");

    if (args.size() == 3) {
        //cp file1 file2
        //cp file1 dir
        string sFileName, tFileName;
        sFileName = path_to_filename(args[1]);
        tFileName = path_to_filename(args[2]);
        Directory*sfileDir = NULL, *tfileDir = NULL;
        Directory*slastDir = path_to_directory(args[1]);
        Directory*tlastDir = path_to_directory(args[2]);
        if (slastDir == NULL || tlastDir == NULL) {
            cmdPrint("Error! Can't find the source path.");
            DIR_ECHO;return;
        }
        else {
            for (int i = 0; i < slastDir->get_fileListNum(); i++) {
                if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
                    sfileDir = slastDir->get_fileList(i);
                    break;
                }
            }
            if (sfileDir == NULL) {
                cmdPrint("Error! Can't find the source file.");
                DIR_ECHO;return;
            }
            if (sfileDir->get_type() == '0') {
                cmdPrint("Don't allow copying directory.");
            }
            else {
                bool flag = false;
                for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
                    if (tFileName == tlastDir->get_fileList(i)->get_fileName()) {
                        tfileDir = tlastDir->get_fileList(i);
                        flag = true;
                        break;
                    }
                }
                if (flag == true) {
                    if (tfileDir->get_type() == '0') {
                        //cp file dir
                        dirOp->cp_file(sfileDir, tfileDir, sFileName);
                    }
                    else {
                        //cp file1 file2 file2已经存在
                        dirOp->cp_file(sfileDir, tlastDir, tFileName);
                    }
                }
                else {
                    //cp file1 file2  file2 不存在
                    dirOp->cp_file(sfileDir, tlastDir, tFileName);
                }
            }
        }
    }
    else
    {
        //cp file1 file2…filen dir
        string sFileName, tDirName;
        int fileNum = args.size() - 2;
        tDirName = path_to_filename(args[args.size() - 1]);
        Directory*tlastDir = path_to_directory(args[2]);
        Directory*sfileDir = NULL, *tDir = NULL, *slastDir = NULL;
        if (tlastDir == NULL) {
            cmdPrint("Error! Can't find the objective directory.");
            DIR_ECHO;return;
        }
        bool flag = false;
        for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
            if (tDirName == tlastDir->get_fileList(i)->get_fileName()) {
                tDir = tlastDir->get_fileList(i);
                flag = true;
                break;
            }
        }
        if (flag == false) {
            cmdPrint("Error! Can't find the objective directory. ");
            DIR_ECHO;return;
        }
        else {
            sFileName = path_to_filename(args[1]);
            slastDir = path_to_directory(args[1]);
            if (slastDir == NULL) {
                cmdPrint("Error! Can't find the source path.");
                DIR_ECHO;return;
            }
            for (int i = 1; i <= fileNum; i++) {
                sfileDir = NULL;
                bool flag = false;
                for (int i = 0; i < slastDir->get_fileListNum() && flag == false; i++) {
                    if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
                        sfileDir = slastDir->get_fileList(i);
                        flag = true;
                        break;
                    }
                }
                if (flag == false) {
                    cmdPrint(QString("Error! Can't find the file "
                                     +QString::fromStdString(sFileName)));
                }
                else {
                    if (sfileDir->get_type() == '0') {
                        cmdPrint("Don't allow copying directory.");
                    }
                    else {
                        dirOp->cp_file(sfileDir, tDir, sFileName);
                    }

                }
                sFileName = path_to_filename(args[i + 1]);
                slastDir = path_to_directory(args[i + 1]);
                if (slastDir == NULL) {
                    cmdPrint("Error! Can't find the source path.");
                    DIR_ECHO;return;
                }
            }
        }
    }
}

void MainWindow::mv_instruction(string op) {
    vector<string> args = split(op, " ");
    if (args.size() == 3) {
        //mv file file2
        //mv file dir
        string sFileName, tFileName;
        sFileName = path_to_filename(args[1]);
        tFileName = path_to_filename(args[2]);
        Directory*sfileDir = NULL, *tfileDir = NULL;
        Directory*slastDir = path_to_directory(args[1]);
        Directory*tlastDir = path_to_directory(args[2]);
        if (slastDir == NULL || tlastDir == NULL) {
            cmdPrint("Error! Can't find the source path.");
        }
        else {
            for (int i = 0; i < slastDir->get_fileListNum(); i++) {
                if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
                    sfileDir = slastDir->get_fileList(i);
                    break;
                }
            }
            if (sfileDir == NULL) {
                cmdPrint("Error! Can't find the source file.");
                DIR_ECHO;return;
            }
            if (sfileDir->get_type() == '0') {
                //mv dir1 dir2
                //dirOp->mv_dir(slastDir, sfileDir, tlastDir);
                bool flag = false;
                for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
                    if (tFileName == tlastDir->get_fileList(i)->get_fileName()) {
                        tfileDir = tlastDir->get_fileList(i);
                        flag = true;
                        break;
                    }
                }
                if (flag == true) {
                    if (tfileDir->get_type() == '0') {
                        //mv dir dir
                        dirOp->mv_file(slastDir, sfileDir, tfileDir);
                    }
                    else {
                        //mv file1 file2 file2已经存在
                        cmdPrint( "Error! The instruction format is wrong.");
                    }
                }
                else {
                    cmdPrint("Error! Can' t find the objective directory.");
                }
            }
            else {
                bool flag = false;
                for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
                    if (tFileName == tlastDir->get_fileList(i)->get_fileName()) {
                        tfileDir = tlastDir->get_fileList(i);
                        flag = true;
                        break;
                    }
                }
                if (flag == true) {
                    if (tfileDir->get_type() == '0') {
                        //mv file dir
                        dirOp->mv_file(slastDir, sfileDir, tfileDir);
                    }
                    else {
                        //mv file1 file2 file2已经存在
                        dirOp->rm_directory(tfileDir, tlastDir);
                        sfileDir->set_fileName(tFileName);
                        dirOp->mv_file(slastDir, sfileDir, tlastDir);
                    }
                }
                else {
                    //mv file1 file2  file2 不存在
                    dirOp->mv_file(slastDir, sfileDir, tlastDir);
                    sfileDir->set_fileName(tFileName);
                }
            }
        }
    }
    else
    {
        //mv file1 file2…filen dir
        string sFileName, tDirName;
        int fileNum = args.size() - 2;
        tDirName = path_to_filename(args[args.size() - 1]);
        Directory*tlastDir = path_to_directory(args[2]);
        Directory*sfileDir = NULL, *tDir = NULL, *slastDir = NULL;
        if (tlastDir == NULL) {
            cmdPrint("Error! Can't find the objective path.");
            DIR_ECHO;return;
        }
        bool flag = false;
        for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
            if (tDirName == tlastDir->get_fileList(i)->get_fileName()) {
                tDir = tlastDir->get_fileList(i);
                flag = true;
                break;
            }
        }
        if (flag == false) {
            cmdPrint("Error! Can't find the objective directory. ");
        }
        else {
            sFileName = path_to_filename(args[1]);
            slastDir = path_to_directory(args[1]);
            if (slastDir == NULL) {
                cmdPrint("Error! Can't find the source file.");
                DIR_ECHO;return;
            }
            for (int i = 1; i <= fileNum; i++) {
                sfileDir = NULL;
                bool flag = false;
                for (int i = 0; i < slastDir->get_fileListNum() && flag == false; i++) {
                    if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
                        sfileDir = slastDir->get_fileList(i);
                        flag = true;
                        break;
                    }
                }
                if (flag == false) {
                    cmdPrint("Error! Can't find the file "+QString::fromStdString(sFileName));
                }
                else {
                    if (sfileDir->get_type() == '0') {
                        cmdPrint("Don't allow copying directory.");
                    }
                    else {
                        dirOp->mv_file(slastDir, sfileDir, tDir);
                    }

                }
                sFileName = path_to_filename(args[i + 1]);
                slastDir = path_to_directory(args[i + 1]);
                if (slastDir == NULL) {
                    cmdPrint("Error! Can't find the source path.");
                    DIR_ECHO;return;
                }
            }
        }
    }
}

void MainWindow::rm_instruction(string op) {
    vector<string> args = split(op, " ");
    string fileName = path_to_filename(args[1]);
    Directory*fileDir = NULL, *lastDir = path_to_directory(args[1]);
    if (lastDir == NULL) {
        cmdPrint("Error! Can't find the file.");
    }
    else {
        for (int i = 0; i < lastDir->get_fileListNum(); i++) {
            if (fileName == lastDir->get_fileList(i)->get_fileName()) {
                fileDir = lastDir->get_fileList(i);
                break;
            }
        }
        if (fileDir == NULL) {
            cmdPrint("Error! Can't find the file.");
            DIR_ECHO;return;
        }
        dirOp->rm_directory(fileDir, lastDir);

    }
}

void MainWindow::ln_instruction(string op) {
    vector<string> args = split(op, " ");
    string sFileName, tFileName;
    sFileName = path_to_filename(args[1]);
    tFileName = path_to_filename(args[2]);
    Directory*sfileDir = NULL;
    Directory*slastDir = path_to_directory(args[1]);
    Directory*tlastDir = path_to_directory(args[2]);
    if (slastDir == NULL) {
        cmdPrint("Error! Can't find the source path.");
    }
    else if (tlastDir == NULL)
    {
        cmdPrint("Error! Can't find the objective path.");
    }
    else {
        for (int i = 0; i < slastDir->get_fileListNum(); i++) {
            if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
                sfileDir = slastDir->get_fileList(i);
                break;
            }
        }
        if (sfileDir == NULL) {
            cmdPrint("Error! Can't find the source file.");
            DIR_ECHO;return;
        }
        if (sfileDir->get_type() == '0') {
            cmdPrint("Don't allow making hard link to directory.");
        }
        else {
            cmdPrint(dirOp->ln_q(sfileDir, tlastDir, tFileName));
        }

    }
}

void MainWindow::su_instruction(string op) {
    vector<string> args = split(op, " ");
    string username = args[1];
    string password;
    cmdPrint("Password:");
    //cin >> password;
    bool ok;
    QString pw=QInputDialog::getText(this,"Input password",
                                     "Password:",QLineEdit::Password,"",&ok);
    if(!ok){
        cmdPrint("Canceled");
        DIR_ECHO;return;
    }
    password=pw.toStdString();
    //getline(cin, password, '\n');
    if (currentUser->change_user(username, password))
        cmdPrint("Success");
    else
        cmdPrint("Error");
}

void MainWindow::chmod_instruction(string op) {
    vector<string> args = split(op, " ");
    string fileName = path_to_filename(args[2]);
    Directory*fileDir = path_to_directory(args[2]);
    if (fileDir == NULL) {
        cmdPrint("Error! Can't find the file.");
    }
    else {
        //fileName = args[1];
        //Directory*fileDir = curDir;
        for (int i = 0; i < fileDir->get_fileListNum(); i++) {
            if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                fileDir = fileDir->get_fileList(i);
                break;
            }
        }
        if (fileDir == NULL) {
            cmdPrint("Error! Can't find the file.");
        }
        int auth[3];
        string authStr = args[1];
        auth[0] = (atoi(authStr.c_str()) - atoi(authStr.c_str()) % 100) / 100;
        auth[1] = (atoi(authStr.c_str()) - auth[0] * 100 - atoi(authStr.c_str()) % 10) / 10;
        auth[2] = atoi(authStr.c_str()) % 10;
        fileDir->set_authority(auth);
    }
}

void MainWindow::chown_instruction(string op) {
    vector<string> args = split(op, " ");
    string fileName = path_to_filename(args[1]);
    Directory*fileDir = path_to_directory(args[1]);
    if (fileDir == NULL) {
        cmdPrint("Error! Can't find the file.");
    }
    else {
        for (int i = 0; i < fileDir->get_fileListNum(); i++) {
            if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                fileDir = fileDir->get_fileList(i);
                break;
            }
        }
        if (fileDir == NULL) {
            cmdPrint("Error! Can't find the file.");
            DIR_ECHO;return;
        }
        fileDir->set_owner(args[2]);
    }
}

void MainWindow::chgrp_instruction(string op) {
    vector<string> args = split(op, " ");
    string fileName = path_to_filename(args[1]);
    Directory*fileDir = path_to_directory(args[1]);
    if (fileDir == NULL) {
        cmdPrint("Error! Can't find the file.");
    }
    else {
        for (int i = 0; i < fileDir->get_fileListNum(); i++) {
            if (fileName == fileDir->get_fileList(i)->get_fileName()) {
                fileDir = fileDir->get_fileList(i);
                break;
            }
        }
        if (fileDir == NULL) {
            cmdPrint("Error! Can't find the file.");
            DIR_ECHO;return;
        }
        fileDir->set_group(args[2]);
    }
}

