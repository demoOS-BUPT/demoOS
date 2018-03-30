#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cmd->setTextColor(QColor(80,0,0));
    ui->cmd->setText("DemoOS 正在启动\n");
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
