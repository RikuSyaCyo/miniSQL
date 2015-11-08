#ifndef _DATA_DEFINE_H
#define _DATA_DEFINE_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef struct _Attribute
{
	string aname;
	int dtype;				//���Ե���������int��0 float��1 char��2
	int atype;		//���Ե��������ͣ�0��unique 1��null 2��primary 
	int alen; 				//��������ŵ����ݵĳ��ȣ�int float����Ϊ1
	int aid;				//�����ڱ����ǵڼ���
}	_Attribute;



//	�������������Ϣ
typedef struct Table
{
	string tname;
	int anum;				//�������Ե��ܸ���
	_Attribute a[32];	//������������б�
}	Table;



//	��������������Ϣ
typedef struct Index
{
	string iname;
	string tname;
	string aname;					//	��������Ӧ������
} Index;


//	���������ж���������Ϣ
typedef struct Condition* ConditionList;
typedef struct Condition
{
	string aname;			//��������Ӧ��������
	string value;			//��������Ҫ���бȽϵ�ֵ
	int dtype;              //item1,item2��ʲô���͵�,0��int,1��float,2��char
	int cmtype;					// 0��>,1��<,2��=,3��>=,4��<=,5��<>
	ConditionList next;	//ָ����һ������
}	Condition;







//	ȫ�ֱ������� 
extern string error_info;					//���ڱ��������Ϣ
extern int cotype;                            //��¼��ǰ����������create tableΪ0 drop tableΪ1 create indexΪ2 drop indexΪ3 insertΪ4 deleteΪ5 selectΪ6 execfileΪ6 quitΪ7 
extern Table table;								//���ڴ�����ѡ���¼��ɾ����¼�������¼��
extern ConditionList conditions;	//���ڽ�����ʱ�򱣴�where����
extern Index index;								//���ڴ�������
extern string tname;					//����ɾ����
extern string iname;					//����ɾ������
extern string values[32];					//����insert����
extern int vnum;						//����insert����


#endif