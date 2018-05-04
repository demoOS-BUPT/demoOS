#include"Directory.h"
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