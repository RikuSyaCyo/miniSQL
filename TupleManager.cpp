#include "TupleManager.h"
#include "CatalogManager.h"
#include "TableManager.h"

TupleManager::TupleManager(string tabName, FilePosition fPos)
{
	filePos = fPos;
	readTupleBlock();
}

TupleManager::TupleManager(string tabName, FilePosition fPos,bool Flag)
{
	strcpy_s(belongTable,tabName.c_str());
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

void TupleManager::readTupleBlock()
{
	buffer.readFileBlock(this, filePos.fileName, filePos.blockNo, sizeof(*this));
}

void TupleManager::writeTupleBlock()
{
	buffer.writeFileBlock(this, filePos.fileName, filePos.blockNo, sizeof(*this));
}