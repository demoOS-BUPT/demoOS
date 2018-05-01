#include<string>
#include<cstring>
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
    strcpy_s(arrContent, content.c_str());
    int i = 0;
    char * startAdd = startBlock * block_size + systemStartAddr;//块号+起始地址
    for(i = 0;i<content.size();i++){
        startAdd[i] = arrContent[i];
    }
    startAdd[i] = '\0';
    return true;
}

string DiskOperate::read(int startBlock, int dataSize){
    char arrContent[block_size];
	//char *block = new(startBlock * block_size + systemStartAddr)char[block_size];
    char * startAdd = startBlock * block_size + systemStartAddr;//块号+起始地址
    int i=0;
    for(i = 0;i<dataSize;i++){
        arrContent[i] = startAdd[i];
    }
    arrContent[i] = '\0';
    string result = arrContent;
    return result;
}
