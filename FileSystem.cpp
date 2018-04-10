#include<bits/stdc++.h>
#include "DiskOperate.cpp"
#define system_size 100*1024*1024   //系统大小，单位是字节  
#define block_szie 1024*1024 //盘块大小  
#define block_count system_size/block_szie //系统盘块数目


using namespace std;

char* systemStartAddr
string currentDir;
DirOperate dirOp;
char* bitmap;
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

void init_system(){
	systemStartAddr = (char*)malloc(system_size * sizeof(char));  
    cout << "磁盘大小:" << system_size << endl;
    cout << "每块大小:" << block_szie << endl;
    cout << "盘块数:" << block_count << endl;
    //初始化盘块的位示图  
    memset(systemStartAddr, 0, system_size * sizeof(char));
    //前三块分别是 bit图，目录项，fcb
    bitmap = systemStartAddr;
    for(int i=0;i<init_blockMap_block_num;i++)
        bitmap[i] = 1;
    Directory* dirList = systemStartAddr + block_szie * 1;
    FCB* fcbList = systemStartAddr + block_szie * 2;
    BlockMap* blockList = systemStartAddr + block_szie * 3;


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