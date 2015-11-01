#include "TupleManager.h"

TupleManager::TupleManager(string tabName, FilePosition fPos)
{
	filePos = fPos;
	readTupleBlock();
}

TupleManager::TupleManager(string tabName, FilePosition fPos,bool Flag)
{
	strcpy(belongTable,tabName.c_str());
	filePos = fPos;
	delFlag = false;
	writeTupleBlock();
}


TupleManager::~TupleManager()
{

}

int TupleManager::getOffset(const TableManager& table, int attrIndex)
{
	int Bytes = 0;
	for (int i = 0; i < attrIndex; i++)
		Bytes += table.getAttri(i).Bytes();
	return Bytes;
}

bool TupleManager::isDelete()
{
	readTupleBlock();
	return delFlag;
}

void TupleManager::Delete()
{
	readTupleBlock();
	delFlag = true;
	writeTupleBlock();
}