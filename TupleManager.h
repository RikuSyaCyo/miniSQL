#pragma once
#include "FilePosition.h"
#include <vector>
static const int TUPLEMAXSIZE = 3072;
static const bool NEWTUPLE = true;

typedef vector<FilePosition> TupleResults;

class TupleManager
{
private:
	static const int TABLENAMELENGTH = 128;
	char belongTable[TABLENAMELENGTH];
	char data[TUPLEMAXSIZE];
	int tupleSize;
	bool delFlag;
	FilePosition filePos;
public:
	TupleManager(FilePosition fPos);
	TupleManager(FilePosition fPos,bool Flag);
	~TupleManager();
	void InsTupleValue();
	bool isDelete();
	bool satisfy(int attrNo,int op, int value);
	void Delete();
};

