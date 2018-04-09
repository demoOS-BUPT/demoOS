#pragma once
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include<iostream>
#include<string>
#include"FCB.h"
#define DIRECTORY_MAX 20
using namespace std;

typedef class Directory {
private:
	string fileName;
	char type;  //�ļ�����,0�ļ���,1�ļ�  
	//int startBlock; //�ļ���FCB��ʼ�̿� �ļ����ǵ�ǰ���̿��????
	FCB* FCBptr; //��һ��Ŀ¼��,�ļ��д���Ϊnull���ļ�ָ��FCB�ĵ�ַ
	Directory *fileList[DIRECTORY_MAX];//Ŀ¼Ϊ�ļ���ʱ��Ŀ¼�����ݵ�ָ��
	int fileListNum;
public:
	Directory();
	string get_fileName();
	void set_fileName(string newFileName);
	char get_type();
	void set_type(char newType);
	//int get_startBlock();
	//void set_startBlock(int newStartBlock);
	FCB*get_FCBptr();
	void set_FCBptr(FCB* newFCBptr);
	Directory* get_fileList(int i);
	void set_fileList(Directory*newFileList[],int i);
	void set_all_fileList(Directory*newFileList[]);
	//int get_fileList_num();
	int get_fileListNum();
	bool add_fileDirectory(Directory*directory);
	/*
	//Directory(string fileName,char type,int startBlock,Directory*folderPtr,Di)
	bool create_file(string fileName, char type,int dataSize);
	bool delete_file(string fileName);
	bool read_file(string fileName);
	bool write_file(string fileName);
	*/
}Directory;
#endif // !DIRECTORY_H
