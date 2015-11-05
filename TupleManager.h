#pragma once
#include <string>
#include "FilePosition.h"
#include "ConstantTable.h"
#include "BufferPool.h"

using namespace std;

class CatalogManager;
class TableManager;
extern CatalogManager catalog;
extern BufferPool buffer;

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
	string getValue(int attrIndex,const string& label);
	template<typename T>
	T getValue(int attrIndex,const T& label);

public:
	TupleManager(string tabName,FilePosition fPos);
	TupleManager(string tabName,FilePosition fPos,bool Flag);
	~TupleManager();
	void InsValue(int attrIndex, string value);
	template<typename T>
	void InsValue(int attrIndex, T value);
	template <typename T>
	bool satisfy(int attrNo, int op, T opValue);
	bool isDelete();
	void Delete();
	void display();
};



template<typename T>
void TupleManager::InsValue(int attrIndex, T value)
{
	readTupleBlock();
	string tabName = belongTable;
	TableManager table=catalog.getTable(tabName);
	int offset = table.getAttrOffset(attrIndex - 1);
	void* ptr = data + offset;
	*((T*)ptr) = value;
	writeTupleBlock();
}

template<typename T>
T TupleManager::getValue(int attrIndex, const T& label)
{
	cout << "getValue" << endl;
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	int offset = table.getAttrOffset(attrIndex - 1);
	void* ptr = data + offset;
	T tupleValue = *((T*)ptr);
	return tupleValue;
}

template <typename T>
bool satisfy(int attrIndex, int op, T opValue)
{
	readTupleBlock();
	T tupleValue = getValue(attrIndex,opValue);
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
	//writeTupleBlock();
}