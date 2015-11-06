#pragma once
#include <string>
#include <iostream>
#include "BufferPool.h"
#include "FilePosition.h"
#include "Attribute.h"
#include "TupleManager.h"
#include "ConstantTable.h"
#include "TupleResults.h"
using namespace std;

extern BufferPool buffer;

class TableManager
{
private:
	char  tableName[TABLENAMELENGTH]; //table name
	FilePosition filePos; //存table的文件和block
	int attriArrayTop; //属性队列尾指针。没有为0
	Attribute attr[ATTRIMAXNUM];//属性
	bool delFlag;//删除flag
	int tupleCount;//tuple数量，没有为0

	void readTableBlock();//读取table信息
	void writeTableBlock();//存储table信息
	void setName(string tabName);//设置table name
	bool hasIndexOn(string attr);//属性attr上是否有Index
	template <typename T>
	TupleResults SelWithIndex(string attr, int op, T value);//通过Index查找
	template <typename T>
	TupleResults SelWithoutIndex(string attr, int op, T value);//不通过index查找
	int getAttrIndex(string attr);//返回属性attr在属性数组中的下标
	Attribute getAttri(int attrIndex);//返回下标为attrIndex的属性
	int getAttrOffset(int attrIndex);//返回下标0~attrIndex的属性的存储总字节数
	int getAttriCount();//返回当前属性个数
	void Delete();//删除table

public:
	friend class CatalogManager;
	friend class TupleManager;

	TableManager(FilePosition fPos);
	TableManager(FilePosition fPos, string tabName, bool flag);
	~TableManager();
	void InsertAttribute(string attname, int type,int uniqueInf); //增加属性（int或float）
	void InsertAttribute(string attname, int type, int charLength,int uniqueInf);//增加属性（char）
	void CreateIndex(string attr);//创建Index
	void DeleteIndex(string attr);//删除index
	TupleManager CreateNewTuple();//新增一个tuple
	template <typename T>
	TupleResults selectTuples(string attr, int op, T value);//查询tuple
	template <typename T>
	TupleResults deleteTuples(string attr, int op, T value);//删除tuple操作
	TupleResults getAllTuples();//******************************
	string strName();//返回table name
	bool isDelete();//table是否已删除
	void disTableInf();//显示table信息
	void disAllTuples();//显示所有tuple
	int attributeCount();
	int getAttributeType(int attriIndex);
	string getAttriName(int attriIndex);
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
	TupleResults results(strName());
	switch (op)
	{
	case EQUAL:
		break;
	case NOEQUAL:
		break;
	case LESSTHAN:
		break;
	case GREATERTHAN:
		break;
	case NOGREATERTHAN:
		break;
	case NOLESSTHAN:
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
	//需要告知b+树
	writeTableBlock();
	return results;
}

