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
	int attriNum;//有值的属性个数，实际是 max(有值属性标号)+1
	bool delFlag;

	TupleManager(string tabName, FilePosition fPos, bool Flag);
	void writeTupleBlock();
	void readTupleBlock();
	string getValue(int attrIndex,string label);
	int getValue(int attrIndex, int label);
	float getValue(int attrIndex, float label);
	//template<typename T>
	//T getValue(int attrIndex, T label);
	bool satisfy(int attrIndex, int op, double opValue);
	bool satisfy(int attrIndex, int op, const char* opValue);
	bool satisfy(int attrIndex, int op, float opValue);
	bool satisfy(int attrIndex, int op, int opValue);
	bool satisfy(int attrIndex, int op, string opValue);
	int InsAttriValue(int attrIndex, string value);
	int InsAttriValue(int attrIndex, const char* value);
	int InsAttriValue(int attrIndex, int value);
	int InsAttriValue(int attrIndex, float value);
	int InsAttriValue(int attrIndex, double value);
	int getOffset(int attrIndex);
	void delInfBTree(int attrIndex);
public:
	friend class TableManager;
	TupleManager(string tabName, int tupleIndex);
	TupleManager(string tabName,FilePosition fPos);
	~TupleManager();
	template<typename T>
	int InsValue(int attrIndex, T value);
	/*template<typename T>
	void InsValue(int attrIndex, T value);*/
	//inline bool satisfy(int attrIndex, int op,const char* opValue);
	//template <typename T>
	//bool satisfy(int attrIndex, int op, T opValue);
	bool isDelete();
	void Delete();
	void display();
};

template<typename T>
int TupleManager::InsValue(int attrIndex, T value)
{
	int flag = INSERT_SUCCEED;
	readTupleBlock();
	string tableName = belongTable;
	TableManager table = catalog.getTable(tableName);
	if (table.isUnique(attrIndex) == true)
	{
		TupleResults results = table.selectTuples(table.getAttriName(attrIndex), EQUAL, value);
		if (results.size() > 0)
		{
			delFlag = true;
			delInfBTree(attrIndex-1);
			writeTupleBlock();
			return INSERT_FAIL_NOTUNIQUE;
		}
	}
	flag=InsAttriValue(attrIndex, value);
	if (flag != INSERT_SUCCEED)
	{
		delFlag = true;
		delInfBTree(attrIndex-1);
		writeTupleBlock();
		return flag;
	}
	if (attrIndex + 1>attriNum)
		attriNum = attrIndex + 1;
	writeTupleBlock();
	return flag;
}

//template<typename T>
//void TupleManager::InsValue(int attrIndex, T value)
//{
//	readTupleBlock();
//	int offset = getOffset(attrIndex - 1);
//	void* ptr = data + offset;
//	*((T*)ptr) = value;
//	if (attrIndex + 1>attriNum)
//		attriNum = attrIndex + 1;
//	writeTupleBlock();
//}

//template<typename T>
//T TupleManager::getValue(int attrIndex, T label)
//{
//	int offset = getOffset(attrIndex - 1);
//	cout << endl <<"offset: "<< offset << endl;
//	void* ptr = data + offset;
//	T tupleValue = *((T*)ptr);
//	return tupleValue;
//}

//template <typename T>
//bool satisfy(int attrIndex, int op, T opValue)
//{
//	readTupleBlock();
//	T tupleValue = getValue(attrIndex,opValue);
//	switch (op)
//	{
//	case EQUAL:
//		return tupleValue == opValue;
//		break;
//	case NOEQUAL:
//		return tupleValue != opValue;
//		break;
//	case LESSTHAN:
//		return tupleValue < opValue;
//		break;
//	case GREATERTHAN:
//		return tupleValue > opValue;
//		break;
//	case NOGREATERTHAN:
//		return tupleValue <= opValue;
//		break;
//	case NOLESSTHAN:
//		return tupleValue >= opValue;
//		break;
//	default:
//		return false;
//		break;
//	}
//	return true;
//	writeTupleBlock();
//}
//
