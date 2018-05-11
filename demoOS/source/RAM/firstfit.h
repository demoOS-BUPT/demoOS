#ifndef _MEMORY_
#define _MEMORY_

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <QtCore>
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
	public:
		Firstfit(size_t total) :_memNum(total), _blockNum(1), _pcbNum(0)
		{
			_pcbHead = new Pcb();
			_memHead = new memList(0, total);	  //全部空闲
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
		
		void push(int id, int mem, int flag)		//添加一个进程,id为进程id，mem为进程所需内存，flag=0为最先匹配，flag=1为最优匹配
		{
			
			Pcb* newPcb = new Pcb(id, mem);
			newPcb->next = _pcbHead->next;
			_pcbHead->next = newPcb;
			switch (flag)
			{
			case 0:
				_distribute(newPcb, mem);
				break;
			case 1:
				_distribute_(newPcb, mem);
				break;
			}
			
			++_pcbNum;
		}

		void pop(int id)   //删除一个进程
		{

			//若删除第一个该怎么办
			Pcb* prev = _find(id);   //返回该进程的前一个进程
			if (NULL == prev)    //没有该进程
			{
				cout << "This process doesn't exisit" << endl;
				return;
			}
			Pcb* cur = prev->next;
			_memRecover(cur);	//分情况处理要回收的内存
			prev->next = cur->next;
			_memNum += cur->needMem;
			delete cur;
			
			cout << "Success!" << endl;
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
                cout << "This process doesn't exisit" << endl;
                return;
            }
            cur = cur->next;
            cout << cur->pcbid << ":" << cur->begin << "~" << cur->end << endl;
        }

	protected:
		bool pcbEmpty()
		{
			return _pcbHead->next == NULL;
		}

		void _distribute(Pcb* pcb, int size)
		{
			memList* cur = _memHead;
			if (size > _memNum)
			{
				cout << "Memory allocation failed1" << endl;
				return;
			}
			while (cur && cur->msize < size)
			{
				cur = cur->next;
			}

			if (NULL == cur)
			{
				cout << "Memory allocation failed2" << endl;
				exit(-1);
			}

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
		}

		void _distribute_(Pcb* pcb, int size)
		{
			memList* cur = _memHead;
			memList* best = NULL;
			int cur_best = 9999;
			if (size > _memNum)
			{
				cout << "Memory allocation failed" << endl;
				return;
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
			{
				cout << "Memory allocation failed" << endl;
				exit(-1);
			}

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
		}

		void _memRecover(Pcb* pcbptr)
		{
			size_t begin = pcbptr->begin;
			size_t end = pcbptr->end;
			size_t size = end - begin;
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
