#pragma once
#include <string>
#include <iostream>
#include "Attribute.h"
#include "TupleManager.h"
#include "FilePosition.h"
#include "BufferPool.h"
using namespace std;

extern BufferPool buffer;
static const int EQUAL = 0;
static const int NOEQUAL = 1;
static const int LESSTHAN = 2;
static const int GREATERTHAN = 3;
static const int NOGREATERTHAN = 4;
static const int NOLESSTHAN = 5;
static const string TupleFilePostfix="DATA";

class TableManager
{
private:
	static const int TABLENAMELENGTH = 128;
	static const int ATTRIMAXNUM = 32;
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
	TupleResults SelWithIndex(string attr, int op, int value);
	TupleResults SelWithoutIndex(string attr, int op, int value);
	int getAttrIndex(string attr);
public:
	TableManager(FilePosition fPos);
	TableManager(FilePosition fPos, string tabName, bool flag);
	~TableManager();
	void InsertAttribute(string attname, int type);
	void InsertAttribute(string attname, int type, int charLength);
	void CreateIndex(string attr);
	void DeleteIndex(string attr);
	TupleManager CreateNewTuple();
	void Delete();
	TupleResults selectTuples(string attr, int op, int value);
	TupleResults deleteTuples(string attr, int op, int value);
	string strName();
	bool isDelete();
};

