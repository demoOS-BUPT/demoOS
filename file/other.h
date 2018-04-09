#pragma once
#ifndef OTHER_H
#define OTHER_H
#include"Directory.h"
#include"FCB.h"
using namespace std;

Directory*get_new_Directory(); 
//在目录存储块为新的目录申请空间，获得新的文件目录指针

FCB*get_new_FCB();
//在FCB的存储磁盘区域申请空间，获得文件新的FCB指针，失败返回null

int get_new_data_block();
//在磁盘中查看磁盘中是否空间，获得一块磁盘号,失败返回-1

#endif // !OTHER_H
