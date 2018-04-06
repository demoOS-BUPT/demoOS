#include<bits/stdc++.h>
#include "DiskOperate.cpp"
#define system_size 100*1024*1024   //系统大小  
#define block_szie 1024 //盘块大小  
#define block_count system_size/block_szie //系统盘块数目

using namespace std;

char* systemStartAddr
string currentDir;
DirOperate dirOp;

//磁盘操作
typedef class DiskOperate{
public:
	bool write(int startBlock, string content);//从哪开始写，写啥
	bool read(int startBlock, int dataSize);//从哪，多长
}DiskOperate;

//指令层
//推荐在这里负责目录逻辑，仅返回status，在命令行里根据返回status，决定此次操作正确与否
typedef class DirOperate{
public:
	//file
	int create_file(); //return int startBlock
	string cat_file(); //return content of file
	bool rm_file(); //remove a file, just change the recycleFlag
	//int change_file(); we need it?or just remove&create

	void list_directory();
	void touch_directory();//创建目录
	void rm_directory();
}DirOperate;


//复制了word
typedef class FCB{
public:
    string time;          //文件建立时间或者上次修改时间
    int blockStartNum;   //文件数据起始盘块号  
    int blockEndtNum;   //文件数据起始盘块号  
    int fileSize;   //文件大小，盘块为单位  
    int dataSize;   //已写入的内容大小，字节为单位  
    int readptr;    //读指针，字节为单位  
    int link;   //文件链接数  
    int permission;   // 文件读写权限 0可读写 1只读 2只写
    bool recycleBin;
}FCB;

typedef class Directory{
public:
    char fileName[59];
    char type;  //文件类型,0文件夹,1文件  
    int startBlock; //文件是FCB起始盘块 文件夹是当前磁盘块号
    Directory* folderPtr; //下一层目录项
    Directory fileList[20];//本目录下内容？
}Directory;  



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
			case "touch":
				dirName = args[1];
				//检查是否存在此目录，不存在则创建
				break;

			case "cat":
				fileName = args[1];
				//查看文件信息
				break;
			case ""


		}

		
	}
}

void init_system(){
	systemStartAddr = (char*)malloc(system_size * sizeof(char));  
    //初始化盘块的位示图  
    for(int i=0; i<block_count; i++)  
        systemStartAddr[i] = '0';  
    //用于存放位示图的空间已被占用  
    int bitMapSize = block_count * sizeof(char) / block_szie;//位示图占用盘块数:100  
    for(int i=0; i<bitMapSize; i++)//从零开始分配  
        systemStartAddr[i] = '1';   //盘块已被使用  
}

void test_unit(){
	//这里调试
}

int main(){
	example();

	return 0;
}