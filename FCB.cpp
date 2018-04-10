#include"FCB.h"
#include<ctime>
using namespace std;

FCB::FCB() {
	//expandName = "";
	time = "";
	blockStarNum = -1;
	blockEndNum = -1;
	fileSize = 0;
	dataSize = 0;
	readptr = NULL;
	link = 0;
	permission = 0;
	recyleBin = 0;
}
/*
string FCB::get_expandName() {
	return expandName;
}
*/
string FCB::get_time() {
	return time;
}

int FCB::get_blockStarNum() {
	return blockStarNum;
}

int FCB::get_blockEndNum() {
	return blockEndNum;
}

int FCB::get_fileSize() {
	return fileSize;
}

int FCB::get_dataSize() {
	return dataSize;
}

int FCB::get_readptr() {
	return readptr;
}

int FCB::get_link() {
	return link;
}

int FCB::get_permission() {
	return permission;
}
bool FCB::get_recyleBin() {
	return recyleBin;
}
/*
void FCB::set_expandName(string newExpandName) {
	this->expandName = newExpandName;
}*/

void FCB::set_time(string newTime) {
	this->time = newTime;
}

void FCB::set_blockStarNum(int newBlockStarNum) {
	this->blockStarNum = newBlockStarNum;
}

void FCB::set_blockEndNum(int newBlockEndNum) {
	this->blockEndNum = newBlockEndNum;
}

void FCB::set_fileSize(int newFileSize) {
	this->fileSize = newFileSize;
}

void FCB::set_dataSize(int newDataSize) {
	this->dataSize = newDataSize;
}

void FCB::set_readptr(int newReadptr) {
	this->readptr = newReadptr;
}

void FCB::set_link(int newLink) {
	this->link = newLink;
}

void FCB::set_permission(int newPermission) {
	this->permission = newPermission;
}

void FCB::set_recyleBin(bool newRecyleBin) {
	this->recyleBin = newRecyleBin;
}