#include"DirOperate.h"
#include"other.h"
using namespace std;

int DirOperate::create_file(Directory*lastDirectory,string fileName,char type) {
	//创建成功返回0，失败返回-1
	//申请目录空间，文件类型判断文件还是文件夹
	//文件 申请FCB空间，申请文件记录块空间
	if (lastDirectory->get_fileListNum() >= DIRECTORY_MAX) {
		return -1;//当前目录下文件数已满
	}
	else {
		Directory*newDirectory = get_new_Directory();//通过函数得到一个已经创建好的对象的指针？所以不该重新创建
		//newDirectory = new Directory();
		newDirectory->set_fileName(fileName);
		newDirectory->set_type(type);
		if (!lastDirectory->add_fileDirectory(newDirectory)) {
			return -1;
		}
		if ('0' == type) {
			//文件夹的目录
			return 0;
		}
		else {
			//文件的目录
			FCB*newFCB = get_new_FCB();
			int blockNum = get_new_block();
			if (-1 == blockNum) {
				return -1;
			}
			else {
				//newFCB->set_expandName(expandFileName);
				time_t nowtime;
				nowtime = time(NULL); //获取日历时间   
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

string DirOperate::cat_file(FCB*FCBptr,DiskOperate*diskOperate) {
	//根据磁盘块号将磁盘块内容转换成字符串返回--------------------读---------------------
	int noCurrentBlock,noNextBlock;//
	string content="";
	noCurrentBlock = FCBptr->get_blockStarNum();
	noNextBlock = BlockMap[noCurrentBlock];
	//num = FCBptr->get_fileSize();
	for (int i = 0; i < FCBptr->get_fileSize()&&noCurrentBlock!=-1; i++) {
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

void DirOperate:: write_file(FCB*FCBptr, DiskOperate*diskOperate, string content) {

}

bool DirOperate::rm_file(FCB*FCBptr) {
	//形参指针是否要改成目录的指针？？
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
		//释放磁盘空间
		delete directory->get_FCBptr();
		delete directory;
	}
	else {
		for (int i = 0; i < directory->get_fileListNum(); i++) {
			rm_directory(directory->get_fileList(i));
		}
	}
}