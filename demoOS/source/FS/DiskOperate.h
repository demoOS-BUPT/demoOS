#pragma once
#ifndef DISKOPERATE_H
#define DISKOPERATE_H
#include<string>
using namespace std;
typedef class DiskOperate {
public:
	bool write(int startBlock, string content);//���Ŀ�ʼд��дɶ
	string read(int startBlock, int dataSize);//���ģ��೤
}DiskOperate;


#endif // !DISKOPERATE_H
