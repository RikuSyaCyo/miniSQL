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
	//cout << "~tablemanager" << endl;
}

void TableManager::readTableBlock()
{
	//cout << sizeof(*this) << endl;
	//TableManager table;
	//buffer.readFileBlock(&table, filePos.filePath(), filePos.blockNo, sizeof(table));
	//*this = table;
	buffer.readFileBlock(this, filePos.filePath(), filePos.blockNo, sizeof(*this));
}

void TableManager::writeTableBlock()
{
	buffer.writeFileBlock(this, filePos.filePath(), filePos.blockNo, sizeof(*this));
}

void TableManager::InsertAttribute(string attname, int type, int charLength, int uniqueInf)
{
	readTableBlock();
	Attribute& attri = attr[attriArrayTop++];
	strcpy_s(attri.name, attname.c_str());
	attri.type = type;
	attri.strLength = charLength+1;
	attri.hasIndex = false;
	switch (uniqueInf)
	{
	case 0:
		attri.primary = false;
		attri.unique = true;
		break;
	case 1:
		attri.primary = false;
		attri.unique = false;
		break;
	case 2:
		attri.primary = true;
		attri.unique = true;
		break;
	default:
		break;
	}
	writeTableBlock();
}

void TableManager::InsertAttribute(string attname, int type,int uniqueInf)
{
	InsertAttribute(attname,type,0);
}

Attribute TableManager::getAttri(int attrIndex) 
{
	readTableBlock();
	return attr[attrIndex];
}

int TableManager::getAttrOffset(int attrIndex)
{
	readTableBlock();
	int offset = 0;
	for (int i = 0; i <= attrIndex; i++)
		offset += attr[i].Bytes();
	return offset;
}

int TableManager::getAttriCount()
{
	readTableBlock();
	return attriArrayTop;
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
	fPos.setFilePath(strName() + TupleFilePostfix);
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

void TableManager::setName(string tabName)
{
	strcpy_s(tableName, tabName.c_str());
}

void TableManager::disTableInf()
{
	readTableBlock();
	cout << "Table:" << endl;
	cout << strName() << endl;
	for (int i = 0; i < attriArrayTop; i++)
	{
		string str=attr[i].name;
		cout << str << '\t';
	}
	cout << endl;
}

void TableManager::disAllTuples()
{
	readTableBlock();
	//cout << "disAllTuples " << tupleCount << endl;
	FilePosition fPos;
	fPos.setFilePath(strName() + TupleFilePostfix);
	for (int i = 0; i < tupleCount; i++)
	{
		fPos.blockNo = baseIndex + i;
		TupleManager tuple(strName(), fPos);
		if (tuple.isDelete() == false)
		{
			tuple.display();
		}
	}
}

int TableManager::attributeCount()
{
	return attriArrayTop;
}

int TableManager::getAttributeType(int attriIndex)
{
	return attr[attriIndex].type;
}

string TableManager::getAttriName(int attriIndex)
{
	string str = attr[attriIndex].name;
	return str;
}

TupleResults TableManager::getAllTuples()
{
	TupleResults results(strName());
	for (int i = 0; i < tupleCount; i++)
	{
		TupleManager tuple(strName(),i);
		if (tuple.isDelete() == false)
			results.insert(i);
	}
	return results;
}