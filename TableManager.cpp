#include "TableManager.h"


TableManager::TableManager(FilePosition fPos)
{
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
	attri.charLength = charLength;
	attri.primary = false;
	attri.unique = false;
	attri.hasIndex = false;
	writeTableBlock();
}

void TableManager::InsertAttribute(string attname, int type)
{
	InsertAttribute(attname,type,0);
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
	TupleManager tuple(fPos,NEWTUPLE);
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

TupleResults TableManager::selectTuples(string attr, int op, int value)
{
	readTableBlock();
	if (hasIndexOn(attr))
	{
		return SelWithIndex(attr,op,value);
	}
	else{
		return SelWithoutIndex(attr,op,value);
	}
}

TupleResults TableManager::SelWithIndex(string attr, int op, int value)
{
	TupleResults results;
	switch (op)
	{
	case EQUAL:
		break;
	case NOEQUAL:
		break;
	case LESSTHAN:
		break;
	case GREATERTHAN:
		break;
	case NOGREATERTHAN:
		break;
	case NOLESSTHAN:
		break;
	default:
		break;
	}
}

TupleResults TableManager::SelWithoutIndex(string attr, int op, int value)
{
	FilePosition fPos;
	fPos.fileName = strName() + TupleFilePostfix;
	TupleResults results;
	int attrNo = getAttrIndex(attr);
	for (int i = 0; i < tupleCount;i++)
	{
		fPos.blockNo = baseIndex + i;
		TupleManager tuple(fPos);
		if (tuple.isDelete()==false)
		{
			if (tuple.satisfy(attrNo, op, value))
				results.push_back(fPos);
			/*switch (op)
			{
			case EQUAL:
				break;
			case NOEQUAL:
				break;
			case LESSTHAN:
				break;
			case GREATERTHAN:
				break;
			case NOGREATERTHAN:
				break;
			case NOLESSTHAN:
				break;
			default:
				break;
			}*/
		}		
	}
	return results;
}

TupleResults TableManager::deleteTuples(string attr, int op, int value)
{
	TupleResults results=selectTuples(attr,op,value);
	for (int i = 0; i < results.size(); i++)
	{
		TupleManager tuple(results[i]);
		tuple.Delete();
	}
	return results;
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

