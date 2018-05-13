#include"other.h"
#include"User.h"
#include <string>

using namespace std;

User::User() {
	this->userName = "test";
	this->password = "test";
	this->group = "www-data";
}

void User::init(string userName, string password, string group) {
	this->userName = userName;
	this->password = password;
	this->group = group;
}
/*
string FCB::get_expandName() {
	return expandName;
}
*/
string User::get_username() {
	return userName;
}

string User::get_password() {
	return password;
}

string User::get_group() {
	return group;
}

bool User::is_login(){
	string userName, password;
	cout << "Geek Linux login:";
	getline(cin, userName, '\n');
	//cin >> userName;
	cout << "Password:";
	//cin >> password;
	getline(cin, password, '\n');
	for(int i=0;i<=5;i++){
		if (userArr[i].get_username() == userName && userArr[i].get_password() == password){
			currentUser->init(userArr[i].get_username(), userArr[i].get_password(), userArr[i].get_group());
			cout << "Login access" << endl;
			return true;
		}
	}
	cout << "Access Denied" << endl;
	return false;
}

LoginResult User::is_login_q(){
    string userName, password;
    LoginDialog dia(0,&userName,&password);
    dia.exec();
    if(dia.result()==QDialog::Rejected){
        return Aborted;
    }
    for(int i=0;i<=5;i++){
        if (userArr[i].get_username() == userName && userArr[i].get_password() == password){
            currentUser->init(userArr[i].get_username(), userArr[i].get_password(), userArr[i].get_group());
            //QMessageBox::information(&dia,"Login access","User login access granted.");
            return Granted;
        }
    }
    return BadPW;
}

bool User::change_user(string userName, string password){
	for(int i=0;i<=5;i++){
		if (userArr[i].get_username() == userName && userArr[i].get_password() == password){
			currentUser->init(userArr[i].get_username(), userArr[i].get_password(), userArr[i].get_group());
			return true;
		}
	}
	return false;
}
