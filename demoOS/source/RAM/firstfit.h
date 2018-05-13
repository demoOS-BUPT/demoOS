#ifndef _MEMORY_
#define _MEMORY_

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <QtCore>
#include <cstring>
//#include "source/mainwindow.h"
using namespace std;

struct Pcb//进程单元
{
	int			pcbid;
	size_t		needMem; //申请的内存大小
	size_t		realMem; //实际内存
	size_t		begin;
	size_t		end;
	Pcb*			next;

	Pcb(int id = 0, int need = 0)
		:pcbid(id), needMem(need), realMem(0), begin(0), end(0), next(NULL)
	{}
};

struct memList	//空闲内存链表
{
	size_t		msize;	//大小
	size_t		first;	//起始地址
	size_t		end;		//终止地址
	memList*		next;
	memList*		prev;
	memList(size_t f = 0, size_t e = 0)
		:msize(e-f),first(f), end(e), next(NULL), prev(NULL)
	{}
};


class Firstfit 
{
	private:
		size_t		_memNum;			//总内存大小
		size_t		_blockNum;	    	//内存块数目
		size_t		_pcbNum;			//进程数目
		Pcb*			_pcbHead;		//进程链表
		memList*		_memHead;		//内存链表
		char*			simmemory;		//模拟内存空间的字符串 
	public:
		Firstfit(size_t total) :_memNum(total), _blockNum(1), _pcbNum(0)
		{
			_pcbHead = new Pcb();
			_memHead = new memList(0, total);	  //全部空闲
			simmemory= new char[total];
			for(int i=0;i<total;i++) simmemory[i]=' '; 
		}

		/*void push()  // 读取添加进程数量，及依次读取ID和内存大小
		{
			int size = 0;
			cout << "Please input the number of processes" << endl;
			cin >> size;
			for (int i = 0; i < size; ++i)
			{
				int id;
				size_t mem;
				cout << "Please input <ID> and <size> of each process" << endl;
				cin >> id >> mem;
				push(id, mem);
			}
		}*/
		
        bool push(int id, int mem, int flag, char* message)		//添加一个进程,id为进程id，mem为进程所需内存，flag=0为最先匹配，flag=1为最优匹配
		{
            bool ret=false;
			Pcb* newPcb = new Pcb(id, mem);
			newPcb->next = _pcbHead->next;
			_pcbHead->next = newPcb;
			switch (flag)
			{
			case 0:
                ret=_distribute(newPcb, mem, message);
				break;
			case 1:
                ret=_distribute_(newPcb, mem, message);
				break;
			}
			
			++_pcbNum;
            return ret;
		}

        bool pop(int id)   //删除一个进程
		{

			//若删除第一个该怎么办
			Pcb* prev = _find(id);   //返回该进程的前一个进程
			if (NULL == prev)    //没有该进程
			{
				cout << "This process doesn't exisit" << endl;
                return false;
			}
			Pcb* cur = prev->next;
			_memRecover(cur);	//分情况处理要回收的内存
			prev->next = cur->next;
			_memNum += cur->needMem;
			delete cur;
			
            return true;
		}

		void PrintMemList()
		{
			memList* cur = _memHead;
			int i = 1;
			while (cur)
			{
				cout << "Hole." << i++ << " Address:" << cur->first << "~" << cur->end << "----> ";
				cur = cur->next;
			}
			cout << "NULL\n" << endl;
		}

        QList<int> PrintMemList_qt()//返回值每两个元素为一个空洞
        {
            QList<int> ret;
            memList* cur = _memHead;
            int i = 1;
            while (cur)
            {
                ret<<cur->first<<cur->end;
                cur = cur->next;
            }
            return ret;
        }

        QString PrintMemList_qstr()
        {
            QString ret;
            memList* cur = _memHead;
            int i = 1;
            while (cur)
            {
                ret += QString("Hole.%1 Address:"
                               "%2~%3"
                               "----> ").arg(i++).arg(cur->first).arg(cur->end);
                cur = cur->next;
            }
            ret+="NULL\n";
            return ret;
        }

		void PrintPcbMem(int id)
		{
			Pcb* cur = _find(id);
			if (cur == NULL)
			{
				cout << "This process doesn't exisit" << endl;
				return;
			}
			cur = cur->next;
			cout << cur->pcbid << ":" << cur->begin << "~" << cur->end << endl;
		}

        QString PrintPcbMem_qt(int id)
        {
            Pcb* cur = _find(id);
            if (cur == NULL)
            {
                return"This process doesn't exisit";
            }
            cur = cur->next;
            return QString("%1:%2~%3\n").arg(cur->pcbid).arg(cur->begin).arg(cur->end);
        }

        int PcbMem_base(int id)
        {
            Pcb* cur = _find(id);
            if (cur == NULL)
            {
                return -1;
            }
            cur = cur->next;
            return cur->begin;
        }

        int PcbMem_size(int id)
        {
            Pcb* cur = _find(id);
            if (cur == NULL)
            {
                return -1;
            }
            cur = cur->next;
            return cur->end-cur->begin;
        }
        
		char * read(int id)
		{
			Pcb* cur = _find(id);
		    if (cur == NULL)
                return "This process doesn't exisit";
            int len = 15;//cur->needMem???
            char* retmessage=new char[len];
            memcpy(retmessage,simmemory+cur->begin,len);
            return retmessage;
		}

	protected:
		bool pcbEmpty()
		{
			return _pcbHead->next == NULL;
		}

        bool _distribute(Pcb* pcb, int size, char* message)
		{
			memList* cur = _memHead;
			if (size > _memNum)
			{
				cout << "Memory allocation failed1" << endl;
                return false;
			}
			while (cur && cur->msize < size)
			{
				cur = cur->next;
			}

			if (NULL == cur)
			{
                qDebug()  << "Memory allocation failed2" << endl;
				exit(-1);
                return false;
			}
			memcpy(simmemory+cur->first,message,(strlen(message)));
            pcb->begin = cur->first;
			pcb->end = pcb->begin + size;
			pcb->realMem = size;
			cur->first += size;
			cur->msize = cur->end - cur->first;
			if (cur->first == cur->end)
			{
				if (cur == _memHead)
					_memHead = cur->next;
				else
				{
					memList* prev = cur->prev;
					prev->next = cur->next;
				}
				delete(cur);
			}
			_memNum -= size;
            qDebug() << "Success!" << endl;
            return true;
		}

        bool _distribute_(Pcb* pcb, int size, char* message)
		{
			memList* cur = _memHead;
			memList* best = NULL;
			int cur_best = 9999;
			if (size > _memNum)
            {   qDebug()<<"too big!!";
                cout << "Memory allocation failed" << endl;
                return false;
			}
			while (cur)
			{
				if (cur->msize > size && cur->msize < cur_best)
				{
					cur_best = cur->msize;
					best = cur;
				}
				cur = cur->next;
			}
			cur = best;
			if (NULL == cur)
            {   qDebug()<<"no best";
				cout << "Memory allocation failed" << endl;
				exit(-1);
                return false;
			}
			memcpy(simmemory+cur->first,message,(strlen(message)));
			pcb->begin = cur->first;
			pcb->end = pcb->begin + size;
			pcb->realMem = size;
			cur->first += size;
			cur->msize = cur->end - cur->first;
			if (cur->first == cur->end)
			{
				if (cur == _memHead)
					_memHead = cur->next;
				else
				{
					memList* prev = cur->prev;
					prev->next = cur->next;
				}
				delete(cur);
			}
			_memNum -= size;
			cout << "Success!" << endl;
            return true;
		}

		void _memRecover(Pcb* pcbptr)
		{
			size_t begin = pcbptr->begin;
			size_t end = pcbptr->end;
			size_t size = end - begin;
			for(int j=begin; j < end; j++)	simmemory[j]=' ';
			memList* prev = _memHead;
			if (prev == NULL)
			{
				memList* mcur = new memList(begin, end);
				mcur->next = prev;
				_memHead = mcur;
				return;
			}
			memList* cur = _memHead->next;

			if (end <= prev->first)  //在第一块前面，需要修改_memHead
			{
				if (end == prev->first)
				{
					prev->first -= size;
				}
				else if (end < prev->first)
				{
					memList* mcur = new memList(begin, end);
					mcur->next = prev;
					prev->prev = mcur;
					_memHead = mcur;
				}
				return;
			}

			while (cur)  //cur 非空
			{
				if (begin == prev->end)  //在该块的后面
				{
					if (end == cur->first) // 1.前后可以回收,则合并成一块
					{
						prev->end = cur->end;
						if (NULL != cur->next)
						{
							cur->next->prev = prev;
						}
						prev->next = cur->next;
						delete cur;
					}
					else if (end < cur->first) // 2 和前面的合并
					{
						prev->end += size;
					}
					return;
				}
				else if (begin > prev->end && end <= cur->end)
				{
					if (end == cur->first)  //3 和后面的块相邻
					{
						cur->first -= size;
					}
					else if (end < cur->first)  //4与所有前后都不能链接
					{
						memList* mcur = new memList(begin, end);
						prev->next = mcur;
						mcur->prev = prev;
						mcur->next = cur;
						cur->prev = mcur;
					}
					return;
				}
				prev = cur;
				cur = cur->next;
			}

			if (cur == NULL)   //  可能是最后的块的，或者最前面的块
			{
				if (begin == prev->end)
				{
					prev->end += size;
				}
				else if (begin > prev->end)
				{
					memList* mcur = new memList(begin, end);
					prev->next = mcur;
					mcur->prev = prev;
				}
				return;
			}
		}

		Pcb* _find(int id)
		{
			if (pcbEmpty())  //进程队列为空
			{
				return NULL;
			}
			Pcb* prev = _pcbHead;
			Pcb* cur = prev->next;
			while (cur && cur->pcbid != id)
			{
				prev = cur;
				cur = cur->next;
			}

			if (NULL == cur)  //没找到
				return NULL;
			return prev;  //找到，返回它的前一个节点
		}
		

};





#endif
