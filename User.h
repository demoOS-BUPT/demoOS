#pragma once
#ifndef USER_H
#define USER_H

#include<iostream>
#include<string>

using namespace std;

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
}User;

#endif // !USER_H
