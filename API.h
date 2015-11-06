#ifndef	API_H
#define	API_H


#include "DataDefine.h"


//	API������
void API();

void ReadTable();

void ReadIndex(string iname);

//	������ʱ���ڲ�����
bool CallCreateTable();

//	ɾ����ʱ���ڲ�����
bool CallDropTable();

//	��������ʱ���ڲ�����
bool CallCreateIndex();

//	ɾ������ʱ���ڲ�����
bool CallDropIndex();

//	�����¼ʱ���ڲ�����
bool CallInsert();

//	ɾ����¼ʱ���ڲ�����
bool CallDelete();

//	ѡ���¼ʱ���ڲ�����
bool CallSelect();




/*******************************************************************************
//	��������ҪRecordģ��ʵ�ֵĹ���

//	ʵ��ѡ���¼�Ĺ���
bool Select(Table table, Condition_list conditions);

//	ʵ�ֲ����¼�Ĺ���
bool Insert(Table table, string attr_value[]);

//	ʵ��ɾ����¼�Ĺ���
bool Delete(Table table, Condition_list conditions);
********************************************************************************/

/*******************************************************************************/

#endif
