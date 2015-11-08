#pragma once
#include <string>
#include <iostream>
#include "BufferPool.h"
#include "FilePosition.h"
#include "Attribute.h"
#include "TupleManager.h"
#include "ConstantTable.h"
#include "TupleResults.h"
#include "BPTree.h"
#include "IndexCatelog.h"
#include <math.h>
using namespace std;

extern BufferPool buffer;
extern CatalogManager catalog;

class CatalogManager;

class TableManager
{
private:
	char  tableName[TABLENAMELENGTH]; //table name
	FilePosition filePos; //存table的文件和block
	int attriArrayTop; //属性队列尾指针。没有为0
	Attribute attr[ATTRIMAXNUM];//属性
	bool delFlag;//删除flag
	int tupleCount;//tuple数量，没有为0

	TableManager(FilePosition fPos, string tabName, bool flag);
	void readTableBlock();//读取table信息
	void writeTableBlock();//存储table信息
	void setName(string tabName);//设置table name
	bool hasIndexOn(int attrIndex);
	template <typename T>
	TupleResults SelWithIndex(string attr, int op, T value);//通过Index查找
	//TupleResults SelWithIndex(string attr, int op, int value);
	//TupleResults SelWithIndex(string attr, int op, float value);
	//TupleResults SelWithIndex(string attr, int op, string value);
	template <typename T>
	TupleResults SelWithoutIndex(string attr, int op, T value);//不通过index查找
	Attribute getAttri(int attrIndex);//返回下标为attrIndex的属性
	int getAttrOffset(int attrIndex);//返回下标0~attrIndex的属性的存储总字节数
	int getAttriCount();//返回当前属性个数
	void Delete();//删除table
	void _DeleteIndex(string attrName);
	void DeleteIndex(string attrName);//删除index
	void _CreateIndex(string attrName,string indexName);
	void CreateIndex(string attrName, string indexName);//创建Index 
public:
	friend class CatalogManager;
	friend class TupleManager;
	TableManager(FilePosition fPos);
	~TableManager();
	void InsertAttribute(string attname, int type,int uniqueInf); //增加属性（int或float）
	void InsertAttribute(string attname, int type, int charLength,int uniqueInf);//增加属性（char）
	TupleManager CreateNewTuple();//新增一个tuple
	template <typename T>
	TupleResults selectTuples(string attrName, int op, T value);//查询tuple
	TupleResults selectTuples(string attrName, int op, string value);
	template <typename T>
	TupleResults deleteTuples(string attr, int op, T value);//删除tuple操作
	TupleResults getAllTuples();//返回所有tuples
	string strName();//返回table name
	bool isDelete();//table是否已删除
	void disTableInf();//显示table信息
	void disAllTuples();//显示所有tuple
	int attributeCount();
	int getAttributeType(int attriIndex);
	string getAttriName(int attriIndex);
	bool isUnique(int attrIndex);
	bool hasIndexOn(string attr);//属性attr上是否有Index
	int getAttrIndex(string attr);//返回属性attr在属性数组中的下标
};


template <typename T>
TupleResults TableManager::selectTuples(string attr, int op, T value)
{
	readTableBlock();
	if (hasIndexOn(attr))
	{
		return SelWithIndex(attr, op, value);
	}
	else{
		return SelWithoutIndex(attr, op, value);
	}
}

template <typename T>
TupleResults TableManager::SelWithIndex(string attr, int op, T value) //若不限制调用者使用字符串常量，此处T有可能为const char*
{
	string fileName = strName() + attr + IndexFilePostfix;
	TupleResults results(strName());
	int n = 0;
	switch (op)
	{
	case EQUAL:
		n = searchIndex(fileName, value);
		if (n>=0)
			results.insert(n);
		break;
	case NOEQUAL:
		return SelWithoutIndex(attr,op,value);
		break;
	case LESSTHAN:
		results.Set(searchLessThan(fileName, value, 0));
		break;
	case GREATERTHAN:
		results.Set(searchBiggerThan(fileName, value, 0));
		break;
	case NOGREATERTHAN:
		results.Set(searchLessThan(fileName, value, 1));
		break;
	case NOLESSTHAN:
		results.Set(searchBiggerThan(fileName, value, 1));
		break;
	default:
		break;
	}
	return results;
}

template <typename T>
TupleResults TableManager::SelWithoutIndex(string attr, int op, T value)
{
	FilePosition fPos;
	fPos.setFilePath(strName() + TupleFilePostfix);
	TupleResults results(strName());
	int attrIndex = getAttrIndex(attr);
	for (int i = 0; i < tupleCount; i++)
	{
		fPos.blockNo = baseIndex + i;
		TupleManager tuple(strName(), fPos);
		if (tuple.isDelete() == false)
		{
			if (tuple.satisfy(attrIndex, op, value))
				results.insert(i);		
		}
	}
	return results;
}

template <typename T>
TupleResults TableManager::deleteTuples(string attr, int op, T value)
{
	TupleResults results = selectTuples(attr, op, value);
	for (int i = 0; i < results.size(); i++)
	{
		TupleManager tuple(strName(), results.getTupleIndexAt(i));
		tuple.Delete();
	}
	writeTableBlock();
	return results;
}

