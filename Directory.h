#pragma once
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include<iostream>
#include<string>
#include"FCB.h"
#define DIRECTORY_MAX 20
using namespace std;

typedef class Directory {
private:
	string fileName;
	char type;  //文件类型,0文件夹,1文件  
	FCB* FCBptr; //下一层目录项,文件夹此项为null，文件指向FCB的地址
	Directory *fileList[DIRECTORY_MAX];//目录为文件夹时本目录下内容的指针
	int fileListNum;
	Directory* curDir;
	Directory* lastDir;
public:
	Directory();
	string get_fileName();
	void set_fileName(string newFileName);
	char get_type();
	void set_type(char newType);
	FCB*get_FCBptr();
	void set_FCBptr(FCB* newFCBptr);
	Directory* get_fileList(int i);
	void set_fileList(Directory*newFileList[],int i);
	void set_all_fileList(Directory*newFileList[]);
	int get_fileListNum();
	void set_fileListNum(int newFileListNum);
	bool add_fileDirectory(Directory*directory);
	Directory*get_curDir();
	void set_curDir(Directory*newCurDir);
	Directory*get_lastDir();
	void set_lastDir(Directory*newLastDir);
}Directory;
#endif // !DIRECTORY_H
