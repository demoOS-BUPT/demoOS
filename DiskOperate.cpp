#include<string>
#include"other.h"
using namespace std;
/*
typedef class DiskOperate{
public:
	bool write(int startBlock, string content);//从哪开始写，写啥
	string read(int startBlock, int dataSize);//从哪，多长
}DiskOperate;
*/
bool DiskOperate::write(int startBlock, string content){
    char arrContent[block_size];
    strcpy(arrContent, content.c_str());
    char * startAdd = startBlock * block_size + systemStartAddr;//块号+起始地址
    for(int i = 0;i<content.size();i++){
        startAdd[i] = arrContent[i];
    }
    return true;
}

string DiskOperate::read(int startBlock, int dataSize){
    char arrContent[block_size];
    char * startAdd = startBlock * block_size + systemStartAddr;//块号+起始地址
    int i=0;
    for(i = 0;i<dataSize;i++){
        arrContent[i] = startAdd[i];
    }
    arrContent[i] = '\0';
    string result = arrContent;
    return result;
}