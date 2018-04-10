#pragma once
#ifndef FCB_H
#define FCB_H
#include<iostream>
#include<string>
using namespace std;


typedef class FCB {
private:
	//string expandName;//�ļ���չ��
	string time;//�ļ�����ʱ������ϴ��޸�ʱ��
	int blockStarNum;  //�ļ�������ʼ�̿�� 
	int  blockEndNum;//�ļ����ݽ����̿�� 
	int fileSize;//�ļ���С���̿�Ϊ��λ  
	int dataSize;//��д������ݴ�С���ֽ�Ϊ��λ 
	int readptr;//��ָ�룬�ֽ�Ϊ��λ 
	int link;//�ļ�������  
	int permission;// �ļ���дȨ�� 0�ɶ�д 1ֻ�� 2ֻд
	bool recyleBin;//�ļ��Ƿ�������վ 0���ڻ���վ 1�ڻ���վ
public:
	FCB();
	//string get_expandName();
	string get_time();
	int get_blockStarNum();
	int get_blockEndNum();
	int get_fileSize();
	int get_dataSize();
	int get_readptr();
	int get_link();
	int get_permission();
	bool get_recyleBin();
	//void set_expandName(string newExpandName);
	void set_time(string newTime);
	void set_blockStarNum(int newBlockStarNum);
	void set_blockEndNum(int newBlockEndNum);
	void set_fileSize(int newFileSize);
	void set_dataSize(int newDataSize);
	void set_readptr(int newReadptr);
	void set_link(int newLink);
	void set_permission(int newPermission);
	void set_recyleBin(bool newRecyleBin);

}FCB;

#endif // !FCB_H
