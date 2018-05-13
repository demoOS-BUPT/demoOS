#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent,string* u_,string* pw_) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    u(u_),pw(pw_)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    *u=ui->userIn->text().toStdString();
    *pw=ui->pwIn->text().toStdString();
    this->accept();
}

void LoginDialog::on_buttonBox_rejected()
{
    this->reject();
}
