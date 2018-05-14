#include "other.h"
#include"instructionOp.h"
#include<string>
#include<cmath>
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
		cout << ""+currentUser->get_username() +"@geek_linux:"+currentDir+"# ";

		fflush(stdin);
		getline(cin,op, '\n');
		fflush(stdin);
		vector<string> args = split(op, " ");
		//		string args[] = op.split(" ");
				//这里加多空格容错
		if (args[0] == "ls") {
			ls_instruction(op);
		}
		else if (args[0] == "ll") {
			if (args.size() == 1){
				dirOp->ll_directory(curDir);
			}
		}
		else if (args[0] == "mkdir") {
			mkdir_instruction(op);
		}
		else if (args[0] == "touch") {
			touch_instruction(op);
		}
        else if (args[0] == "vim") {
			vim_instruction(op);
        }
		else if (args[0] == "cat") {
			cat_instruction(op);
		}
		else if (args[0] == "cd") {
			//cd /home/zxh
			//dirName = args[1];
			dirOp->change_directory(args[1]);
			//switch dir
		}
		else if (args[0] == "rm") {
			rm_instruction(op);
		}
		else if (args[0] == "ln") {
			ln_instruction(op);
		}
		else if (args[0]=="cp") {
			cp_instruction(op);
			
		}
		else if (args[0]=="mv") {
			mv_instruction(op);
			
		}
		//User
		else if (args[0] == "whoami") {
			whoami_instruction(op);
		}
		else if (args[0] == "whichgroup") {
			cout << currentUser->get_group() << endl;
		}
		else if (args[0] == "su") {
			su_instruction(op);
		}
		else if (args[0] == "chmod"){
			chmod_instruction(op);
		}
		else if (args[0] == "chown"){
			chown_instruction(op);
		}
		else if (args[0] == "chgrp"){
			chgrp_instruction(op);
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
