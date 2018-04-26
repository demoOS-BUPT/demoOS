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