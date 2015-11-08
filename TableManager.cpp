#include "TableManager.h"
#include "CatalogManager.h"

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
		_CreateIndex(attname,attname+"PRIMARYINDEX");
		attri.unique = true;
		break;
	default:
		break;
	}
	writeTableBlock();
}

void TableManager::InsertAttribute(string attname, int type,int uniqueInf)
{
	InsertAttribute(attname,type,0,uniqueInf);
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

void TableManager::_CreateIndex(string attrName,string indexName)
{
	int attrIndex = getAttrIndex(attrName);
	Attribute& attribute = attr[attrIndex];
	string fileName = strName() + attrName + IndexFilePostfix;
	switch (attribute.type)
	{
	case INT:
		createIndex(120, INT_BP, fileName);
		break;
	case FLOAT:
		createIndex(120, FLOAT_BP, fileName);
		break;
	case CHAR:
		createIndex(4, attribute.strLength - 1, fileName);
		break;
	default:
		break;
	}
	TupleResults tuplesRes = getAllTuples();
	string insertValue;
	for (int i = 0; i < tuplesRes.size(); i++)
	{
		TupleManager tuple(strName(),tuplesRes.getTupleIndexAt(i));
		switch (attribute.type)
		{
		case INT:
			insertIndex(fileName, tuple.getValue(attrIndex,1),tuple.filePos.blockNo-baseIndex);
			break;
		case FLOAT:
			insertIndex(fileName, tuple.getValue(attrIndex, (float)1.1), tuple.filePos.blockNo-baseIndex);
			break;
		case CHAR:
			insertValue = extendString(tuple.getValue(attrIndex, "abc"),attribute.strLength - 1);
			insertIndex(fileName,insertValue ,tuple.filePos.blockNo-baseIndex);
			break;
		default:
			break;
		}
	}
	strcpy_s(attribute.indexName,indexName.c_str());	
	catalog.IndexToTableMap[indexName] = strName();
	catalog.IndexToAttributeMap[indexName] = attrName;
	attribute.hasIndex = true;
	writeTableBlock();
}

void TableManager::CreateIndex(string attrName,string indexName)
{
	readTableBlock();
	_CreateIndex(attrName,indexName);
	writeTableBlock();
}

void TableManager::_DeleteIndex(string attrName)
{
	int attrIndex = getAttrIndex(attrName);
	Attribute& attribute = attr[attrIndex];
	string fileName = strName() + attrName + IndexFilePostfix;
	dropIndex(fileName);
	string indexName = attribute.indexName;
	catalog.IndexToTableMap.erase(indexName);
	catalog.IndexToAttributeMap.erase(indexName);
	attribute.hasIndex = false;
	writeTableBlock();
}

void TableManager::DeleteIndex(string attrName)
{
	readTableBlock();
	_DeleteIndex(attrName);
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
	for (int i = 0; i < attriArrayTop;i++)
		if (hasIndexOn(i))
		{
			_DeleteIndex(getAttriName(i));
		}
	delFlag = true;	
	writeTableBlock();
}

bool TableManager::hasIndexOn(int attrIndex)
{
	return attr[attrIndex].hasIndex;
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

bool TableManager::isUnique(int attrIndex)
{
	return attr[attrIndex].unique;
}

TupleResults TableManager::selectTuples(string attrName, int op, string value)
{
	readTableBlock();
	if (hasIndexOn(attrName))
	{
		int attrIndex = getAttrIndex(attrName);
		string selValue = extendString(value, attr[attrIndex].strLength - 1);
		return SelWithIndex(attrName, op, selValue);
	}
	else{
		return SelWithoutIndex(attrName, op, value);
	}
}


//TupleResults TableManager::SelWithIndex(string attr, int op, int value)
//{
//	string fileName = strName() + attr + IndexFilePostfix;
//	TupleResults results(strName());
//	int k;
//	switch (op)
//	{
//	case EQUAL:
//		k = searchIndex(fileName, value);
//		results.insert(k);
//		break;
//	case NOEQUAL:
//		return SelWithoutIndex(attr, op, value);
//		break;
//	case LESSTHAN:
//		results.Set(searchLessThan(fileName, value, 0));
//		break;
//	case GREATERTHAN:
//		results.Set(searchBiggerThan(fileName, value, 0));
//		break;
//	case NOGREATERTHAN:
//		results.Set(searchLessThan(fileName, value, 1));
//		break;
//	case NOLESSTHAN:
//		results.Set(searchBiggerThan(fileName, value, 1));
//		break;
//	default:
//		break;
//	}
//	return results;
//}
//
//TupleResults TableManager::SelWithIndex(string attr, int op, float value)
//{
//	string fileName = strName() + attr + IndexFilePostfix;
//	TupleResults results(strName());
//	int k;
//	switch (op)
//	{
//	case EQUAL:
//		k = searchIndex(fileName, value);
//		results.insert(k);
//		break;
//	case NOEQUAL:
//		return SelWithoutIndex(attr, op, value);
//		break;
//	case LESSTHAN:
//		results.Set(searchLessThan(fileName, value, 0));
//		break;
//	case GREATERTHAN:
//		results.Set(searchBiggerThan(fileName, value, 0));
//		break;
//	case NOGREATERTHAN:
//		results.Set(searchLessThan(fileName, value, 1));
//		break;
//	case NOLESSTHAN:
//		results.Set(searchBiggerThan(fileName, value, 1));
//		break;
//	default:
//		break;
//	}
//	return results;
//}
//
////TupleResults TableManager::SelWithIndex(string attr, int op, string value)
////{
////	TupleResults results(strName());
////	return results;
////}
//
//TupleResults TableManager::SelWithIndex(string attr, int op, string value)
//{
//	string fileName = strName() + attr + IndexFilePostfix;
//	TupleResults results(strName());
//	int k;
//	switch (op)
//	{
//	case EQUAL:
//		k = searchIndex(fileName, value);
//		results.insert(k);
//		break;
//	case NOEQUAL:
//		return SelWithoutIndex(attr, op, value);
//		break;
//	case LESSTHAN:
//		results.Set(searchLessThan(fileName, value, 0));
//		break;
//	case GREATERTHAN:
//		results.Set(searchBiggerThan(fileName, value, 0));
//		break;
//	case NOGREATERTHAN:
//		results.Set(searchLessThan(fileName, value, 1));
//		break;
//	case NOLESSTHAN:
//		results.Set(searchBiggerThan(fileName, value, 1));
//		break;
//	default:
//		break;
//	}
//	return results;
//}