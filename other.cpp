#include"other.h"
#include<iostream>
#include<string>
#include <vector>

using namespace std;
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
Directory*get_new_Directory() {
	//在目录存储块为新的目录申请空间，获得新的文件目录指针
	Directory*newDirectory;
	if (directory_count < directory_count_max) {
		//当前磁盘块有空间
		newDirectory = new Directory();//要用new还是malloc---------------------------------
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
		newDirectory = new Directory();//要用new还是malloc---------------------------------
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
	FCB*newFCB = new FCB();
	FCB_count++;
	return newFCB;
}

int get_new_block() {
	//在磁盘中查看磁盘中是否空间，获得一块磁盘号,失败返回-1
	//对位图逐bit检索查找空余的磁盘
	//找到空余磁盘 置1 返回其代表的磁盘号
	
}

void init_blockMap() {
	for (int i = 0; i < block_count; i++) {
		BlockMap[i] = -1;
	}
}
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

Directory*path_to_directory(string path) {
	//路径转换成目录的映射 D:\\os_filesystem\\Project1
	//string s = "a,b*c*d,e";
	vector<string> v = split(path, "\\"); //可按多个字符来分隔;
	for (vector<string>::size_type i = 0; i != v.size(); ++i) {
		//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
	}


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

