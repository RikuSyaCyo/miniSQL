#pragma once
#include <string>
#include <iostream>
#include "BufferPool.h"
#include "FilePosition.h"
#include "Attribute.h"
#include "TupleManager.h"
#include "ConstantTable.h"
using namespace std;

extern BufferPool buffer;

class TableManager
{
private:
	char  tableName[TABLENAMELENGTH];
	FilePosition filePos;
	int attriArrayTop; //没有为0
	Attribute attr[ATTRIMAXNUM];
	bool delFlag;
	int tupleCount;//tuple数量，没有为0
	void readTableBlock();
	void writeTableBlock();
	void setName(string tabName);
	bool hasIndexOn(string attr);
	template <typename T>
	TupleResults SelWithIndex(string attr, int op, T value);
	template <typename T>
	TupleResults SelWithoutIndex(string attr, int op, T value);
	int getAttrIndex(string attr);
public:
	TableManager(FilePosition fPos);
	TableManager(FilePosition fPos, string tabName, bool flag);
	~TableManager();
	void InsertAttribute(string attname, int type);
	void InsertAttribute(string attname, int type, int charLength);
	Attribute getAttri(int attrIndex) const;
	void CreateIndex(string attr);
	void DeleteIndex(string attr);
	TupleManager CreateNewTuple();
	void Delete();
	template <typename T>
	TupleResults selectTuples(string attr, int op, T value);
	template <typename T>
	TupleResults deleteTuples(string attr, int op, T value);
	string strName();
	bool isDelete();
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
TupleResults TableManager::SelWithIndex(string attr, int op, T value)
{
	TupleResults results;
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
}

template <typename T>
TupleResults TableManager::SelWithoutIndex(string attr, int op, T value)
{
	FilePosition fPos;
	fPos.fileName = strName() + TupleFilePostfix;
	TupleResults results;
	int attrIndex = getAttrIndex(attr);
	for (int i = 0; i < tupleCount; i++)
	{
		fPos.blockNo = baseIndex + i;
		TupleManager tuple(strName(), fPos);
		if (tuple.isDelete() == false)
		{
			if (tuple.satisfy(attrIndex, op, value))
				results.push_back(fPos);		
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
		TupleManager tuple(strName(), results[i]);
		tuple.Delete();
	}
	//需要告知b+树
	writeTableBlock();
	return results;
}

