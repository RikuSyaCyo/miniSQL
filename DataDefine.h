#ifndef _DATA_DEFINE_H
#define _DATA_DEFINE_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef struct _Attribute
{
	string aname;
	int dtype;				//属性的数据类型int是0 float是1 char是2
	int atype;		//属性的主键类型，0是unique 1是null 2是primary 
	int alen; 				//属性所存放的数据的长度，int float长度为1
	int aid;				//属性在表中是第几个
}	_Attribute;



//	用于描述表的信息
typedef struct Table
{
	string tname;
	int anum;				//表中属性的总个数
	_Attribute a[32];	//表的所有属性列表
}	Table;



//	用于描述索引信息
typedef struct Index
{
	string iname;
	string tname;
	string aname;					//	索引所对应的属性
} Index;


//	用于描述判断条件的信息
typedef struct Condition* ConditionList;
typedef struct Condition
{
	string aname;			//条件所对应的属性名
	string value;			//条件所需要进行比较的值
	int dtype;              //item1,item2是什么类型的,0是int,1是float,2是char
	int cmtype;					// 0是>,1是<,2是=,3是>=,4是<=,5是<>
	ConditionList next;	//指向下一个条件
}	Condition;







//	全局变量声明 
extern string error_info;					//用于保存出错信息
extern int cotype;                            //纪录当前操作的类型create table为0 drop table为1 create index为2 drop index为3 insert为4 delete为5 select为6 execfile为6 quit为7 
extern Table table;								//用于创建表、选择纪录、删除纪录、插入纪录等
extern ConditionList conditions;	//用于解析的时候保存where条件
extern Index index;								//用于创建索引
extern string tname;					//用于删除表单
extern string iname;					//用于删除索引
extern string values[32];					//用于insert操作
extern int vnum;						//用于insert操作


#endif