#include"Directory.h"
#include <ctime>
using namespace std;

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
	authority[1] = 7;
	authority[2] = 7;
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
void Directory::set_fileList(Directory*newFileList[], int i) {
	if (i < DIRECTORY_MAX&&i >= 0) {
		fileList[i] = newFileList[i];
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
	for (int i=0;i<3;i++){
		char s[12];             //设定12位对于存储32位int值足够  
    	itoa(this->authority[i],s,10);            //itoa函数亦可以实现，但是属于C中函数，在C++中推荐用流的方法  
    	string str_tmp = s;
		autho += str_tmp;
	}
	return autho;
}

string Directory::get_change_time(){
	return this->changeTime;
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
	if (group == this->group){
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
	if (group == this->group){
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