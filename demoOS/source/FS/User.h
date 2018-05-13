#pragma once
#ifndef USER_H
#define USER_H

#include<iostream>
#include<string>

#include<QtCore>
//#include<QMessageBox>
#include"logindialog.h"

using namespace std;

enum LoginResult{Aborted,Granted,BadPW};

typedef class User {
private:
	string userName;
	string password;
	string group;
public:
	User();
	void init(string userName, string password, string group);

	string get_username();
	string get_password();
	string get_group();
	bool is_login();
    LoginResult is_login_q();
	bool change_user(string userName, string password);
}User;

#endif // !USER_H
