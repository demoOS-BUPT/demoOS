#include"DirOperate.h"
#include"other.h"
#include<time.h>
#include<math.h>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning( disable : 4996 )

int DirOperate::mkdir_dir(Directory*lastDirectory, string fileName) {
	//创建成功返回0，失败返回-1
	//申请目录空间，文件类型判断文件还是文件夹
	//文件 申请FCB空间，申请文件记录块空间
	if (lastDirectory->get_fileListNum() >= DIRECTORY_MAX) {
		return -1;//当前目录下文件数已满
	}
	else {
		Directory*newDirectory = get_new_Directory();
		newDirectory->set_fileName(fileName);
		newDirectory->set_type(DIRECTORY_TYPE);

		newDirectory->set_curDir(curDir);
		newDirectory->set_lastDir(lastDir);

		if (!lastDirectory->add_fileDirectory(newDirectory)) {
			return -1;
		}

		
	}
	return 0;
}

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
		/*if (type == '0') {
			lastDir = newDirectory;
		}
		*/
		newDirectory->set_curDir(curDir);
	newDirectory->set_lastDir(lastDir);
		
		if (!lastDirectory->add_fileDirectory(newDirectory)) {
			return -1;
		}
		if ('0' == type) {
			//currentDir = currentDir + '/' + fileName;
			//文件夹的目录
			//newDirectory->set_curDir(curDir);
			//newDirectory->set_lastDir(lastDir);
			//return 0;
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
				time_t time_second;
				struct tm nowtime;

				time_second = time(NULL); //获取日历时间   
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&time_second));
				//cout << tmp << endl;
				newFCB->set_time(tmp);
				newFCB->set_blockStarNum(blockNum);
				newFCB->set_blockEndNum(blockNum);
				newFCB->set_link(newFCB->get_link() + 1);
				newDirectory->set_FCBptr(newFCB);
			}
		}
	}
	
	/*
	if (currentDir.compare("")!=0) {
		cout << currentDir << endl;
		cout << curDir->get_fileName() << endl << lastDir->get_fileName();
		update_dir();
		cout << endl;
		cout << curDir->get_fileName() << endl << lastDir->get_fileName();

	}
	*/
	return 0;
}

string DirOperate::cat_file(FCB*FCBptr,DiskOperate*diskOperate) {
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

	double content_size = content.size();
	int blockNum = ceil(content_size/(block_size));

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

bool DirOperate::rm_file(Directory*fileDir,Directory*lastDir) {
	int link = fileDir->get_FCBptr()->get_link();
	if (link == 1) {
		//删除磁盘
		int noCurBlock = fileDir->get_FCBptr()->get_blockStarNum();
		int noNextBlock = BlockMap[noCurBlock];
		char*blockAdd = noCurBlock * block_size + systemStartAddr;
		for (int i = 0; i < fileDir->get_FCBptr()->get_fileSize()&&noCurBlock!=-1;i++) {
			bitmap[noCurBlock] = 0;
			BlockMap[noCurBlock] = -1;
			//删除磁盘内容
			for (int i = 0; i < block_size; i++) {
				blockAdd[i] = '\0';
			}
			//cout <<"noCurBlock"<< noCurBlock << endl;
			//delete[]blockAdd;
			noCurBlock = noNextBlock;
			noNextBlock = BlockMap[noCurBlock];
			blockAdd = noCurBlock * block_size + systemStartAddr;
		}
		//删除fcb
		//cout << "FCB_add" << fileDir->get_FCBptr() << endl;
		//void * buf = reinterpret_cast ();
		//FCB*fcb_tmp = fileDir->get_FCBptr();
		//cout << "fcb_tmp" << fcb_tmp << endl;
		fileDir->get_FCBptr()->~FCB();
		fileDir->set_FCBptr(NULL);
		//char *add= get_FCBptr();
		//FCB*newFCB = new(fcb_tmp)FCB;
		//cout << newFCB << endl;
		//delete []fileDir->get_FCBptr();
		//删除dir
		bool flag = false;
		for (int i = 0; i < lastDir->get_fileListNum() && flag == false; i++) {
			if (lastDir->get_fileList(i)->get_fileName() == fileDir->get_fileName()) {
				flag = true;
				lastDir->set_fileList(NULL, i);
				lastDir->set_fileListNum(lastDir->get_fileListNum() - 1);
			}
		}
		fileDir->~Directory();
		fileDir = NULL;
		adjust_array(lastDir);
		//delete fileDir;
	}
	else {
		fileDir->get_FCBptr()->set_link(link - 1);
		bool flag = false;
		for (int i = 0; i < lastDir->get_fileListNum()&&flag==false; i++) {
			if (lastDir->get_fileList(i)->get_fileName() == fileDir->get_fileName()) {
				flag = true;
				lastDir->set_fileList(NULL, i);
				lastDir->set_fileListNum(lastDir->get_fileListNum() - 1);
			}
		}
		fileDir->~Directory();
		fileDir = NULL;
		adjust_array(lastDir);
		//delete fileDir;
	}
	return true;
}

void DirOperate::list_directory(Directory*directory) {
	//cout << directory->get_fileName() <<endl<< endl;
	for (int i = 0; i < directory->get_fileListNum(); i++) {
		cout << directory->get_fileList(i)->get_fileName() << endl;
	}
}

QString DirOperate::list_directory_q(Directory*directory) {
    //cout << directory->get_fileName() <<endl<< endl;
    QString ret;
    for (int i = 0; i < directory->get_fileListNum(); i++) {
        ret+=QString::fromStdString(directory->get_fileList(i)->get_fileName())
                +"\n";
    }
    return ret;
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

QString DirOperate::list_all_directory_q(Directory*directory) {
    QString ret;
    if ('1' == directory->get_type()) {
        //cout << directory->get_fileName()<<endl;
        return QString();
    }
    else {
        for (int i = 0; i < directory->get_fileListNum(); i++) {
            ret+=QString::fromStdString(directory->get_fileList(i)->get_fileName())
                    +"\n";
            ret+=list_all_directory_q(directory->get_fileList(i));
        }
    }
    return ret;
}

void DirOperate::ll_directory(Directory*directory) {
	if ('1' == directory->get_type()) {
		//cout << directory->get_fileName()<<endl;
		return;
	}
	else {
	//drwxr-xr-x  2 root root 4096 Feb  2 09:40 Videos
		for (int i = 0; i < directory->get_fileListNum(); i++) {
			Directory* tmpDirectory = directory->get_fileList(i);
			cout << tmpDirectory->get_authority() << "  ";
			if (tmpDirectory->get_FCBptr() == NULL) {
				cout << tmpDirectory->get_fileListNum()<<" ";
			}
			else {
				cout << tmpDirectory->get_FCBptr()->get_link() << " ";
			}
			cout << tmpDirectory->get_owner() << " ";
			cout << tmpDirectory->get_group() << " ";
			cout << 4096 << "  ";
			cout << tmpDirectory->get_change_time() << " ";
			cout << tmpDirectory->get_fileName() << endl;
		}
	}
}

QString DirOperate::ll_directory_q(Directory*directory) {
    QString ret;
    if ('1' == directory->get_type()) {
        //cout << directory->get_fileName()<<endl;
        return QString();
    }
    else {
    //drwxr-xr-x  2 root root 4096 Feb  2 09:40 Videos
        for (int i = 0; i < directory->get_fileListNum(); i++) {
            Directory* tmpDirectory = directory->get_fileList(i);
            ret+=QString::fromStdString(tmpDirectory->get_authority())
                    +"  ";
            if (tmpDirectory->get_FCBptr() == NULL) {
                ret+=QString::number(tmpDirectory->get_fileListNum())
                        +" ";
            }
            else {
                ret+=QString::number(tmpDirectory->get_FCBptr()->get_link())
                        +" ";
            }
            ret+=QString::fromStdString(tmpDirectory->get_owner())
                    +" ";
            ret+=QString::fromStdString(tmpDirectory->get_group())
                    +" ";
            ret+=QString::number(4096)
                    +"  ";
            ret+=QString::fromStdString(tmpDirectory->get_change_time())
                    +" ";
            ret+=QString::fromStdString(tmpDirectory->get_fileName())
                    +"\n";
        }
    }
    return ret;
}

void DirOperate::rm_directory(Directory*dir,Directory*lastDir) {
	if ('1' == dir->get_type()) {
		rm_file(dir, lastDir);
	}
	else if (dir->get_fileListNum() == 0) {
		bool flag = false;
		for (int i = 0; i < lastDir->get_fileListNum() && flag == false; i++) {
			if (lastDir->get_fileList(i)->get_fileName() == dir->get_fileName()) {
				flag = true;
				lastDir->set_fileList(NULL, i);
				lastDir->set_fileListNum(lastDir->get_fileListNum() - 1);
			}
		}
		dir->~Directory();
		dir = NULL;
		adjust_array(lastDir);
		//delete dir;
	}
	else {
		for (int i = 0; i < dir->get_fileListNum(); i++) {
			rm_directory(dir->get_fileList(i),dir);
		}
		bool flag = false;
		for (int i = 0; i < lastDir->get_fileListNum() && flag == false; i++) {
			if (lastDir->get_fileList(i)->get_fileName() == dir->get_fileName()) {
				flag = true;
				lastDir->set_fileList(NULL, i);
				lastDir->set_fileListNum(lastDir->get_fileListNum() - 1);
			}
		}
		dir->~Directory();
		dir = NULL;
		adjust_array(lastDir);
	}
}

void DirOperate::change_directory(string inputNewPath) {
	bool flag = true;
	string backUpPath = currentDir;
	Directory*backUpDir = curDir;
	Directory*tmp;
	if (inputNewPath[0] == '/') {
		if (inputNewPath == "/") {
			//根目录
			curDir = root_directory;
			lastDir = NULL;
			currentDir = "/";
		}
		else {
			//绝对路径
			tmp = root_directory;
			vector<string> v = split(inputNewPath, "/");
			string newPath = "";
			for (vector<string>::size_type i = 0; i != v.size(); ++i) {
				flag = false;
				//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
				for (int j = 0; j < tmp->get_fileListNum() && flag == false; j++) {
					if (v[i] == tmp->get_fileList(j)->get_fileName()) {
						tmp = tmp->get_fileList(j);
						flag = true;
						newPath = newPath + '/' + v[i];
					}
				}
				currentDir = newPath;
			}
			if (v.size() == 1) {
				//flag = true;
			}
			curDir = tmp;
			lastDir = curDir->get_curDir();//!!!!			
		}
		
	}
	else if (inputNewPath ==".") {
		//当前目录
	}
	else if (inputNewPath == "..") {
		//上级目录
		curDir = lastDir;
		lastDir = lastDir->get_curDir();
		currentDir= get_lastPath(currentDir);
		if (currentDir == "")
			currentDir = "/";
	}
	else {
		//相对路径
		//string tmpPath = currentDir;
		tmp = curDir;
		vector<string> v = split(inputNewPath, "/");
		string newPath = currentDir;
		for (vector<string>::size_type i = 0; i != v.size(); ++i) {
			flag = false;
			if (v[i] == ".") {
				flag = true;
				//tmp = curDir;
			}
			else if (v[i] == "..") {
				flag = true;
				tmp = tmp->get_curDir();
				newPath = get_lastPath(newPath);
			}
			else {
				for (int j = 0; j < tmp->get_fileListNum() && flag == false; j++) {
					//对每个v[i]存的路径名寻找其对应的目录项  树的遍历
					if (v[i] == tmp->get_fileList(j)->get_fileName()) {
						tmp = tmp->get_fileList(j);
						flag = true;
						if (newPath == "/")
							newPath = '/' + v[i];
						else
							newPath = newPath + '/' + v[i];
					}
				}
			}
			
		}
		if (v.size() == 1) {
			//flag = true;
		}
		currentDir = newPath;
		curDir = tmp;
		lastDir = curDir->get_curDir();
		/*
		for (vector<string>::size_type i = 0; i != v.size(); ++i) {
			string newPath = "";
			newPath = newPath + '/' + v[i];
			currentDir = newPath;
		}*/
		
	}
	//找到要转换的文件夹目录
	//更改当前目录以及最新记录指针
	if (curDir->get_type() == '1'||flag==false) {
		cout << "Error! Please input valid path." << endl;
		currentDir = backUpPath;
		curDir = backUpDir;
		lastDir = curDir->get_curDir();
	}
}

void DirOperate::ln(Directory*sfiledir, Directory* tlastdir, string fileName) {
	//在tlastdir下创建目录

	if (tlastdir->get_fileListNum() >= DIRECTORY_MAX) {
        cout << "Error! The current directory is full." << endl;
        return;//当前目录下文件数已满
	}
	else {
		Directory*newDirectory = get_new_Directory();
		newDirectory->set_fileName(fileName);
		newDirectory->set_type('1');

		newDirectory->set_curDir(curDir);
		newDirectory->set_lastDir(lastDir);

		if (!tlastdir->add_fileDirectory(newDirectory)) {
            return;
		}
		//建立连接
		newDirectory->set_FCBptr(sfiledir->get_FCBptr());
		sfiledir->get_FCBptr()->set_link(sfiledir->get_FCBptr()->get_link() + 1);
		sfiledir->set_change_time();
		newDirectory->set_change_time(sfiledir->get_change_time());
		newDirectory->set_owner(sfiledir->get_owner());
		newDirectory->set_group(sfiledir->get_group());
		newDirectory->set_authority(sfiledir->get_authority_value());
    }
}

QString DirOperate::ln_q(Directory*sfiledir, Directory* tlastdir, string fileName) {
    //在tlastdir下创建目录

    if (tlastdir->get_fileListNum() >= DIRECTORY_MAX) {
        return"Error! The current directory is full.";//当前目录下文件数已满
    }
    else {
        Directory*newDirectory = get_new_Directory();
        newDirectory->set_fileName(fileName);
        newDirectory->set_type('1');

        newDirectory->set_curDir(curDir);
        newDirectory->set_lastDir(lastDir);

        if (!tlastdir->add_fileDirectory(newDirectory)) {
            return "";
        }
        //建立连接
        newDirectory->set_FCBptr(sfiledir->get_FCBptr());
        sfiledir->get_FCBptr()->set_link(sfiledir->get_FCBptr()->get_link() + 1);
        sfiledir->set_change_time();
        newDirectory->set_change_time(sfiledir->get_change_time());
        newDirectory->set_owner(sfiledir->get_owner());
        newDirectory->set_group(sfiledir->get_group());
        newDirectory->set_authority(sfiledir->get_authority_value());
    }
    return "";
}

void DirOperate::cp_file(Directory*sfiledir, Directory*tlastdir, string tfileName) {
	//文件是否存在，存在则覆盖，不存在则复制
	bool flag = false;
	Directory*tfiledir=NULL;
	string content;
	int i = 0;
	for ( i = 0; i < tlastdir->get_fileListNum() && flag == false; i++) {
		if (tlastdir->get_fileList(i)->get_fileName() == tfileName) {
			flag = true;
		}
	}
	if (flag == true) {
		//文件已经存在
		tfiledir = tlastdir->get_fileList(i-1);
	}
	else {
		//文件不存在
		dirOp->create_file(tlastdir, tfileName, '1');
		for (int i = 0; i < tlastdir->get_fileListNum() && flag == false; i++) {
			if (tlastdir->get_fileList(i)->get_fileName() == tfileName) {
				flag = true;
				tfiledir = tlastdir->get_fileList(i);
			}
		}
	}
	//cout << tfiledir << endl;
	content = dirOp->cat_file(sfiledir->get_FCBptr(), diskOP);
	//cout << tfiledir << endl;
	dirOp->write_file(tfiledir->get_FCBptr(), diskOP, content);
}

void DirOperate::mv_file(Directory*slastdir, Directory*sfiledir, Directory*tlastdir) {
		bool flag = false;
		for (int i = 0; i < slastdir->get_fileListNum() && flag == false; i++) {
			if (slastdir->get_fileList(i)->get_fileName() ==sfiledir->get_fileName()) {
				flag = true;
				slastdir->set_fileList(NULL, i);
				slastdir->set_fileListNum(slastdir->get_fileListNum() - 1);
			}
		}
		adjust_array(slastdir);
		tlastdir->add_fileDirectory(sfiledir);
}
