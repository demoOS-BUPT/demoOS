#include"Directory.h"
#include"FCB.h"
#include"DirOperate.h"
#include"other.h"
#include<iostream>
#include<ctime>
using namespace std;

int DirOperate::create_file(Directory*lastDirectory,string fileName,string expandFileName,char type) {
	//�����ɹ�����0��ʧ�ܷ���-1
	//����Ŀ¼�ռ䣬�ļ������ж��ļ������ļ���
	//�ļ� ����FCB�ռ䣬�����ļ���¼��ռ�
	if (lastDirectory->get_fileListNum() >= DIRECTORY_MAX) {
		return -1;//��ǰĿ¼���ļ�������
	}
	else {
		Directory*newDirectory = get_new_Directory();
		newDirectory = new Directory();
		newDirectory->set_fileName(fileName);
		newDirectory->set_type(type);
		if (!lastDirectory->add_fileDirectory(newDirectory)) {
			return -1;
		}
		if ('0' == type) {
			//�ļ��е�Ŀ¼
			return 0;
		}
		else {
			//�ļ���Ŀ¼
			FCB*newFCB = new FCB();
			int blockNum = get_new_data_block();
			if (-1 == blockNum) {
				return -1;
			}
			else {
				newFCB->set_expandName(expandFileName);
				time_t nowtime;
				nowtime = time(NULL); //��ȡ����ʱ��   
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&nowtime));
				newFCB->set_time(tmp);
				newFCB->set_blockStarNum(blockNum);
				newFCB->set_blockEndNum(blockNum);
				newDirectory->set_FCBptr(newFCB);
			}
		}
	}
	return 0;
}

string DirOperate::cat_file(FCB*FCBptr) {
	//���ݴ��̿�Ž����̿�����ת�����ַ�������
}

bool DirOperate::rm_file(FCB*FCBptr) {
	//�β�ָ���Ƿ�Ҫ�ĳ�Ŀ¼��ָ�룿��
	FCBptr->set_recyleBin(1);
	return true;
}

void DirOperate::list_directory(Directory*directory) {
	cout << directory->get_fileName() << endl << endl;
	for (int i = 0; i < directory->get_fileListNum(); i++) {
		cout << directory->get_fileList(i)->get_fileName() << endl;
	}
}

void DirOperate::list_all_directory(Directory*directory) {
	if ('1' == directory->get_type()) {
		cout << directory->get_fileName();
	}
	else {
		for (int i = 0; i < directory->get_fileListNum(); i++) {
			list_all_directory(directory->get_fileList(i));
		}
	}
}

void DirOperate::rm_directory(Directory*directory) {
	if ('1' == directory->get_type()) {
		//�ͷŴ��̿ռ�
		delete directory->get_FCBptr();
		delete directory;
	}
	else {
		for (int i = 0; i < directory->get_fileListNum(); i++) {
			rm_directory(directory->get_fileList(i));
		}
	}
}