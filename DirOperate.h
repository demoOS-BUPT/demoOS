#pragma once
#ifndef DIROPERATE_H
#define DIROPERATE_H
#include<string>
#include<iostream>
#include"FCB.h"
#include"Directory.h"
#include"DiskOperate.h"
using namespace std;
typedef class DirOperate {
	//指令层
	//推荐在这里负责目录逻辑，仅返回status，在命令行里根据返回status，决定此次操作正确与否
public:
	//file
	int mkdir_dir(Directory*lastDirectory, string fileName); //创建成功返回0，失败返回-1
	int create_file(Directory*lastDirectory, string fileName, char type); //创建成功返回0，失败返回-1
	string cat_file(FCB*FCBptr, DiskOperate*diskOperate); //return content of file
	void write_file(FCB*FCBptr, DiskOperate*diskOperate, string content);//
	void change_directory(string inputNewPath);
	bool rm_file(Directory*fileDir,Directory*lastDir); //remove a file, just change the recycleFlag   形参指针是否要改成目录的指针？？	
					//int change_file(); we need it?or just remove&create
	void list_directory(Directory*directory);//列出当前目录下的文件或文件夹-1层
	void list_all_directory(Directory*directory);//列出当前目录下所有文件或者文件夹
	void ll_directory(Directory*directory);
	//void touch_directory();//创建目录-------创建文件或者文件夹时自动创建目录了 该函数没有实现
	void rm_directory(Directory*dir,Directory*lastdir);
}DirOperate;
#endif // !DIROPERATE_H

