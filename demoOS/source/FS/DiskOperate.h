#pragma once
#ifndef DISKOPERATE_H
#define DISKOPERATE_H
#include<string>
using namespace std;
typedef class DiskOperate {
public:
	bool write(int startBlock, string content);//从哪开始写，写啥
	string read(int startBlock, int dataSize);//从哪，多长
}DiskOperate;


#endif // !DISKOPERATE_H
