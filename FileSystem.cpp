//#include<bits/stdc++.h>
//#include "DiskOperate.cpp"
#include"other.h"
#include<string>
#include<iostream>
using namespace std;
void example(){
	initSystem();
	string op;
	//这里是命令行，在这里进行初始化，循环，交互
	while (1){
		cout << "[root@localhost " + currentDir + "]";
		cin >> op;
		bool endThisOperate = false;
		fflush(stdin);
		string[] args = op.split(' ');
		//这里加多空格容错
		switch (args[0]){
			case "ls":
			case "ll":
				//列出目录
				dirOp.list_directory("");
				endThisOperate = true;
				break;
			case "mkdir":
				dirName = args[1];
				//检查是否存在此目录，不存在则创建
				break;
            case "touch":
                fileName = args[1];
                //创建file
                break;
			case "cat":
				fileName = args[1];
				//查看文件信息
				break;
            case "cd":
                dirName = args[1];
                //switch dir
                break;
			default:
                cout << "未识别" << endl;
                continue;
		}
	}
}

void init_system() {
	systemStartAddr = (char*)malloc(system_size * sizeof(char));  
    cout << "磁盘大小:" << system_size << endl;
    cout << "每块大小:" << block_size << endl;
    cout << "盘块数:" << block_count << endl;
    //初始化盘块的位示图  
    memset(systemStartAddr, 0, system_size * sizeof(char));
    //前三块分别是 bit图，目录项，fcb
    bitmap = systemStartAddr;
    for(int i=0;i<init_blockMap_block_num;i++)
        bitmap[i] = 1;
	root_directory = systemStartAddr + block_size * 1;
    root_fcb = systemStartAddr + block_size * 2;
    //BlockMap* blockList = systemStartAddr + block_szie * 3;-----------------------------


    //创建目录 /home
    //创建目录 /home/www
    //创建文件 /home/www/in.c
    //创建文件 /home/out.c
}

void test_unit(){
	//这里调试
}

int main(){
	example();

	return 0;
}