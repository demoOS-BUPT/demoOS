#include"instructionOp.h"
using namespace std;

void cat_instruction(string op) {
	vector<string> args = split(op, " ");
	string fileContent;
	string fileName = path_to_filename(args[1]);
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
			if (fileDir->is_authority(currentUser->get_username(), currentUser->get_group(), "r"))
				cout << dirOp->cat_file(fileDir->get_FCBptr(), diskOP) << endl;
			else
				//查看文件信息
				cout << "Permission denied." << endl;
		}
		else {
			cout << "Error! Can't find the file." << endl;
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
		dirOp->list_directory(curDir);
	}
	else if (args[1] == "*" || args[1] == "-a") {
		cout << "." << endl << ".." << endl;
		dirOp->list_all_directory(curDir);
	}
	else {
		string fileName = path_to_filename(args[1]);
		Directory*fileDir = path_to_directory(args[1]);
		for (int i = 0; i < fileDir->get_fileListNum(); i++) {
			if (fileName == fileDir->get_fileList(i)->get_fileName()) {
				fileDir = fileDir->get_fileList(i);
				break;
			}
		}
		dirOp->list_directory(fileDir);
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
		}
		else {
			for (int i = 0; i < slastDir->get_fileListNum(); i++) {
				if (sFileName == slastDir->get_fileList(i)->get_fileName()) {
					sfileDir = slastDir->get_fileList(i);
					break;
				}
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
						dirOp->cp_file(sfileDir, tlastDir, sFileName);
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
		//cp file1 file2…filen dir
		string sFileName, tDirName;
		int fileNum = args.size() - 2;
		tDirName = path_to_filename(args[args.size() - 1]);
		Directory*tlastDir = path_to_directory(args[2]);
		Directory*sfileDir = NULL, *tDir = NULL, *slastDir = NULL;
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
		if (args.size() == 3) {
			if (args[3] == "-r") {
				//dirOp->rm_all_directory(fileDir, lastDir);
			}

		}
		else {
			dirOp->rm_directory(fileDir, lastDir);
		}

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
		if (sfileDir->get_type() == '0') {
			cout << "Don't allow making hard link to directory." << endl;
		}
		else {
			dirOp->ln(sfileDir, tlastDir, tFileName);
		}

	}
}