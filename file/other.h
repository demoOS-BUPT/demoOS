#pragma once
#ifndef OTHER_H
#define OTHER_H
#include"Directory.h"
#include"FCB.h"
using namespace std;

Directory*get_new_Directory(); 
//��Ŀ¼�洢��Ϊ�µ�Ŀ¼����ռ䣬����µ��ļ�Ŀ¼ָ��

FCB*get_new_FCB();
//��FCB�Ĵ洢������������ռ䣬����ļ��µ�FCBָ�룬ʧ�ܷ���null

int get_new_data_block();
//�ڴ����в鿴�������Ƿ�ռ䣬���һ����̺�,ʧ�ܷ���-1

#endif // !OTHER_H
