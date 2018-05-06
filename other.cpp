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
Directory*lastDir;
Directory*curDir;
DirOperate*dirOp;
DiskOperate*diskOP;
char* bitmap;

User* currentUser;
User* userArr;

Directory*get_new_Directory() {
	//在目录存储块为新的目录申请空间，获得新的文件目录指针
	Directory*newDirectory;
	if (directory_count < directory_count_max) {
		//当前磁盘块有空间
		//newDirectory = new Directory();
		newDirectory =new( root_directory + directory_count * sizeof(Directory))Directory;
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
		newDirectory =new( root_directory + directory_count * sizeof(Directory))Directory;
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
	FCB*newFCB =new( root_fcb + FCB_count * sizeof(FCB))FCB;
	FCB_count++;
	return newFCB;
}

int get_new_block() {
	//在磁盘中查看磁盘中是否空间，获得一块磁盘号,失败返回-1
	//对位图逐bit检索查找空余的磁盘
	//找到空余磁盘 置1 返回其代表的磁盘号
	int i = init_blockMap_block_num+1;
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
	//cout << "block_count" <<block_count<< endl;
	//cout << block_size << endl;
	//cout << (sizeof(BlockMap) / sizeof(BlockMap[0])) << endl;
	for (int i = 0; i < block_count; i++) {
		BlockMap[i] = -1;
	}
}



Directory*path_to_directory(string path) {
	//路径转换成目录的映射  /home/www/in.c----->www所在文件夹的目录项指针
	//                 /home/www/zxh----->www所在文件夹
	Directory*tmp;
	string backUpPath = currentDir;
	Directory*backUpDir = curDir;
	bool flag = true;
	//Directory*lastTmp = NULL;
	if (path[0] == '/') {
		tmp = root_directory;
		//	     / home / work / www / a.c
		vector<string> v = split(path, "/"); //可按多个字符来分隔;--------------扩展名删了------------------------
		for (vector<string>::size_type i = 0; i != v.size()-1; ++i) {
			flag = false;
			//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
			for (int j = 0; j < tmp->get_fileListNum() && flag == false; j++) {
				if (v[i] == tmp->get_fileList(j)->get_fileName()) {
					//lastTmp = tmp;
					tmp = tmp->get_fileList(j);
					flag = true;
				}
			}
		}
		if (v.size() ==1) {
			flag = true;
		}
		/*
		for (vector<string>::size_type i = 0; i != v.size()-1; ++i) {
			string newPath = "";
			newPath = newPath + '/' + v[i];
			currentDir = newPath;//自动转换目录  /home/www
		}
		lastDir = lastTmp;
		curDir = tmp;
		*/
	}
	//else if (path[0] == '.'&&path[1] == '/') {
		// . / a.c
		//vector<string> v = split(path, "/");
	//}
	else {
		//	touch a.c
		//相对路径
		
		tmp = curDir;
		vector<string> v = split(path, "/");
		string newPath = currentDir;
		for (vector<string>::size_type i = 0; i != v.size()-1; ++i) {
			flag = false;
			if (v[i] == ".") {
				flag = true;
				//tmp = curDir;
			}
			else if (v[i] == "..") {
				flag = true;
				tmp = tmp->get_curDir();
				newPath = get_lastPath(newPath);
			}
			else {
				for (int j = 0; j < tmp->get_fileListNum() &&flag == false; j++) {
					//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
					if (v[i] == tmp->get_fileList(j)->get_fileName()) {
						tmp = tmp->get_fileList(j);
						flag = true;
						newPath = newPath + '/' + v[i];
					}
				}
			}
		}
		if (v.size() == 1) {
			flag = true;
		}
	}
	if (flag == false) {
		//cout << "Error! The path is invaild." << endl;
		//currentDir = backUpPath;
		//curDir = backUpDir;
		//lastDir = curDir->get_curDir();
		return NULL;
	}
	else {
		return tmp;
	}
	
}

string path_to_filename(string op) {
	// /home/www/in.c----->in.c
	//a.c->a.c
	//./a.c->a.c
	string filename;
	//if (op[0] == '/'||op[0]=='.') {
		//	     / home / work / www / a.c
		//   ./a.c
		vector<string> v = split(op, "/"); 
		filename = v[v.size() - 1];
		
//	}
	/*
	else if (op[0] == '.'&&op[1] == '/') {
		// . / a.c
		filename= filename = v[v.size() - 1];
	}*/
	//else {
		//filename = op;
	//}
	return filename;
}
vector<string> split(const string &s, const string &seperator) {
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

void update_dir(void) {
	Directory*tmp = root_directory;
	Directory*lastTmp = NULL;
	// /home/zxh
	if (currentDir == "") {
		curDir = root_directory;
		lastDir = NULL;
	}
	else {
		vector<string> v = split(currentDir, "/"); //可按多个字符来分隔;--------------扩展名删了------------------------
		for (vector<string>::size_type i = 0; i != v.size(); ++i) {
			bool flag = false;
			//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
			for (int j = 0; j < tmp->get_fileListNum() && flag == false; j++) {
				if (v[i] == tmp->get_fileList(j)->get_fileName()) {
					lastTmp = tmp;
					tmp = tmp->get_fileList(j);
					flag = true;
				}
			}
		}
		curDir = tmp;
		lastDir = lastTmp;
	}
	

}

string get_lastPath(string curPath) {
	vector<string> v = split(curPath, "/");
	string newPath = "";
	for (vector<string>::size_type i = 0; i != v.size()-1; ++i) {
		newPath = newPath + '/' + v[i];
	}
	return newPath;
}



