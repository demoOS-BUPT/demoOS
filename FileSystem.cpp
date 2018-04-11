#include "other.h"
#include<string>
#include<new>
using namespace std;

void example(){
	//-----------------------currentDir怎么修改-------------------------------
	init_system();
	string op,dirName,fileName;
	//这里是命令行，在这里进行初始化，循环，交互
	while (1) {
		cout << "[root@localhost " + currentDir + "]";
		getline(cin,op);
		//cin >> op;
		fflush(stdin);
		vector<string> args = split(op, " ");
		//		string args[] = op.split(" ");
				//这里加多空格容错
		if (args[0] == "ls" || args[0] == "ll") {
			//列出目录
			dirOp->list_directory(path_to_directory(currentDir));
		}
		else if (args[0] == "mkdir") {
			dirName = args[1];
	        //创建的文件是文件
            if (-1 == dirOp->create_file(path_to_directory(currentDir), dirName, '1')) {
                cout << "Failed in creating files." << endl;
            }
		}
		else if (args[0] == "touch") {
			//创建file
			fileName = args[1];
			if (-1 == dirOp->create_file(path_to_directory(currentDir), fileName, '0')) {
				cout << "Failed in creating files." << endl;
			}
		}
        else if (args[0] == "vim") {
            string fileContent;
            fileName = args[1];
            cout << "Please input file content:";
            cin >> fileContent;

            dirOp->write_file(path_to_directory(currentDir+"/"+fileName)->get_FCBptr(), diskOP, fileContent);
            //写文件
        }
		else if (args[0] == "cat") {
			fileName = args[1];
			dirOp->cat_file(path_to_directory(currentDir+"/"+fileName)->get_FCBptr(), diskOP);
			//查看文件信息
		}
		else if (args[0] == "cd") {
			dirName = args[1];
			//switch dir
		}
		else {
			cout << "unidentification!" << endl;
		}
	}
		/*

			case 'ls':

			case "ll":
				
				break;
			case "mkdir":
				dirName = args[1];
				//检查是否存在此目录，不存在则创建
				break;
            case "touch":
                fileName = args[1];
                //创建file
                break;
			case "cat":
				fileName = args[1];
				//查看文件信息
				break;
            case "cd":
                dirName = args[1];
                //switch dir
                break;
			default:
                cout << "未识别" << endl;
                continue;

				*/
		
	}


void init_system() {
	//初始化有问题！！！！！！！！！block_count太多了 一块记录块根本不够存的
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
    for(int i=0;i<init_blockMap_block_num;i++)
        bitmap[i] = 1;
	//-------------------------------------root初始化------------------------------------
	//在实际的物理地址上有指针创建对象
	//void*buf= systemStartAddr + block_size * init_directory_block_num;
	//cout <<( systemStartAddr + block_size * init_directory_block_num )<< endl;
	//cout << buf << endl;
	//root_directory = new(systemStartAddr + block_size * init_directory_block_num)Directory;
	root_directory = (Directory *)systemStartAddr + block_size * init_directory_block_num;
	//cout << root_directory << endl;
	root_fcb = (FCB*)systemStartAddr + block_size * init_FCB_block_num;
    BlockMap = new(systemStartAddr + block_size * init_blockMap_block_num)int[block_count+1];
	//root_directory = new Directory();//不知道物理地址会不会变
	//cout << root_directory << endl;
	//root_directory->set_fileName("root");
	//root_directory->set_type('0');
	init_blockMap();
	//directory FCB物理上顺序存储 逻辑上链式存储
    //创建目录 /home
    //创建目录 /home/www
    //创建文件 /home/www/in.c
    //创建文件 /home/out.c
}

void test_unit(){
	//这里调试
	//注册时间格式没有分秒
	//初始化时候不该生成对象
	//创建文件的时候文件名有误
}

int main(){
	example();

	return 0;
}
