#include"DirOperate.h"
#include"other.h"
#include<time.h>
#include<math.h>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning( disable : 4996 )

int DirOperate::create_file(Directory*lastDirectory,string fileName,char type) {
	//�����ɹ�����0��ʧ�ܷ���-1
	//����Ŀ¼�ռ䣬�ļ������ж��ļ������ļ���
	//�ļ� ����FCB�ռ䣬�����ļ���¼��ռ�
	if (lastDirectory->get_fileListNum() >= DIRECTORY_MAX) {
		return -1;//��ǰĿ¼���ļ�������
	}
	else {
		Directory*newDirectory = get_new_Directory();//ͨ�������õ�һ���Ѿ������õĶ����ָ�룿���Բ������´���
		//newDirectory = new Directory();
		newDirectory->set_fileName(fileName);
		newDirectory->set_type(type);
		if (type == '0') {
			lastDir = newDirectory;
		}
		
		if (!lastDirectory->add_fileDirectory(newDirectory)) {
			return -1;
		}
		if ('0' == type) {
			currentDir = currentDir + '/' + fileName;
			//�ļ��е�Ŀ¼
			return 0;
		}
		else {
			//�ļ���Ŀ¼
			FCB*newFCB = get_new_FCB();
			int blockNum = get_new_block();
			if (-1 == blockNum) {
				return -1;
			}
			else {
				//newFCB->set_expandName(expandFileName);
				time_t time_second;
				struct tm nowtime;

				time_second = time(NULL); //��ȡ����ʱ��   
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&time_second));
				newFCB->set_time(tmp);
				newFCB->set_blockStarNum(blockNum);
				newFCB->set_blockEndNum(blockNum);
				newDirectory->set_FCBptr(newFCB);
			}
		}
	}
	return 0;
}

string DirOperate::cat_file(FCB*FCBptr,DiskOperate*diskOperate) {
	//���ݴ��̿�Ž����̿�����ת�����ַ�������--------------------��---------------------
	int noCurrentBlock,noNextBlock;//
	string content = "";
	noCurrentBlock = FCBptr->get_blockStarNum();
	noNextBlock = BlockMap[noCurrentBlock];
	//num = FCBptr->get_fileSize();
	for (int i = 0; i < FCBptr->get_fileSize() && noCurrentBlock!=-1; i++) {
		if ((i + 1) == FCBptr->get_fileSize()) {
			int lastBlockData = FCBptr->get_dataSize() - block_size*i;
			content = content + diskOperate->read(noCurrentBlock, lastBlockData);
		}
		else {
			content = content + diskOperate->read(noCurrentBlock, block_size);
		}
		noCurrentBlock = noNextBlock;
		noNextBlock = BlockMap[noNextBlock];
	}
	return content;
}

//write file
void DirOperate::write_file(FCB*FCBptr, DiskOperate*diskOperate, string content) {
	int blockNum = ceil((double)content.size()/100);

	int i = 0;
	int currBlock;
	currBlock = FCBptr->get_blockStarNum();

	for (;i<blockNum-1;i++){
		string sub = content.substr(i*block_size ,block_size);
		diskOperate->write(currBlock, sub);
		BlockMap[currBlock] = get_new_block();
		currBlock = BlockMap[currBlock];
	}
	string sub = content.substr(i*block_size );
	diskOperate->write(currBlock, sub);
	BlockMap[currBlock] = -1;
	FCBptr->set_fileSize(blockNum);
	FCBptr->set_dataSize(content.size());
	FCBptr->set_blockEndNum(currBlock);
}

bool DirOperate::rm_file(FCB*FCBptr) {
	//�β�ָ���Ƿ�Ҫ�ĳ�Ŀ¼��ָ�룿��
	FCBptr->set_recyleBin(1);
	return true;
}

void DirOperate::list_directory(Directory*directory) {
	cout << directory->get_fileName() <<endl<< endl;
	for (int i = 0; i < directory->get_fileListNum(); i++) {
		cout << directory->get_fileList(i)->get_fileName() << endl;
	}
}

void DirOperate::list_all_directory(Directory*directory) {
	if ('1' == directory->get_type()) {
		//cout << directory->get_fileName()<<endl;
		return;
	}
	else {
		for (int i = 0; i < directory->get_fileListNum(); i++) {
			cout << directory->get_fileList(i)->get_fileName() << endl;
			list_all_directory(directory->get_fileList(i));
		}
	}
}

void DirOperate::rm_directory(Directory*directory) {
	if ('1' == directory->get_type()) {
		//�ͷŴ��̿ռ�
		int noCurBlock, noNextBlock;
		noCurBlock = directory->get_FCBptr()->get_blockStarNum();
		noNextBlock = BlockMap[noCurBlock];
		for (int i = 0; i < directory->get_FCBptr()->get_fileSize() && noCurBlock!=-1; i++) {
			BlockMap[noCurBlock] = -1;
			noCurBlock = noNextBlock;
			noNextBlock = BlockMap[noNextBlock];
		}

		//----------------------------��delete�ͷſռ����� �о������а�------------------------------
		delete directory->get_FCBptr();
		delete directory;
	}
	else {
		for (int i = 0; i < directory->get_fileListNum(); i++) {
			rm_directory(directory->get_fileList(i));
		}
	}
}

void DirOperate::change_directory(string newPath) {
	//�ж��ǲ����ļ���  cd��������г���ȫ����ַ ��������ô֪�������ĸ��ļ���
	Directory*tmp = root_directory;
	if (newPath == "/") {
		lastDir = root_directory;
		currentDir = "";
	}
	else {
		vector<string> v = split(newPath, "/");
		string newPath = "";
		for (vector<string>::size_type i = 0; i != v.size(); ++i) {
			bool flag = false;
			//��ÿ��v[i]���·����Ѱ�����Ӧ��Ŀ¼��  ���ı���
			for (int j = 0; j < tmp->get_fileListNum() && flag == false; j++) {
				if (v[i] == tmp->get_fileList(j)->get_fileName()) {
					tmp = tmp->get_fileList(j);
					flag = true;
					newPath = newPath + '/' + v[i];
				}
			}
			currentDir = newPath;
		}
		/*
		for (vector<string>::size_type i = 0; i != v.size(); ++i) {
			string newPath = "";
			newPath = newPath + '/' + v[i];
			currentDir = newPath;
		}*/
		lastDir = tmp;
	}
	//�ҵ�Ҫת�����ļ���Ŀ¼
	//���ĵ�ǰĿ¼�Լ����¼�¼ָ��
}