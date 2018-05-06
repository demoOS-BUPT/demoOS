#pragma once
#ifndef OTHER_H
#define OTHER_H
#include"Directory.h"
#include"FCB.h"
#include"DirOperate.h"
#include"DiskOperate.h"
#include"User.h"
#include<iostream>
#include<ctime>
#include<string>
#include <vector>
using namespace std;

#define system_size 100*1024*1024   //ϵͳ��С����λ���ֽ�  
#define block_size 1024*1024 //�̿��С  
#define block_count 100 //ϵͳ�̿���Ŀ
#define init_directory_block_num 6//---------------------------------------ת��Ҫ����ó�----------------------------
#define init_FCB_block_num 7
#define init_blockMap_block_num 8
#define DIRECTORY_TYPE 0
#define FILE_TYPE 1

extern int directory_count_max;//ÿ����̿���װ�µ�Ŀ¼����
extern int FCB_count_max ;//ÿ����̿���װ�µ�FCB��
											 
extern int directory_count;//����ʹ�õĴ��̵�ǰ��Ŀ¼����
extern int FCB_count ;//����ʹ�õĴ��̵�ǰ�ĵ�FCB��

extern int current_directory_block;//��ǰĿ¼���ڴ��̺�
extern int current_FCB_block;//��ǰFCB���ڴ��̺�

extern Directory*root_directory;//Ŀ¼��Դ���
extern FCB*root_fcb;//FCBԴ���

extern int* BlockMap;//�����
extern char* systemStartAddr;
extern string currentDir;
extern Directory*lastDir;
extern Directory*curDir;
extern DirOperate*dirOp;
extern DiskOperate*diskOP;
extern char* bitmap;

extern User* currentUser;
extern User* userArr;

Directory*get_new_Directory(); 
//��Ŀ¼�洢��Ϊ�µ�Ŀ¼����ռ䣬����µ��ļ�Ŀ¼ָ��

FCB*get_new_FCB();
//��FCB�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null

int get_new_block();
//�ڴ����в鿴�������Ƿ�ռ䣬���һ����̺�,ʧ�ܷ���-1

Directory*path_to_directory(string path);
//·��ת����Ŀ¼��ӳ�� /home/www/zxh.txt ���������Ҫ�ж��ļ����;ʹ���ȷ�ļ����Ͳ���Ҫ�жϴ�-1

string path_to_filename(string op);

void update_dir(void);
//ͨ��currentDir��ǰ·���� ����curDir lastDirָ��

string get_lastPath(string curPath);

void init_system();
//ϵͳ��ʼ��

void init_blockMap();
//��ʼ�������

void my_spilt(string s, char separator,string* segment);
//�����ָ��ַ���

vector<string> split(const string &s, const string &seperator);
/*
typedef class BlockMap {
private:
	int currentBlock;
	int nextBlock;//Ϊ-1����Ϊ����
	
public:
	BlockMap();
	int get_currentBlock();
	int get_nextBlock();
	void set_currentBlock(int newCurrentBlock);
	void set_nextBlock(int newNextBlock);
}BlockMap;


//Directory*root_to_directory(Directory*directory,string path);
//��Դ·�����ҵ���һ��directory��ָ��

//void changeNextBlock(int currentBlock,int initBlock);
//���ݵ�һ��Ĵ��̿�ű����ҵ���ǰ���̿飬�����������һ��

//BlockMap*get_new_blockMap();
//��blockMap�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null

//#define blockMap_count_max  block_size / sizeof(BlockMap)//ÿ����̿���װ�µ��������
//int blockMap_count;//����ʹ�õĴ��̵�ǰ�ĵ��������
//Directory FCB BlockMap ��ǰ�ĵĴ��̿� ������ȫ�ֱ������� �������ã�
//int current_blockMap_block;

*/
#endif // !OTHER_H