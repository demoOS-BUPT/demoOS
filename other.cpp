#include"other.h"
#include<iostream>
#include<string>
#include <vector>

using namespace std;


int directory_count_max = block_size / sizeof(Directory);//每块磁盘可以装下的目录项数
int FCB_count_max = block_size / sizeof(FCB);//每块磁盘可以装下的FCB数

int directory_count;//正在使用的磁盘当前的目录项数
int FCB_count;//正在使用的磁盘当前的的FCB数

int current_directory_block;//当前目录所在磁盘号
int current_FCB_block;//当前FCB所在磁盘号

Directory*root_directory;//目录的源结点
FCB*root_fcb;//FCB源结点

int* BlockMap;
char* systemStartAddr;
string currentDir;
DirOperate*dirOp;
DiskOperate*diskOP;
char* bitmap;

Directory*get_new_Directory() {
	//在目录存储块为新的目录申请空间，获得新的文件目录指针
	Directory*newDirectory;
	if (directory_count < directory_count_max) {
		//当前磁盘块有空间
		//newDirectory = new Directory();
		newDirectory = root_directory + directory_count * sizeof(Directory);
		directory_count++;
	}
	else {
		//当前磁盘块满  寻找新的磁盘块
		int newBlockNum = get_new_block();
		//创建新的BlockMap  
		BlockMap[current_directory_block] = newBlockNum;
		BlockMap[newBlockNum] = -1;
		//更改上一块区块表记录
		current_directory_block = newBlockNum;
		//newBlockMap->set_currentBlock(newBlockNum);
		//newDirectory = new Directory();
		newDirectory = root_directory + directory_count * sizeof(Directory);
		directory_count=1;
	}
	return newDirectory;
}

FCB*get_new_FCB() {
	//在FCB的存储磁盘区域申请空间，获得文件新的FCB指针，失败返回null
	if (FCB_count < FCB_count_max);
	else {
		//当前磁盘块满  寻找新的磁盘块
		int newBlockNum = get_new_block();
		FCB_count = 0;
		//创建新的BlockMap  
		//BlockMap*newBlockMap = get_new_blockMap();
		BlockMap[current_FCB_block] = newBlockNum;
		BlockMap[newBlockNum] = -1;
		current_FCB_block = newBlockNum;
		//newBlockMap->set_currentBlock(newBlockNum);
	}
	FCB*newFCB = root_fcb + FCB_count * sizeof(FCB);
	FCB_count++;
	return newFCB;
}

int get_new_block() {
	//在磁盘中查看磁盘中是否空间，获得一块磁盘号,失败返回-1
	//对位图逐bit检索查找空余的磁盘
	//找到空余磁盘 置1 返回其代表的磁盘号
	int i = init_blockMap_block_num;
	for (;i<block_count;i++){
		if (bitmap[i] == 0){
			bitmap[i] = 1;
			return i;
		}
	}
	cout << "disk is full" << endl;
	return -1;
}

void init_blockMap() {
	for (int i = 0; i < block_count; i++) {
		BlockMap[i] = -1;
	}
}



Directory*path_to_directory(string path) {
	//路径转换成目录的映射  /home/www/in.c
	Directory*tmp = root_directory;
	vector<string> v = split(path, "\/"); //可按多个字符来分隔;--------------扩展名删了------------------------
	for (vector<string>::size_type i = 0; i != v.size(); ++i) {
		bool flag = false;
		//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
		for (int j = 0; j< tmp->get_fileListNum() && flag==false; j++) {
			if (v[i] == tmp->get_fileList(j)->get_fileName()) {
				tmp = tmp->get_fileList(j);
				flag = true;
			}
		}
	}
	return tmp;
}
vector<string> split(const string &s, const string &seperator) {
	//-----------------------网上找的 用来分隔地址-----------------------------
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}
void my_spilt(string s, char separator, string* segment) {

}


/*
BlockMap*get_new_blockMap() {
	//在blockMap的存储磁盘区域申请空间，获得文件新的指针，失败返回null 
	//区块表暂定一块磁盘 不扩展-------------------
	//在FCB的存储磁盘区域申请空间，获得文件新的FCB指针，失败返回null
	if (blockMap_count < blockMap_count_max);
	else {
		//当前磁盘块满  寻找新的磁盘块
		int newBlockNum = get_new_block();
		blockMap_count = 0;
		//创建新的BlockMap  
		BlockMap*newBlockMapForItself = new BlockMap();
		//更改上一块区块表记录 ---------------------------------没写---------------------------
		current_blockMap_block = newBlockNum;
		newBlockMapForItself->set_currentBlock(newBlockNum);
		blockMap_count++;
	}
	BlockMap*newBlockMap = new BlockMap();
	blockMap_count++;
	return newBlockMap;
}

void changeNextBlock(int currentBlock, int initBlock){
	//根据第一块的磁盘块号遍历找到当前磁盘块，在其后链接下一块
	
}
*/
/*
BlockMap::BlockMap() {
currentBlock = -1;
nextBlock = -1;
}
int BlockMap::get_currentBlock() {
return currentBlock;
}
int BlockMap::get_nextBlock() {
return nextBlock;
}
void BlockMap::set_currentBlock(int newCurrentBlock) {
currentBlock = newCurrentBlock;
}
void BlockMap::set_nextBlock(int newNextBlock) {
nextBlock = newNextBlock;
}
*/
/*
Directory*root_to_directory(Directory*directory,string path) {
//在源路径下找到第一个directory的指针
//----------------返回值怎么保存---bug--------------------------------
Directory* rightDirectory;
if (directory->get_fileName() == path) {
return directory;
}
else {
for (int i = 0; i < directory->get_fileListNum(); i++) {
root_to_directory(directory->get_fileList(i),path);
}
}

}
*/


