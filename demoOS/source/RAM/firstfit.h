#ifndef _MEMORY_
#define _MEMORY_

#include <iostream>
#include <assert.h>
#include <stdlib.h>
using namespace std;

struct Pcb//���̵�Ԫ
{
	int			pcbid;
	size_t		needMem; //������ڴ��С
	size_t		realMem; //ʵ���ڴ�
	size_t		begin;
	size_t		end;
	Pcb*			next;

	Pcb(int id = 0, int need = 0)
		:pcbid(id), needMem(need), realMem(0), begin(0), end(0), next(NULL)
	{}
};

struct memList	//�����ڴ�����
{
	size_t		msize;	//��С
	size_t		first;	//��ʼ��ַ
	size_t		end;		//��ֹ��ַ
	memList*		next;
	memList*		prev;
	memList(size_t f = 0, size_t e = 0)
		:msize(e-f),first(f), end(e), next(NULL), prev(NULL)
	{}
};

namespace FIRSTFIT   
{//�״���Ӧ�㷨

	class Firstfit 
	{
	private:
		size_t		_memNum;			//���ڴ��С
		size_t		_blockNum;	    	//�ڴ����Ŀ
		size_t		_pcbNum;			//������Ŀ
		Pcb*			_pcbHead;		//��������
		memList*		_memHead;		//�ڴ�����
	public:
		Firstfit(size_t total) :_memNum(total), _blockNum(1), _pcbNum(0)
		{
			_pcbHead = new Pcb();
			_memHead = new memList(0, total);	  //ȫ������
		}

		/*void push()  // ��ȡ��ӽ��������������ζ�ȡID���ڴ��С
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
		
		void push(int id, int mem, int flag)		//���һ������,idΪ����id��memΪ���������ڴ棬flag=0Ϊ����ƥ�䣬flag=1Ϊ����ƥ��
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

		void pop()   //ɾ��һ������
		{
			int id = 0;
			cout << "Please input <ID> of the process to be deleted:" << endl;
			cin >> id;

			//��ɾ����һ������ô��
			Pcb* prev = _find(id);   //���ظý��̵�ǰһ������
			if (NULL == prev)    //û�иý���
			{
				cout << "This process doesn't exisit" << endl;
				return;
			}
			Pcb* cur = prev->next;
			_memRecover(cur);	//���������Ҫ���յ��ڴ�
			prev->next = cur->next;
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

		void PrintPcbMem()
		{
			int id = 0;
			cout << "Please input <ID> of the process:";
			cin >> id;
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
				cout << "Memory allocation failed" << endl;
				return;
			}
			while (cur && cur->msize < size)
			{
				cur = cur->next;
			}

			if (NULL == cur)
			{
				cout << "Memory allocation failed" << endl;
				exit(-1);
			}

			pcb->begin = cur->first;
			pcb->end = pcb->begin + size;
			pcb->realMem = size;
			cur->first += size;
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
			cur = best
			if (NULL == cur)
			{
				cout << "Memory allocation failed" << endl;
				exit(-1);
			}

			pcb->begin = cur->first;
			pcb->end = pcb->begin + size;
			pcb->realMem = size;
			cur->first += size;
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

			if (end <= prev->first)  //�ڵ�һ��ǰ�棬��Ҫ�޸�_memHead
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

			while (cur)  //cur �ǿ�
			{
				if (begin == prev->end)  //�ڸÿ�ĺ���
				{
					if (end == cur->first) // 1.ǰ����Ի���,��ϲ���һ��
					{
						prev->end = cur->end;
						if (NULL != cur->next)
						{
							cur->next->prev = prev;
						}
						prev->next = cur->next;
						delete cur;
					}
					else if (end < cur->first) // 2 ��ǰ��ĺϲ�
					{
						prev->end += size;
					}
					return;
				}
				else if (begin > prev->end && end <= cur->end)
				{
					if (end == cur->first)  //3 �ͺ���Ŀ�����
					{
						cur->first -= size;
					}
					else if (end < cur->first)  //4������ǰ�󶼲�������
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

			if (cur == NULL)   //  ���������Ŀ�ģ�������ǰ��Ŀ�
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
			if (pcbEmpty())  //���̶���Ϊ��
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

			if (NULL == cur)  //û�ҵ�
				return NULL;
			return prev;  //�ҵ�����������ǰһ���ڵ�
		}

	};

}



#endif