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

void TupleManager::readTupleBlock()
{
	buffer.readFileBlock(this, filePos.fileName, filePos.blockNo, sizeof(*this));
}

void TupleManager::writeTupleBlock()
{
	buffer.writeFileBlock(this, filePos.fileName, filePos.blockNo, sizeof(*this));
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

void TupleManager::display()
{
	readTupleBlock();
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	int attriCount = table.getAttriCount();
	for (int i = 0; i < attriCount; i++)
	{
		const Attribute& attri = table.getAttri(i);
		switch (attri.type)
		{
		case INT:
			cout << getValue(i, 1);
			break;
		case CHAR:
			cout << getValue(i, "1");
			break;
		case FLOAT:
			cout << getValue(i, 1.1);
			break;
		default:
			break;
		}
		cout << '\t';
	}		
	cout << endl;
}

void TupleManager::InsValue(int attrIndex, string value)
{
	readTupleBlock();
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	int offset = table.getAttrOffset(attrIndex - 1);
	char* ptr = data + offset;
	strcpy_s((char*)ptr,value.length()+1, value.c_str());
	writeTupleBlock();
}

string TupleManager::getValue(int attrIndex, const string& label)
{
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	int offset = table.getAttrOffset(attrIndex - 1);
	void* ptr = data + offset;
	string tupleValue = (char*)ptr;
	return tupleValue;
}