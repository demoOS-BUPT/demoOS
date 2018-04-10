#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cmd->setTextColor(QColor(80,0,0));
    ui->cmd->setText("DemoOS 正在启动\n");

    this->lastPID = -1;
    this->pcbPool.clear();
    this->pnewProQue.clear();
    this->pwaitProQue.clear();
    this->preadyProQue.clear();

    connect(&timer,SIGNAL(timeout()),this,SLOT(kernel()));
    timer.start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::kernel(){
    timer.stop();

    cmdPrint("1000ms CYCLE");

    //每个时间片建个进程吼吼，但是新建进程的信息不足###################################################
    newProcess(this->pcbPool,this->pnewProQue,this->lastPID);

    qDebug()<<"new";
    printQue(this->pnewProQue);

    timer.start(1000);

    chgState(this->pnewProQue, this->pwaitProQue,0);


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
