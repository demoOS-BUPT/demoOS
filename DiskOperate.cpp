
typedef class DiskOperate{
public:
	bool write(int startBlock, string content);//从哪开始写，写啥
	string read(int startBlock, int dataSize);//从哪，多长
}DiskOperate;

bool DiskOperate::write(int startBlock, string content){
    for(;;)
}

string DiskOperate::read(int startBlock, int dataSize){

}