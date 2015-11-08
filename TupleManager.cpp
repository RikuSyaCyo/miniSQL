#include "TupleManager.h"
#include "CatalogManager.h"
#include "TableManager.h"

TupleManager::TupleManager(string tabName, int tupleIndex)
{
	filePos.setFilePath(tabName + TupleFilePostfix);
	filePos.blockNo = baseIndex+tupleIndex;
	readTupleBlock();
}

TupleManager::TupleManager(string tabName, FilePosition fPos)
{
	filePos = fPos;
	readTupleBlock();
}

TupleManager::TupleManager(string tabName, FilePosition fPos,bool Flag)
{
	strcpy_s(belongTable,tabName.c_str());
	filePos = fPos;
	attriNum = 0;
	delFlag = false;
	writeTupleBlock();
}

TupleManager::~TupleManager()
{

}

void TupleManager::readTupleBlock()
{
	buffer.readFileBlock(this,filePos.filePath(), filePos.blockNo, sizeof(*this));
}

void TupleManager::writeTupleBlock()
{
	buffer.writeFileBlock(this,filePos.filePath(), filePos.blockNo, sizeof(*this));
}

bool TupleManager::isDelete()
{
	readTupleBlock();
	return delFlag;
}

void TupleManager::Delete()
{
	readTupleBlock();
	string tableName = belongTable;
	TableManager table = catalog.getTable(tableName);
	delInfBTree(table.getAttriCount()-1);
	delFlag = true;
	writeTupleBlock();
}

void TupleManager::display()
{
	readTupleBlock();
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	for (int i = 0; i < attriNum; i++)
	{
		const Attribute& attri = table.getAttri(i);
		switch (attri.type)
		{
		case INT:
			cout << getValue(i, 1);
			break;
		case CHAR:
			cout << getValue(i, (string)"1");
			break;
		case FLOAT:
			cout << (float)getValue(i, (float)1.1);
			break;
		default:
			break;
		}
		cout << '\t';
	}		
	cout << endl;
}

int TupleManager::InsAttriValue(int attrIndex, string value)
{
	string tableName = belongTable;
	TableManager table = catalog.getTable(tableName);
	const Attribute& attr = table.getAttri(attrIndex);
	//int k = value.length();
	if (attr.strLength - 1 < value.length())
		return INSERT_FAIL_BEYONGDLENGTH;
	int offset = getOffset(attrIndex - 1);
	char* ptr = data + offset;
	strcpy_s((char*)ptr,value.length()+1, value.c_str());
	if (attrIndex+1>attriNum)
		attriNum = attrIndex + 1;
	string indexFileName = table.strName() + table.getAttriName(attrIndex) + IndexFilePostfix;
	if (table.hasIndexOn(attrIndex))
	{
		string insertValue=extendString(value,attr.strLength - 1);
		insertIndex(indexFileName, insertValue, filePos.blockNo-baseIndex);
	}
	return INSERT_SUCCEED;
}

int TupleManager::InsAttriValue(int attrIndex, const char* value)
{
	string strValue = value;
	return InsAttriValue(attrIndex, strValue);
}

int TupleManager::InsAttriValue(int attrIndex, int value)
{
	string tableName = belongTable;
	TableManager table = catalog.getTable(tableName);
	string indexFileName = table.strName() + table.getAttriName(attrIndex) + IndexFilePostfix;
	int offset = getOffset(attrIndex - 1);
	void* ptr = data + offset;
	*((int*)ptr) = value;
	if (table.hasIndexOn(attrIndex))
	{
		insertIndex(indexFileName, value, filePos.blockNo-baseIndex);
	}
	return INSERT_SUCCEED;
}

int TupleManager::InsAttriValue(int attrIndex, float value)
{
	string tableName = belongTable;
	TableManager table = catalog.getTable(tableName);
	string indexFileName = table.strName() + table.getAttriName(attrIndex) + IndexFilePostfix;
	int offset = getOffset(attrIndex - 1);
	void* ptr = data + offset;
	*((float*)ptr) = value;
	if (table.hasIndexOn(attrIndex))
	{
		insertIndex(indexFileName, value, filePos.blockNo-baseIndex);
	}
	return INSERT_SUCCEED;
}

int TupleManager::InsAttriValue(int attrIndex, double value)
{
	float f = (float)value;
	return InsAttriValue(attrIndex, f);
}

string TupleManager::getValue(int attrIndex, string label)
{
	int offset = getOffset(attrIndex - 1);
	void* ptr = data + offset;
	string tupleValue = (char*)ptr;
	return tupleValue;
}

int TupleManager::getValue(int attrIndex, int label)
{
	int offset = getOffset(attrIndex - 1);
	void* ptr = data + offset;
	int tupleValue = *((int*)ptr);
	return tupleValue;
}

float TupleManager::getValue(int attrIndex, float label)
{
	int offset = getOffset(attrIndex - 1);
	void* ptr = data + offset;
	float tupleValue = *((float*)ptr);
	return tupleValue;
}

bool TupleManager::satisfy(int attrIndex, int op, double opValue)
{
	float f = (float)opValue;
	return satisfy(attrIndex,op,f);
}

bool TupleManager::satisfy(int attrIndex, int op, const char* opValue)
{
	string strValue = opValue;
	return satisfy(attrIndex, op, strValue);
}

bool TupleManager::satisfy(int attrIndex, int op, string opValue)
{
	readTupleBlock();
	string tupleValue = getValue(attrIndex, opValue);
	switch (op)
	{
	case EQUAL:
		return tupleValue == opValue;
		break;
	case NOEQUAL:
		return tupleValue != opValue;
		break;
	case LESSTHAN:
		return tupleValue < opValue;
		break;
	case GREATERTHAN:
		return tupleValue > opValue;
		break;
	case NOGREATERTHAN:
		return tupleValue <= opValue;
		break;
	case NOLESSTHAN:
		return tupleValue >= opValue;
		break;
	default:
		return false;
		break;
	}
	//writeTupleBlock();
}

bool TupleManager::satisfy(int attrIndex, int op, int opValue)
{
	readTupleBlock();
	int tupleValue = getValue(attrIndex,opValue);
	switch (op)
	{
	case EQUAL:
		return tupleValue == opValue;
		break;
	case NOEQUAL:
		return tupleValue != opValue;
		break;
	case LESSTHAN:
		return tupleValue < opValue;
		break;
	case GREATERTHAN:
		return tupleValue > opValue;
		break;
	case NOGREATERTHAN:
		return tupleValue <= opValue;
		break;
	case NOLESSTHAN:
		return tupleValue >= opValue;
		break;
	default:
		return false;
		break;
	}
	return true;
	//writeTupleBlock();
}

bool TupleManager::satisfy(int attrIndex, int op, float opValue)
{
	readTupleBlock();
	float tupleValue = getValue(attrIndex,opValue);
	switch (op)
	{
	case EQUAL:
		return tupleValue == opValue;
		break;
	case NOEQUAL:
		return tupleValue != opValue;
		break;
	case LESSTHAN:
		return tupleValue < opValue;
		break;
	case GREATERTHAN:
		return tupleValue > opValue;
		break;
	case NOGREATERTHAN:
		return tupleValue <= opValue;
		break;
	case NOLESSTHAN:
		return tupleValue >= opValue;
		break;
	default:
		return false;
		break;
	}
	return true;
	//writeTupleBlock();
}

int TupleManager::getOffset(int attrIndex)
{
	string tabName = belongTable;
	TableManager table = catalog.getTable(tabName);
	int offset = table.getAttrOffset(attrIndex);
	return offset;
}

void TupleManager::delInfBTree(int attrIndex)
{
	string tableName = belongTable;
	TableManager table = catalog.getTable(tableName);
	for (int i = 0; i <=attrIndex; i++)
	{
		string indexFileName = table.strName() + table.getAttriName(i) + IndexFilePostfix;
		if (table.hasIndexOn(i))
		{
			string delValue;
			const Attribute& attribute = table.getAttri(i);
			switch (table.getAttributeType(i))
			{
			case INT:
				deleteIndex(indexFileName, getValue(i, 1));
				break;
			case FLOAT:
				deleteIndex(indexFileName, getValue(i, (float)1.1));
				break;
			case CHAR:				
				delValue = extendString(getValue(i, "1.1"), attribute.strLength - 1);
				deleteIndex(indexFileName,delValue);
				break;
			default:
				break;
			}
		}
	}
}