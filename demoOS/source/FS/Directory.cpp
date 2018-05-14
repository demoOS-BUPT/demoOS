#include"Directory.h"
#include <ctime>
using namespace std;
#pragma warning( disable : 4996 )
Directory::Directory() {
	fileName = "";
	type = -1;
	FCBptr = NULL;
	for (int i = 0; i < 20; i++) {
		fileList[i] = NULL;
	}
	fileListNum = 0;
	curDir = NULL;
	lastDir = NULL;
	owner = "root";
	group = "root";
	authority[0] = 7;
	authority[1] = 4;
	authority[2] = 4;
}
Directory::~Directory() {

}

string Directory::get_fileName() {
	return fileName;
}
void Directory::set_fileName(string newFileName) {
	fileName = newFileName;
}
char Directory::get_type() {
	return type;
}
void Directory::set_type(char newType) {
	type = newType;
}

FCB*Directory::get_FCBptr() {
	return FCBptr;
}
void Directory::set_FCBptr(FCB* newFCBptr) {
	FCBptr = newFCBptr;
}
Directory* Directory::get_fileList(int i) {
	return fileList[i];
}
void Directory::set_fileList(Directory*newFileList, int i) {
	if (i < DIRECTORY_MAX&&i >=0) {
		fileList[i] = newFileList;
	}
}
void Directory::set_all_fileList(Directory*newFileList[]) {
	for (int i = 0; i < DIRECTORY_MAX;i++) {
		fileList[i] = newFileList[i];
	}
}

int Directory::get_fileListNum() {
	return fileListNum;
}

void Directory::set_fileListNum(int newFileListNum) {
	fileListNum = newFileListNum;
}

bool  Directory::add_fileDirectory(Directory*directory) {
	if (fileListNum < DIRECTORY_MAX) {
		fileList[fileListNum] = directory;
		fileListNum++;
		return true;
	}
	else {
		return false;
	}
}

Directory* Directory::get_curDir() {
	return curDir;
}
void Directory::set_curDir(Directory*newCurDir) {
	curDir = newCurDir;
}
Directory* Directory::get_lastDir() {
	return lastDir;
}
void Directory::set_lastDir(Directory*newLastDir) {
	lastDir = newLastDir;
}

string Directory::get_owner(){
	return this->owner;
}

string Directory::get_group(){
	return this->group;
}

string Directory::get_authority(){
	string autho = "";
	if (this->type == 0)
		autho = "d";
	else
		autho = "-";

	for (int i=0;i<3;i++){
		if (this->authority[i] >= 4)
			autho += "r";
		else
			autho += "-";
		if ((this->authority[i]%4) >= 2)
			autho += "w";
		else
			autho += "-";
		if ((this->authority[i]%2) >= 1)
			autho += "x";
		else
			autho += "-";
	}
	return autho;
}
int *Directory::get_authority_value() {
	return this->authority;
}

string Directory::get_change_time(){
	time_t t;
	tm* local;
	char buf[64];
	t = time(NULL);
	local = localtime(&t);
	strftime(buf, 64, "%B  %d %H:%M", local);
	return buf;//this->changeTime;
}

void Directory::set_owner(string owner){
	this->owner = owner;
}
void Directory::set_group(string group){
	this->group = group;
}
void Directory::set_change_time(){
	this->changeTime = time(NULL);
}
void Directory::set_change_time(string newTime) {
	this->changeTime = newTime;
}
	

void Directory::set_authority(int x[]){
	for(int i=0;i<3;i++){
		this->authority[i] = x[i];
	}
}

bool Directory::is_authority(string userName, string group, string action){
	//1x 2w 4r
	if (userName == this->owner){
		if ("r" == action){
			if (this->authority[0] >= 4)
				return true;
		}else if("w" == action){
			if ((this->authority[0]%4) >= 2)
				return true;
		}else if("x" == action){
			if ((this->authority[0]%2) >= 1)
				return true;
		}
	}
    else if (group == this->group){
		if ("r" == action){
			if (this->authority[1] >= 4)
				return true;
		}else if("w" == action){
			if ((this->authority[1]%4) >= 2)
				return true;
		}else if("x" == action){
			if ((this->authority[1]%2) >= 1)
				return true;
		}
	}
    else{
		if ("r" == action){
			if (this->authority[2] >= 4)
				return true;
		}else if("w" == action){
			if ((this->authority[2]%4) >= 2)
				return true;
		}else if("x" == action){
			if ((this->authority[2]%2) >= 1)
				return true;
		}
	}
	return false;
}
