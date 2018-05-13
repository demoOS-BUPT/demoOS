#pragma once
#ifndef DIROPERATE_H
#define DIROPERATE_H
#include<string>
#include<iostream>
#include"FCB.h"
#include"Directory.h"
#include"DiskOperate.h"
#include<QtCore>
using namespace std;
typedef class DirOperate {
	//ָ���
	//�Ƽ������︺��Ŀ¼�߼���������status��������������ݷ���status�������˴β�����ȷ���
public:
	//file
	int mkdir_dir(Directory*lastDirectory, string fileName); //�����ɹ�����0��ʧ�ܷ���-1
	int create_file(Directory*lastDirectory, string fileName, char type); //�����ɹ�����0��ʧ�ܷ���-1
	string cat_file(FCB*FCBptr, DiskOperate*diskOperate); //return content of file
	void write_file(FCB*FCBptr, DiskOperate*diskOperate, string content);//
	void change_directory(string inputNewPath);
	bool rm_file(Directory*fileDir,Directory*lastDir); //remove a file, just change the recycleFlag  
	void list_directory(Directory*directory);//�г���ǰĿ¼�µ��ļ����ļ���-1��
    QString list_directory_q(Directory*directory);
	void list_all_directory(Directory*directory);//�г���ǰĿ¼�������ļ������ļ���
    QString list_all_directory_q(Directory*directory);
	void ln(Directory*sfiledir, Directory* tlastdir, string fileName);
    QString ln_q(Directory*sfiledir, Directory* tlastdir, string fileName);
	void cp_file(Directory*sfiledir, Directory*tlastdir, string tfileName);
	void mv_file ( Directory*slastdir,Directory*sfiledir, Directory*tlastdir);
	void ll_directory(Directory*directory);
    QString ll_directory_q(Directory*directory);
	//void touch_directory();//����Ŀ¼-------�����ļ������ļ���ʱ�Զ�����Ŀ¼�� �ú���û��ʵ��
	void rm_directory(Directory*dir,Directory*lastdir);
}DirOperate;
#endif // !DIROPERATE_H

