#pragma once
#ifndef DIROPERATE_H
#define DIROPERATE_H
#include<string>
#include<iostream>
#include"FCB.h"
#include"Directory.h"
#include"DiskOperate.h"
using namespace std;
typedef class DirOperate {
	//ָ���
	//�Ƽ������︺��Ŀ¼�߼���������status��������������ݷ���status�������˴β�����ȷ���
public:
	//file
	int create_file(Directory*lastDirectory, string fileName, char type); //�����ɹ�����0��ʧ�ܷ���-1
	string cat_file(FCB*FCBptr, DiskOperate*diskOperate); //return content of file
	void write_file(FCB*FCBptr, DiskOperate*diskOperate, string content);//
	void change_directory(string newpath);
	bool rm_file(FCB*FCBptr); //remove a file, just change the recycleFlag   �β�ָ���Ƿ�Ҫ�ĳ�Ŀ¼��ָ�룿��	
					//int change_file(); we need it?or just remove&create
	void list_directory(Directory*directory);//�г���ǰĿ¼�µ��ļ����ļ���-1��
	void list_all_directory(Directory*directory);//�г���ǰĿ¼�������ļ������ļ���
	//void touch_directory();//����Ŀ¼-------�����ļ������ļ���ʱ�Զ�����Ŀ¼�� �ú���û��ʵ��
	void rm_directory(Directory*directory);
}DirOperate;
#endif // !DIROPERATE_H

