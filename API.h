#ifndef	API_H
#define	API_H


#include "DataDefine.h"


//	API������
void API();

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


#endif
