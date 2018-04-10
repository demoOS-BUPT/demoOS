#pragma once
#ifndef OTHER_H
#define OTHER_H
#include"Directory.h"
#include"FCB.h"
#include"DirOperate.h"
#include"DiskOperate.h"
#include<iostream>
#include<ctime>
#include<string>
#include <vector>
using namespace std;

#define system_size 100*1024*1024   //系统大小，单位是字节  
#define block_size 1024*1024 //盘块大小  
#define block_count system_size/block_size //系统盘块数目
#define init_directory_block_num 6//---------------------------------------转换-----------------
#define init_FCB_block_num 7
#define init_blockMap_block_num 8

int directory_count_max = block_size / sizeof(Directory);//每块磁盘可以装下的目录项数
int FCB_count_max = block_size / sizeof(FCB);//每块磁盘可以装下的FCB数
											 
int directory_count;//正在使用的磁盘当前的目录项数
int FCB_count ;//正在使用的磁盘当前的的FCB数

int current_directory_block;//当前目录所在磁盘号
int current_FCB_block;//当前FCB所在磁盘号

Directory*root_directory;//目录的源结点
FCB*root_fcb;//FCB源结点

int* BlockMap;
char* systemStartAddr;
string currentDir;
DirOperate dirOp;
char* bitmap;

Directory*get_new_Directory(); 
//在目录存储块为新的目录申请空间，获得新的文件目录指针

FCB*get_new_FCB();
//在FCB的存储磁盘区域申请空间，获得文件新的FCB指针，失败返回null

int get_new_block();
//在磁盘中查看磁盘中是否空间，获得一块磁盘号,失败返回-1

Directory*path_to_directory(string path);
//路径转换成目录的映射 /home/www/zxh.txt

void init_system();
//系统初始化

void init_blockMap();
//初始化区块表

void my_spilt(string s, char separator,string* segment);
//用来分隔字符串

vector<string> split(const string &s, const string &seperator);
/*
typedef class BlockMap {
private:
	int currentBlock;
	int nextBlock;//为-1，即为结束
	
public:
	BlockMap();
	int get_currentBlock();
	int get_nextBlock();
	void set_currentBlock(int newCurrentBlock);
	void set_nextBlock(int newNextBlock);
}BlockMap;


//Directory*root_to_directory(Directory*directory,string path);
//在源路径下找到第一个directory的指针

//void changeNextBlock(int currentBlock,int initBlock);
//根据第一块的磁盘块号遍历找到当前磁盘块，在其后链接下一块

//BlockMap*get_new_blockMap();
//在blockMap的存储磁盘区域申请空间，获得文件新的FCB指针，失败返回null

//#define blockMap_count_max  block_size / sizeof(BlockMap)//每块磁盘可以装下的区块表数
//int blockMap_count;//正在使用的磁盘当前的的区块表数
//Directory FCB BlockMap 当前的的磁盘块 暂且用全局变量定义 从哪里获得？
//int current_blockMap_block;

*/
#endif // !OTHER_H
