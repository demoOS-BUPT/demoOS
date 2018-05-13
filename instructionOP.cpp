#include"instructionOp.h"
using namespace std;

void mkdir_instruction(string op) {
	vector<string> args = split(op, " ");
	string dirName = path_to_filename(args[1]);
	//创建的文件是文件
	Directory*lastDir = path_to_directory(args[1]);
	if (lastDir == NULL) {
		cout << "Error! Can't find the path." << endl;
		return;
	}
	if (-1 == dirOp->create_file(lastDir, dirName, '0')) {
		cout << "Failed in creating files." << endl;
	}
}

void touch_instruction(string op) {	
	//创建file
	vector<string> args = split(op, " ");
	Directory*lastDir = path_to_directory(args[1]);
	if (lastDir == NULL) {
		cout << "Error! Can't find the path." << endl;
		return;
	}
	string fileName = path_to_filename(args[1]);
	if (-1 == dirOp->create_file(lastDir, fileName, '1')) {
		cout << "Failed in creating files." << endl;
	}
}

void cat_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileContent;
	string fileName = path_to_filename(args[1]);
	Directory*fileDir = path_to_directory(args[1]);
	if (fileDir == NULL) {
		cout << "Error! Can't find the file." << endl;
		return;
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
			if (fileDir->is_authority(currentUser->get_username(), currentUser->get_group(), "r"))
				cout << dirOp->cat_file(fileDir->get_FCBptr(), diskOP) << endl;
			else {
				//查看文件信息
				cout << "Permission denied." << endl;
				return;
			}				
		}
		else {
			cout << "Error! Can't find the file." << endl;
			return;
		}

	}
}

void vim_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileContent;
	//fileName = args[1];
	string fileName = path_to_filename(args[1]);
	Directory*fileDir;
	//fileDir = curDir;
	fileDir = path_to_directory(args[1]);
	if (fileDir == NULL) {
		cout << "Error! Can't find the file." << endl;
		return;
	}
	else {
		for (int i = 0; i < fileDir->get_fileListNum(); i++) {
			if (fileName == fileDir->get_fileList(i)->get_fileName()) {
				fileDir = fileDir->get_fileList(i);
				break;
			}
		}
		if (fileDir->get_type() == '1') {
			if (!fileDir->is_authority(currentUser->get_username(), currentUser->get_group(), "w"))
				cout << "Permission denied." << endl;
			else {
				cout << "Please input file content:";
				//cin >> fileContent;
				getline(cin, fileContent, '\n');
				dirOp->write_file(fileDir->get_FCBptr(), diskOP, fileContent);
				//cin.clear();
				//cin.sync();
				//写文件
			}
		}
		else {
			cout << "Error! Can't find the file." << endl;
		}

	}

}

void ls_instruction(string op) {
	vector<string> args = split(op, " ");

	//列出目录
	//  ll  || ls ||  ll /zxh/a
	if (args.size() == 1) {
		//ls
		dirOp->list_directory(curDir);
	}
	else if (args[1] == "/"){
		//ls /
		dirOp->list_directory(root_directory);
	}
	else if (args[1] == "*" ) {
		// ls *
		//cout << "." << endl << ".." << endl;
		dirOp->list_all_directory(curDir);
	}
	else if (args[1] == "-a") {
		//ls -a
		cout << "." << endl << ".." << endl;
		dirOp->list_directory(curDir);
	}
	else if (args[1] == "-l") {
		//ls -l
		dirOp->ll_directory(curDir);
	}
	else {
		
		string fileName = path_to_filename(args[1]);
		Directory*fileDir = path_to_directory(args[1]);
		if (fileDir == NULL) {
			cout << "Error! Can't find the file or directory." << endl;
			return;
		}
		for (int i = 0; i < fileDir->get_fileListNum(); i++) {
			if (fileName == fileDir->get_fileList(i)->get_fileName()) {
				fileDir = fileDir->get_fileList(i);
				break;
			}
		}
		if (fileDir == NULL) {
			cout << "Error! Can't find the file or directory." << endl;
		}
		else {
			if (fileDir->get_type() == '1') {
				cout << fileDir->get_fileName() << endl;
			}
			else
			{
				if (args.size() == 2) {
					dirOp->list_directory(fileDir);
				}
				else {
					if (args[2] == "*") {
						dirOp->list_all_directory(fileDir);
					}
					cout << "unidentification!" << endl;
				}
			}
		}
		
		
	}

}

void cp_instruction(string op) {
	vector<string> args = split(op, " ");

	if (args.size() == 3) {
		//cp file1 file2
		//cp file1 dir
		string sFileName, tFileName;
		sFileName = path_to_filename(args[1]);
		tFileName = path_to_filename(args[2]);
		Directory*sfileDir = NULL, *tfileDir = NULL;
		Directory*slastDir = path_to_directory(args[1]);
		Directory*tlastDir = path_to_directory(args[2]);
		if (slastDir == NULL || tlastDir == NULL) {
			cout << "Error! Can't find the source path." << endl;
			return;
		}
		else {
			for (int i = 0; i < slastDir->get_fileListNum(); i++) {
				if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
					sfileDir = slastDir->get_fileList(i);
					break;
				}
			}
			if (sfileDir == NULL) {
				cout << "Error! Can't find the source file." << endl;
				return;
			}
			if (sfileDir->get_type() == '0') {
				cout << "Don't allow copying directory." << endl;
			}
			else {
				bool flag = false;
				for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
					if (tFileName == tlastDir->get_fileList(i)->get_fileName()) {
						tfileDir = tlastDir->get_fileList(i);
						flag = true;
						break;
					}
				}
				if (flag == true) {
					if (tfileDir->get_type() == '0') {
						//cp file dir
						dirOp->cp_file(sfileDir, tfileDir, sFileName);
					}
					else {
						//cp file1 file2 file2已经存在 
						dirOp->cp_file(sfileDir, tlastDir, tFileName);
					}
				}
				else {
					//cp file1 file2  file2 不存在
					dirOp->cp_file(sfileDir, tlastDir, tFileName);
				}
			}
		}
	}
	else
	{
		//cp file1 file2…filen dir
		string sFileName, tDirName;
		int fileNum = args.size() - 2;
		tDirName = path_to_filename(args[args.size() - 1]);
		Directory*tlastDir = path_to_directory(args[2]);
		Directory*sfileDir = NULL, *tDir = NULL, *slastDir = NULL;
		if (tlastDir == NULL) {
			cout << "Error! Can't find the objective directory." << endl;
			return;
		}
		bool flag = false;
		for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
			if (tDirName == tlastDir->get_fileList(i)->get_fileName()) {
				tDir = tlastDir->get_fileList(i);
				flag = true;
				break;
			}
		}
		if (flag == false) {
			cout << "Error! Can't find the objective directory. " << endl;
			return;
		}
		else {
			sFileName = path_to_filename(args[1]);
			slastDir = path_to_directory(args[1]);
			if (slastDir == NULL) {
				cout << "Error! Can't find the source path." << endl;
				return;
			}
			for (int i = 1; i <= fileNum; i++) {
				sfileDir = NULL;
				bool flag = false;
				for (int i = 0; i < slastDir->get_fileListNum() && flag == false; i++) {
					if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
						sfileDir = slastDir->get_fileList(i);
						flag = true;
						break;
					}
				}
				if (flag == false) {
					cout << "Error! Can't find the file " << sFileName << endl;
				}
				else {
					if (sfileDir->get_type() == '0') {
						cout << "Don't allow copying directory." << endl;
					}
					else {
						dirOp->cp_file(sfileDir, tDir, sFileName);
					}

				}
				sFileName = path_to_filename(args[i + 1]);
				slastDir = path_to_directory(args[i + 1]);
				if (slastDir == NULL) {
					cout << "Error! Can't find the source path." << endl;
					return;
				}
			}
		}
	}
}

void mv_instruction(string op) {
	vector<string> args = split(op, " ");
	if (args.size() == 3) {
		//mv file file2
		//mv file dir
		string sFileName, tFileName;
		sFileName = path_to_filename(args[1]);
		tFileName = path_to_filename(args[2]);
		Directory*sfileDir = NULL, *tfileDir = NULL;
		Directory*slastDir = path_to_directory(args[1]);
		Directory*tlastDir = path_to_directory(args[2]);
		if (slastDir == NULL || tlastDir == NULL) {
			cout << "Error! Can't find the source path." << endl;
		}
		else {
			for (int i = 0; i < slastDir->get_fileListNum(); i++) {
				if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
					sfileDir = slastDir->get_fileList(i);
					break;
				}
			}
			if (sfileDir == NULL) {
				cout << "Error! Can't find the source file." << endl;
				return;
			}
			if (sfileDir->get_type() == '0') {
				//mv dir1 dir2
				//dirOp->mv_dir(slastDir, sfileDir, tlastDir);
				bool flag = false;
				for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
					if (tFileName == tlastDir->get_fileList(i)->get_fileName()) {
						tfileDir = tlastDir->get_fileList(i);
						flag = true;
						break;
					}
				}
				if (flag == true) {
					if (tfileDir->get_type() == '0') {
						//mv dir dir
						dirOp->mv_file(slastDir, sfileDir, tfileDir);
					}
					else {
						//mv file1 file2 file2已经存在 
						cout << "Error! The instruction format is wrong." << endl;
					}
				}
				else {
					cout << "Error! Can' t find the objective directory." << endl;
				}
			}
			else {
				bool flag = false;
				for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
					if (tFileName == tlastDir->get_fileList(i)->get_fileName()) {
						tfileDir = tlastDir->get_fileList(i);
						flag = true;
						break;
					}
				}
				if (flag == true) {
					if (tfileDir->get_type() == '0') {
						//mv file dir
						dirOp->mv_file(slastDir, sfileDir, tfileDir);
					}
					else {
						//mv file1 file2 file2已经存在 
						dirOp->rm_directory(tfileDir, tlastDir);
						sfileDir->set_fileName(tFileName);
						dirOp->mv_file(slastDir, sfileDir, tlastDir);
					}
				}
				else {
					//mv file1 file2  file2 不存在
					dirOp->mv_file(slastDir, sfileDir, tlastDir);
					sfileDir->set_fileName(tFileName);
				}
			}
		}
	}
	else
	{
		//mv file1 file2…filen dir
		string sFileName, tDirName;
		int fileNum = args.size() - 2;
		tDirName = path_to_filename(args[args.size() - 1]);
		Directory*tlastDir = path_to_directory(args[2]);
		Directory*sfileDir = NULL, *tDir = NULL, *slastDir = NULL;
		if (tlastDir == NULL) {
			cout << "Error! Can't find the objective path." << endl;
			return;
		}
		bool flag = false;
		for (int i = 0; i < tlastDir->get_fileListNum() && flag == false; i++) {
			if (tDirName == tlastDir->get_fileList(i)->get_fileName()) {
				tDir = tlastDir->get_fileList(i);
				flag = true;
				break;
			}
		}
		if (flag == false) {
			cout << "Error! Can't find the objective directory. " << endl;
		}
		else {
			sFileName = path_to_filename(args[1]);
			slastDir = path_to_directory(args[1]);
			if (slastDir == NULL) {
				cout << "Error! Can't find the source file." << endl;
				return;
			}
			for (int i = 1; i <= fileNum; i++) {
				sfileDir = NULL;
				bool flag = false;
				for (int i = 0; i < slastDir->get_fileListNum() && flag == false; i++) {
					if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
						sfileDir = slastDir->get_fileList(i);
						flag = true;
						break;
					}
				}
				if (flag == false) {
					cout << "Error! Can't find the file " << sFileName << endl;
				}
				else {
					if (sfileDir->get_type() == '0') {
						cout << "Don't allow copying directory." << endl;
					}
					else {
						dirOp->mv_file(slastDir, sfileDir, tDir);
					}

				}
				sFileName = path_to_filename(args[i + 1]);
				slastDir = path_to_directory(args[i + 1]);
				if (slastDir == NULL) {
					cout << "Error! Can't find the source path." << endl;
					return;
				}
			}
		}
	}
}

void rm_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileName = path_to_filename(args[1]);
	Directory*fileDir = NULL, *lastDir = path_to_directory(args[1]);
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
		if (fileDir == NULL) {
			cout << "Error! Can't find the file." << endl;
			return;
		}
		dirOp->rm_directory(fileDir, lastDir);

	}
}

void ln_instruction(string op) {
	vector<string> args = split(op, " ");
	string sFileName, tFileName;
	sFileName = path_to_filename(args[1]);
	tFileName = path_to_filename(args[2]);
	Directory*sfileDir = NULL;
	Directory*slastDir = path_to_directory(args[1]);
	Directory*tlastDir = path_to_directory(args[2]);
	if (slastDir == NULL) {
		cout << "Error! Can't find the source path." << endl;
	}
	else if (tlastDir == NULL)
	{
		cout << "Error! Can't find the objective path." << endl;
	}
	else {
		for (int i = 0; i < slastDir->get_fileListNum(); i++) {
			if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
				sfileDir = slastDir->get_fileList(i);
				break;
			}
		}
		if (sfileDir == NULL) {
			cout << "Error! Can't find the source file." << endl;
			return;
		}
		if (sfileDir->get_type() == '0') {
			cout << "Don't allow making hard link to directory." << endl;
		}
		else {
			dirOp->ln(sfileDir, tlastDir, tFileName);
		}

	}
}

void su_instruction(string op) {
	vector<string> args = split(op, " ");
	string username = args[1];
	string password;
	cout << "Password:";
	//cin >> password;
	getline(cin, password, '\n');
	if (currentUser->change_user(username, password))
		cout << "Success" << endl;
	else
		cout << "Error" << endl;
}

void chmod_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileName = path_to_filename(args[2]);
	Directory*fileDir = path_to_directory(args[2]);
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
		if (fileDir == NULL) {
			cout << "Error! Can't find the file." << endl;
		}
		int auth[3];
		string authStr = args[1];
		auth[0] = (atoi(authStr.c_str()) - atoi(authStr.c_str()) % 100) / 100;
		auth[1] = (atoi(authStr.c_str()) - auth[0] * 100 - atoi(authStr.c_str()) % 10) / 10;
		auth[2] = atoi(authStr.c_str()) % 10;
		fileDir->set_authority(auth);
	}
}

void chown_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileName = path_to_filename(args[1]);
	Directory*fileDir = path_to_directory(args[1]);
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
		if (fileDir == NULL) {
			cout << "Error! Can't find the file." << endl;
			return;
		}
		fileDir->set_owner(args[2]);
	}
}

void chgrp_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileName = path_to_filename(args[1]);
	Directory*fileDir = path_to_directory(args[1]);
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
		if (fileDir == NULL) {
			cout << "Error! Can't find the file." << endl;
			return;
		}
		fileDir->set_group(args[2]);
	}
}
