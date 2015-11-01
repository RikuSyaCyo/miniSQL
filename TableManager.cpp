#include "TableManager.h"


TableManager::TableManager(FilePosition fPos)
{
	filePos = fPos;
	readTableBlock();
}

TableManager::TableManager(FilePosition fPos, string tabName, bool flag)
{
	setName(tabName);
	filePos = fPos;
	attriArrayTop = 0;
	delFlag = false;
	tupleCount = 0;
	writeTableBlock();
}

TableManager::~TableManager()
{
}

void TableManager::InsertAttribute(string attname, int type, int charLength)
{
	readTableBlock();
	Attribute& attri = attr[attriArrayTop++];
	strcpy(attri.name, attname.c_str());
	attri.type = type;
	attri.strLength = charLength+1;
	attri.primary = false;
	attri.unique = false;
	attri.hasIndex = false;
	writeTableBlock();
}

void TableManager::InsertAttribute(string attname, int type)
{
	InsertAttribute(attname,type,0);
}

Attribute TableManager::getAttri(int attrIndex) const
{
	return attr[attrIndex];
}

void TableManager::CreateIndex(string attr)
{
	readTableBlock();
	writeTableBlock();
}

void TableManager::DeleteIndex(string attr)
{
	readTableBlock();
	writeTableBlock();
}

TupleManager TableManager::CreateNewTuple()
{
	readTableBlock();
	FilePosition fPos;
	tupleCount++;
	fPos.fileName = strName() + TupleFilePostfix;
	fPos.blockNo = baseIndex+tupleCount-1;
	TupleManager tuple(strName(),fPos,NEWTUPLE);
	writeTableBlock();
	return tuple;
}

void TableManager::Delete()
{
	readTableBlock();
	buffer.deleteFile(strName()+TupleFilePostfix);
	delFlag = true;	
	writeTableBlock();
}

bool TableManager::hasIndexOn(string attrName)
{
	readTableBlock();
	int attrIndex = getAttrIndex(attrName);
	return attr[attrIndex].hasIndex;
}

string TableManager::strName()
{
	string str = tableName;
	return str;
}

bool TableManager::isDelete()
{
	readTableBlock();
	return delFlag;
}

void TableManager::readTableBlock()
{
	buffer.readFileBlock(this,filePos.fileName,filePos.blockNo,sizeof(*this));
}

void TableManager::writeTableBlock()
{
	buffer.writeFileBlock(this,filePos.fileName,filePos.blockNo,sizeof(*this));
}

void TableManager::setName(string tabName)
{
	strcpy(tableName, tabName.c_str());
}

int  TableManager::getAttrIndex(string attrName)
{
	for (int i = 0; i < attriArrayTop; i++)
	{
		string str = attr[i].name;
		if (str == attrName)
			return i;
	}
}

