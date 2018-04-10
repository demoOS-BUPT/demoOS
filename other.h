#pragma once
#ifndef OTHER_H
#define OTHER_H
#include"Directory.h"
#include"FCB.h"
using namespace std;
#define system_size 100*1024*1024   //ϵͳ��С����λ���ֽ�  
#define block_size 1024*1024 //�̿��С  
#define block_count system_size/block_size //ϵͳ�̿���Ŀ
#define init_directory_block_num 6
#define init_FCB_block_num 7
#define init_blockMap_block_num 8
#define directory_count_max  block_size / sizeof(Directory)//ÿ����̿���װ�µ�Ŀ¼����
#define FCB_count_max  block_size / sizeof(FCB)//ÿ����̿���װ�µ�FCB��
//#define blockMap_count_max  block_size / sizeof(BlockMap)//ÿ����̿���װ�µ��������

int directory_count;//����ʹ�õĴ��̵�ǰ��Ŀ¼����
int FCB_count ;//����ʹ�õĴ��̵�ǰ�ĵ�FCB��
//int blockMap_count;//����ʹ�õĴ��̵�ǰ�ĵ��������
//Directory FCB BlockMap ��ǰ�ĵĴ��̿� ������ȫ�ֱ������� �������ã�
int current_directory_block;//ȫ�ֱ����������»��߷������
int current_FCB_block;
//int current_blockMap_block;
Directory*root;//Ŀ¼��Դ�ڵ� ---------------------��ʼ��ʱ��ֵ-----------

Directory*get_new_Directory(); 
//��Ŀ¼�洢��Ϊ�µ�Ŀ¼����ռ䣬����µ��ļ�Ŀ¼ָ��

FCB*get_new_FCB();
//��FCB�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null

//BlockMap*get_new_blockMap();
//��blockMap�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null

int get_new_block();
//�ڴ����в鿴�������Ƿ�ռ䣬���һ����̺�,ʧ�ܷ���-1

Directory*path_to_directory(string path);
//·��ת����Ŀ¼��ӳ�� D:\\os_filesystem\\Project1

Directory*root_to_directory(Directory*directory,string path);
//��Դ·�����ҵ���һ��directory��ָ��

//void changeNextBlock(int currentBlock,int initBlock);
//���ݵ�һ��Ĵ��̿�ű����ҵ���ǰ���̿飬�����������һ��

//���������
//int BlockMap[block_count+1];

void init_blockMap();
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
*/


#endif // !OTHER_H
