#ifndef FIFO
#define FIFO
#include <iostream>

#include <stdlib.h>
#include <cstring>
#include <math.h>
//#include "source/mainwindow.h"
using namespace std;
#define pagesize 1024	//ҳ���С1K
#define msize  65536	//�ڴ��С64K

struct pcb
{
	int pcbid;		//����id
	size_t needsize;//���̴�С
	size_t needpage;//����ҳ��
	int*  pagetable;//����ҳ��
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
		size_t memsize;		//�ڴ��С
		size_t framenum;	//��ҳ���� 
		size_t freeblock;	// ����ҳ���� 
		size_t pcbnum;		//��������
		pcb*   pcbheader;	//��������ͷָ��
		int*   usage;		//����״̬�� : 0���У�1ռ�� 
		char*  simmemory;	//�ַ���ģ���ڴ�ռ�
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
        ~paging(){
            delete pcbheader;
            delete usage;
            delete simmemory;
        }

		bool push(int id, int mem, int flag, char* message)	//��ӽ��̣�����id����С���û����Լ����� 
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
				cout << "����" << cur->pcbid<<endl;
				for (int i = 0; i < cur->needpage; i++)
					cout << "ҳ" << i << "ҳ��" << cur->pagetable[i] << endl;
				cur = cur->next;
			}
			cout<<endl; 
		}
		char* read(int id)
		{
			pcb* prev=findp(id);
            if(prev==NULL){
                char* ret=new char[2];
                ret[0]='x';
                ret[1]=0;
                return ret;
            }
			pcb* cur=prev->next;
			char*retmessage=new char[cur->needsize];                     
            for(int i=0;i<cur->needpage&&simmemory[cur->pagetable[i]*pagesize]!=' ';i++)
			{
				memcpy(retmessage+i*pagesize,simmemory+cur->pagetable[i]*pagesize,pagesize);
			}
			return retmessage;
		}
        int visit(int id, int address)
        {
            pcb* cur = findp(id);

            if (cur == NULL)
                return -1; //�ý��̲�����
            cur = cur->next;
            if (address > cur->needsize)
                return -2; //�߼���ַ���
            int pagenum=address/pagesize;
            address=address%pagesize;
            qDebug()<<"m normal\n";
            return cur->pagetable[pagenum]*pagesize+address;
        }
        int PcbMem_base(int id)
        {
            pcb* cur = findp(id);
            if (cur == NULL)
            {
                return -1;
            }
            cur = cur->next;
            return cur->pagetable[0];
        }

        int PcbMem_size(int id)
        {
            pcb* cur = findp(id);
            if (cur == NULL)
            {
                return -1;
            }
            cur = cur->next;
            return cur->needsize;
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
                        return false;
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

#else
#endif
