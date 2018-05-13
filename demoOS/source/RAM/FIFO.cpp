#include <iostream>

#include <stdlib.h>
#include <cstring>
#include <math.h>
//#include "source/mainwindow.h"
using namespace std;
#define pagesize 1024	//页面大小1K
#define msize  65536	//内存大小64K

struct pcb
{
	int pcbid;		//进程id
	size_t needsize;//进程大小
	size_t needpage;//进程页数
	int*  pagetable;//进程页表
	pcb*  next;
	pcb(int id = 0, size_t need=0) :pcbid(id), needsize(need), next(NULL)
	{
		needpage = ceil((float)need / pagesize);
		pagetable = new int[need];
	}
};

class paging
{
	private:
		size_t memsize;		//内存大小
		size_t framenum;	//总页框数 
		size_t freeblock;	// 空闲页框数 
		size_t pcbnum;		//进程数量
		pcb*   pcbheader;	//进程链表头指针
		int*   usage;		//空闲状态表 : 0空闲，1占用 
		char*  simmemory;	//字符串模拟内存空间
	public:
		paging() :memsize(msize), pcbnum(0)
		{
			framenum= msize/pagesize;
			freeblock= msize/pagesize; 
			pcbheader = new pcb();
			usage= new int[msize/pagesize];
			int i;
			for(i=0;i<msize/pagesize;i++) usage[i]=0; 
			simmemory= new char[msize];
			for(i=0;i<msize;i++) simmemory[i]=' '; 
		}
		bool push(int id, int mem, int flag, char* message)	//添加进程，传参id、大小、置换策略及内容 
		{
			bool ret = false;
			pcb* newpcb = new pcb(id, mem);
			newpcb->next = pcbheader->next;
			pcbheader->next = newpcb;
			pcbnum++;
			ret = distribute(newpcb, flag, message);
			if (!ret)
			{
				cout<<id<<"  allocation failed";
				pcbheader->next = newpcb->next;
				delete(newpcb);
				pcbnum--;
			}
			return ret;
		}
		bool pop(int id)
		{
			pcb* prev=findp(id);
			if(!prev)	return false;
			pcb * cur = prev->next;
			memrecover(cur);
			prev->next=cur->next;
			delete cur;
			return true; 
		}
		void printinfo()
		{
			pcb* cur = pcbheader->next;
			while (cur)
			{
				cout << "进程" << cur->pcbid<<endl;
				for (int i = 0; i < cur->needpage; i++)
					cout << "页" << i << "页框" << cur->pagetable[i] << endl;
				cur = cur->next;
			}
			cout<<endl; 
		}
		char* read(int id)
		{
			pcb* prev=findp(id);
			pcb* cur=prev->next;
			char*retmessage=new char[cur->needsize];
			for(int i=0;simmemory[cur->pagetable[i]*pagesize]!=' ';i++)
			{
				memcpy(retmessage+i*pagesize,simmemory+cur->pagetable[i]*pagesize,pagesize);
			}
			return retmessage;
		}
	protected:
		bool distribute(pcb* _pcb, int flag, char* message)
		{
			//cout<<freeblock<<endl;
			if (freeblock < _pcb->needpage) 
			{
				
				size_t pagedef=  _pcb->needpage - freeblock;
				switch (flag)
				{
					case 0: 
				//	cout<<"here"<<endl;
						fifo(pagedef);
						break;
					case 1:
					//	lru(pagedef);
						break;
				}
			}
			int i,getpage;
				for (i = 0, getpage = 0; getpage < _pcb->needpage && i < framenum; i++)
					if (usage[i] == 0)
					{
						usage[i] = 1;
						freeblock--;
						_pcb->pagetable[getpage] = i;
						getpage++;
					}
				size_t mespage=ceil((double)strlen(message)/pagesize);
				for (i=0;i<mespage;i++)
				{
					memcpy(simmemory+_pcb->pagetable[i]*pagesize,message+i*pagesize,pagesize);
				}
				return true;
		}
		void memrecover(pcb* pptr)
		{
			for(int i=0; i<pptr->needpage;i++)
				usage[pptr->pagetable[i]]=0;
			pcbnum--;
			freeblock+=pptr->needpage;
		}
		pcb* findp(int id)
		{
			if(!pcbheader->next)
				return NULL;
			pcb* prev= pcbheader;
			pcb* cur = prev->next;
			while(cur&&cur->pcbid!=id)
			{
				prev=cur;
				cur=cur->next;
			}
			if(!cur)
				return NULL;
			return prev;
		}
		void fifo(int pagedef)
		{
			int recycled=0;
			pcb* prev,*cur;
			while(recycled<pagedef)
			{
				prev=pcbheader;
				cur=prev->next;
				for(cur;cur->next;cur=cur->next) prev=prev->next;
				for(int i=0;i<cur->needpage;i++)
					usage[cur->pagetable[i]]=0;
				recycled+=cur->needpage;
				prev->next=NULL;
				//free (cur);
			}
			freeblock+=recycled;
		}
		
};/*
int main()
{
paging* PGESIM;
string getmess;
PGESIM=new paging();
for(int id=1;id<6;id++)

	{
	PGESIM->push(id,32768,0,"sssddsfsdssss");
	PGESIM->printinfo();
	}

/*PGESIM->pop(4);
PGESIM->push(88,3256,1,"AAAA");
cout<< PGESIM->read(88);
PGESIM->printinfo();*/
	
//}
