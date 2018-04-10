#include"other.h"
#include<iostream>
#include<string>
#include <vector>

using namespace std;


int directory_count_max = block_size / sizeof(Directory);//ÿ����̿���װ�µ�Ŀ¼����
int FCB_count_max = block_size / sizeof(FCB);//ÿ����̿���װ�µ�FCB��

int directory_count;//����ʹ�õĴ��̵�ǰ��Ŀ¼����
int FCB_count;//����ʹ�õĴ��̵�ǰ�ĵ�FCB��

int current_directory_block;//��ǰĿ¼���ڴ��̺�
int current_FCB_block;//��ǰFCB���ڴ��̺�

Directory*root_directory;//Ŀ¼��Դ���
FCB*root_fcb;//FCBԴ���

int* BlockMap;
char* systemStartAddr;
string currentDir;
DirOperate*dirOp;
DiskOperate*diskOP;
char* bitmap;

Directory*get_new_Directory() {
	//��Ŀ¼�洢��Ϊ�µ�Ŀ¼����ռ䣬����µ��ļ�Ŀ¼ָ��
	Directory*newDirectory;
	if (directory_count < directory_count_max) {
		//��ǰ���̿��пռ�
		//newDirectory = new Directory();
		newDirectory = root_directory + directory_count * sizeof(Directory);
		directory_count++;
	}
	else {
		//��ǰ���̿���  Ѱ���µĴ��̿�
		int newBlockNum = get_new_block();
		//�����µ�BlockMap  
		BlockMap[current_directory_block] = newBlockNum;
		BlockMap[newBlockNum] = -1;
		//������һ��������¼
		current_directory_block = newBlockNum;
		//newBlockMap->set_currentBlock(newBlockNum);
		//newDirectory = new Directory();
		newDirectory = root_directory + directory_count * sizeof(Directory);
		directory_count=1;
	}
	return newDirectory;
}

FCB*get_new_FCB() {
	//��FCB�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null
	if (FCB_count < FCB_count_max);
	else {
		//��ǰ���̿���  Ѱ���µĴ��̿�
		int newBlockNum = get_new_block();
		FCB_count = 0;
		//�����µ�BlockMap  
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
	//�ڴ����в鿴�������Ƿ�ռ䣬���һ����̺�,ʧ�ܷ���-1
	//��λͼ��bit�������ҿ���Ĵ���
	//�ҵ�������� ��1 ���������Ĵ��̺�
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
	//·��ת����Ŀ¼��ӳ��  /home/www/in.c
	Directory*tmp = root_directory;
	vector<string> v = split(path, "\/"); //�ɰ�����ַ����ָ�;--------------��չ��ɾ��------------------------
	for (vector<string>::size_type i = 0; i != v.size(); ++i) {
		bool flag = false;
		//��ÿ��v[i]���·����Ѱ�����Ӧ��Ŀ¼��  ���ı���
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
	//-----------------------�����ҵ� �����ָ���ַ-----------------------------
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//�ҵ��ַ������׸������ڷָ�������ĸ��
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

		//�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
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
	//��blockMap�Ĵ洢������������ռ䣬����ļ��µ�ָ�룬ʧ�ܷ���null 
	//������ݶ�һ����� ����չ-------------------
	//��FCB�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null
	if (blockMap_count < blockMap_count_max);
	else {
		//��ǰ���̿���  Ѱ���µĴ��̿�
		int newBlockNum = get_new_block();
		blockMap_count = 0;
		//�����µ�BlockMap  
		BlockMap*newBlockMapForItself = new BlockMap();
		//������һ��������¼ ---------------------------------ûд---------------------------
		current_blockMap_block = newBlockNum;
		newBlockMapForItself->set_currentBlock(newBlockNum);
		blockMap_count++;
	}
	BlockMap*newBlockMap = new BlockMap();
	blockMap_count++;
	return newBlockMap;
}

void changeNextBlock(int currentBlock, int initBlock){
	//���ݵ�һ��Ĵ��̿�ű����ҵ���ǰ���̿飬�����������һ��
	
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
//��Դ·�����ҵ���һ��directory��ָ��
//----------------����ֵ��ô����---bug--------------------------------
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


