#include "other.h"
#include<string>
#include<new>
#include<cstdlib>
#include<cstring>
using namespace std;

void example(){
	string op,dirName,fileName, fileContent;

	bool isLogin = false;
	while (! isLogin){
		isLogin = currentUser->is_login();
	}

	//这里是命令行，在这里进行初始化，循环，交互
	while (1) {
		op = "";
		cout << "["+currentUser->get_username() +"@localhost " + currentDir + "]";

		fflush(stdin);
		getline(cin,op, '\n');
		fflush(stdin);
		vector<string> args = split(op, " ");
		//		string args[] = op.split(" ");
				//这里加多空格容错
		if (args[0] == "ls" || args[0] == "ll") {
			//列出目录
			//  ll  || ls ||  ll /zxh/a
			if (args.size() == 1) {
				dirOp->list_directory(curDir);
			}
			else if(args[1]=="*"|| args[1] == "-a"){
				dirOp->list_all_directory(curDir);
			}
			else {
				fileName = path_to_filename(args[1]);
				Directory*fileDir = path_to_directory(args[1]);
				for (int i = 0; i < fileDir->get_fileListNum(); i++) {
					if (fileName == fileDir->get_fileList(i)->get_fileName()) {
						fileDir = fileDir->get_fileList(i);
						break;
					}
				}
				dirOp->list_directory(fileDir);
			}

			/*
			if (args.size() == 1) {
				dirOp->list_all_directory(root_directory);
			}
			else {
				dirOp->change_directory(args[1]);
				dirOp->list_all_directory(lastDir);
			}
			*/
		}
		else if (args[0] == "mkdir") {
			dirName = path_to_filename(args[1]);
	        //创建的文件是文件
            if (-1 == dirOp->create_file(path_to_directory(args[1]),dirName,'0')) {
                cout << "Failed in creating files." << endl;
            }
		}
		else if (args[0] == "touch") {
			//创建file
			fileName = path_to_filename(args[1]);
			if (-1 == dirOp->create_file(path_to_directory(args[1]), fileName, '1')) {
				cout << "Failed in creating files." << endl;
			}
		}
        else if (args[0] == "vim") {
            
            //fileName = args[1];
			fileName = path_to_filename(args[1]);
           
			Directory*fileDir;
			//fileDir = curDir;
			fileDir = path_to_directory(args[1]);
			if (fileDir == NULL) {
				cout << "Error! Can't find the file." << endl;
			}
			else {
				for (int i = 0; i < fileDir->get_fileListNum(); i++) {
					if (fileName == fileDir->get_fileList(i)->get_fileName()) {
						fileDir = fileDir->get_fileList(i);
						break;
					}
				}
				if (fileDir->get_type() == '1') {
					cout << "Please input file content:";
					//cin >> fileContent;
					getline(cin, fileContent, '\n');
					dirOp->write_file(fileDir->get_FCBptr(), diskOP, fileContent);
					//cin.clear();
					//cin.sync();
					//写文件
				}
				else {
					cout << "Error! Can't find the file." << endl;
				}
				
			}
			
        }
		else if (args[0] == "cat") {
			fileName = path_to_filename(args[1]);
			Directory*fileDir = path_to_directory(args[1]);
			if (fileDir == NULL) {
				cout << "Error! Can't find the file." << endl;
			}
			else {
				//fileName = args[1];
				//Directory*fileDir = curDir;
				for (int i = 0; i < fileDir->get_fileListNum(); i++) {
					if (fileName == fileDir->get_fileList(i)->get_fileName()) {
						fileDir = fileDir->get_fileList(i);
						break;
					}
				}

				if (fileDir->get_type() == '1') {
					cout << dirOp->cat_file(fileDir->get_FCBptr(), diskOP) << endl;
					//查看文件信息
				}
				else {
					cout << "Error! Can't find the file." << endl;
				}
				
			}
			
		}
		else if (args[0] == "cd") {
			//cd /home/zxh
			//dirName = args[1];
			dirOp->change_directory(args[1]);
			//switch dir
		}
		else if (args[0] == "rm") {
			fileName = path_to_filename(args[1]);
			Directory*fileDir=NULL , *lastDir= path_to_directory(args[1]);
			if (lastDir == NULL) {
				cout << "Error! Can't find the file." << endl;
			}
			else {
				for (int i = 0; i < lastDir->get_fileListNum(); i++) {
					if (fileName == lastDir->get_fileList(i)->get_fileName()) {
						fileDir = lastDir->get_fileList(i);
						break;
					}
				}
				dirOp->rm_directory(fileDir, lastDir);
			}			
		}
		//User
		else if (args[0] == "whoami") {
			cout << "whoami" << endl;
		}
		else {
			cout << "unidentification!" << endl;
		}
	}
		
}


void init_system() {
	string username = "root";
	string password = "toor";
	currentUser = new User();
	userArr = new User[20];
	currentUser->init(username, password, username);
	userArr[0].init(username, password, username);

	dirOp = new DirOperate();
	diskOP=new DiskOperate();
	systemStartAddr = (char*)malloc(system_size * sizeof(char));  
    cout << "system size:" << system_size << endl;
    cout << "block size:" << block_size << endl;
    cout << "block count:" << block_count << endl;
    //初始化盘块的位示图  
    memset(systemStartAddr, 0, system_size * sizeof(char));
    //前三块分别是 bit图，目录项，fcb
    bitmap = systemStartAddr;
    for(int i=0;i<init_blockMap_block_num+5;i++)
        bitmap[i] = 1;
	//void*buf= systemStartAddr + block_size * init_directory_block_num;
	//cout <<( systemStartAddr + block_size * init_directory_block_num )<< endl;
	//cout << buf << endl;
	//root_directory = new(systemStartAddr + block_size * init_directory_block_num)Directory;
	directory_count = 0;
	FCB_count = 0;
	//root_directory = (Directory *)systemStartAddr + block_size * init_directory_block_num;
	root_directory = new(systemStartAddr + block_size * init_directory_block_num)Directory;
	directory_count++;
	root_directory->set_fileName("/");
	root_directory->set_type('0');
	curDir = root_directory;
	lastDir = NULL;
	currentDir = "/";
	//cout << root_directory << endl;
	//root_fcb = (FCB*)systemStartAddr + block_size * init_FCB_block_num;
	root_fcb = new(systemStartAddr + block_size * init_FCB_block_num)FCB;
	FCB_count++;
    BlockMap = new(systemStartAddr + block_size * init_blockMap_block_num)int[block_count+1];
	bitmap[init_blockMap_block_num] = 1;
	//root_directory = new Directory();//不知道物理地址会不会变
	//cout << root_directory << endl;
	init_blockMap();
	//directory FCB物理上顺序存储 逻辑上链式存储
	//

	dirOp->create_file(path_to_directory("/tmp"),path_to_filename("/tmp"),'0');
	dirOp->create_file(path_to_directory("/root"),path_to_filename("/root"),'0');
	dirOp->create_file(path_to_directory("/home"),path_to_filename("/home"),'0');
	dirOp->create_file(path_to_directory("/home/a.c"), path_to_filename("/home/a.c"), '1');

}

void test_unit(){
	//这里调试
	//-----------------------未完成------------------
	//注册时间格式没有分秒
	//创建的时候当前目录重名问题
	//读写的时候没有判断是否已经创建
}

int main(){
	init_system();
	example();

	return 0;
}
