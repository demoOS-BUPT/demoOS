#pragma once
#ifndef FCB_H
#define FCB_H
#include<iostream>
#include<string>
using namespace std;


typedef class FCB {
private:
	string expandName;//文件扩展名
	string time;//文件建立时间或者上次修改时间
	int blockStarNum;  //文件数据起始盘块号 
	int  blockEndNum;//文件数据结束盘块号 
	int fileSize;//文件大小，盘块为单位  
	int dataSize;//已写入的内容大小，字节为单位 
	int readptr;//读指针，字节为单位 
	int link;//文件链接数  
	int permission;// 文件读写权限 0可读写 1只读 2只写
	bool recyleBin;//文件是否放入回收站 0不在回收站 1在回收站
public:
	FCB();
	string get_expandName();
	string get_time();
	int get_blockStarNum();
	int get_blockEndNum();
	int get_fileSize();
	int get_dataSize();
	int get_readptr();
	int get_link();
	int get_permission();
	bool get_recyleBin();
	void set_expandName(string newExpandName);
	void set_time(string newTime);
	void set_blockStarNum(int newBlockStarNum);
	void set_blockEndNum(int newBlockEndNum);
	void set_fileSize(int newFileSize);
	void set_dataSize(int newDataSize);
	void set_readptr(int newReadptr);
	void set_link(int newLink);
	void set_permission(int newPermission);
	void set_recyleBin(bool newRecyleBin);

}FCB;

#endif // !FCB_H
