#include "CatalogManager.h"
#include <string>
#include <fstream>
using namespace std;

CatalogManager::CatalogManager()
{
	FilePosition fPos;
	fPos.setFilePath(CATALOGFILE);
	fstream _file;
	_file.open(fPos.filePath(),ios::in);
	if (!_file) ///文件不存在
	{
		tableNum = 0;
		writeBackTableNum();
	}
	else
	{ 
		buffer.readFileBlock(&tableNum, CATALOGFILE, baseIndex, sizeof(tableNum));
		for (int i = 0; i < tableNum; i++)
		{
			fPos.blockNo = baseIndex + i + 1;
			TableManager table(fPos);
			if (table.isDelete() == false)
			{
				tableMap[table.strName()] = fPos;
				for (int j = 0; j < table.attriArrayTop;j++)
					if (table.attr[j].hasIndex)
					{
						string indexName = table.attr[j].indexName;
						IndexToTableMap[indexName] = table.strName();
						IndexToAttributeMap[indexName] = table.getAttriName(j);
					}
			}
		}
	}

}

CatalogManager::~CatalogManager()
{
	//cout << "~catalogManager" << endl;
}

void CatalogManager::writeBackTableNum()
{
	buffer.writeFileBlock(&tableNum, CATALOGFILE, baseIndex, sizeof(tableNum));
}

TableManager CatalogManager::getTable(string tabName)
{
	FilePosition fPos = tableMap[tabName];
	TableManager table(fPos);
	return table;
}

TableManager CatalogManager::createTable(string tabName)
{
	FilePosition fPos;
	tableNum++;
	writeBackTableNum();
	fPos.setFilePath(CATALOGFILE);
	fPos.blockNo = baseIndex + tableNum;
	TableManager table(fPos,tabName,NEWTABLE);
	//TableManager table;
	tableMap[tabName] = fPos;
	return table;
}

bool CatalogManager::deleteTable(string tabName)
{
	if (findTable(tabName) == false) return false;
	FilePosition fPos = tableMap[tabName];
	TableManager table(fPos);
	table.Delete();
	tableMap.erase(tabName);
	return true;
}

bool CatalogManager::findTable(string strName)
{
	if (tableMap.count(strName) > 0)
		return true;
	else
		return false;
}

void CatalogManager::CreateIndexOn(string tableName, string attributeName, string indexName)
{
	TableManager table = getTable(tableName);
	table.CreateIndex(attributeName,indexName);
	//IndexToTableMap[indexName] = tableName;
	//IndexToAttributeMap[indexName] = attributeName;
}

void CatalogManager::deleteIndex(string indexName)
{
	string tableName = IndexToTableMap[indexName];
	string attributeName = IndexToAttributeMap[indexName];
	TableManager table = catalog.getTable(tableName);
	table.DeleteIndex(attributeName);
}

bool CatalogManager::hasIndex(string indexName)
{
	if (IndexToTableMap.count(indexName) > 0)
		return true;
	else
		return false;
}