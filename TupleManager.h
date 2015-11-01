#pragma once
#include "FilePosition.h"
#include "ConstantTable.h"
#include "CatalogManager.h"
#include "TableManager.h"
#include <string>
using namespace std;

extern CatalogManager catalog;

class TupleManager
{
private:
	static const int TUPLEMAXSIZE = 3072;
	char belongTable[TABLENAMELENGTH];
	FilePosition filePos;
	char data[TUPLEMAXSIZE];
//	int attriBytesTotal[ATTRIMAXNUM]; //前i个属性累计占字节数
//	int dataSize; //空的时候为0
	bool delFlag;

	void writeTupleBlock();
	void readTupleBlock();
	int getOffset(const TableManager& table, int attrIndex);
public:
	TupleManager(string tabName,FilePosition fPos);
	TupleManager(string tabName,FilePosition fPos,bool Flag);
	~TupleManager();
	template<typename T>
	void InsValue(int attrIndex, T value);
	template <typename T>
	bool satisfy(int attrNo, int op, T opValue);
	bool isDelete();
	void Delete();
};

template<typename T>
void TupleManager::InsValue(int attrIndex, T value)
{
	readTupleBlock();
	string tabName = belongTable;
	TableManager table=catalog.getTable(tabName);
	Attribute& attribute = table.attr[attrIndex];
	int offset = getOffset(table,attrIndex);
	if (attribute.type == CHAR)
		strcpy(data + offset, (string)value.c_str());
	else
		memcpy(data + offset, &T, sizeof(T));
	writeTupleBlock();
}

template <typename T>
bool satisfy(int attrIndex, int op, T opValue)
{
	T tupleValue
	readTupleBlock();
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	Attribute& attribute = table.attr[attrIndex];
	int offset = getOffset(table, attrIndex);
	if (attribute.type == CHAR)
		strcpy(&tupleValue,data + offset);
	else
		memcpy(&tupleValue,data + offset,sizeof(T));
	switch (op)
	{
	case EQUAL:
		return tupleValue == opValue;
		break;
	case NOEQUAL:
		return tupleValue != opValue;
		break;
	case LESSTHAN:
		return tupleValue < opValue;
		break;
	case GREATERTHAN:
		return tupleValue > opValue;
		break;
	case NOGREATERTHAN:
		return tupleValue <= opValue;
		break;
	case NOLESSTHAN:
		return tupleValue >= opValue;
		break;
	default:
		return false;
		break;
	}
	writeTupleBlock();
}