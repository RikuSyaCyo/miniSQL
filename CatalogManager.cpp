#include "CatalogManager.h"
#include <string>
using namespace std;

CatalogManager::CatalogManager()
{
	FilePosition fPos;
	fPos.fileName = CATALOGFILE;
	buffer.readFileBlock(&tableNum, CATALOGFILE, baseIndex, sizeof(tableNum));
	for (int i = 0; i < tableNum; i++)
	{
		fPos.blockNo = baseIndex + i + 1;
		TableManager table(fPos);
		if (table.isDelete()== false)
		{
			tableMap[table.strName()] = fPos;
		}
	}
}


CatalogManager::~CatalogManager()
{
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
	buffer.writeFileBlock(&tableNum, CATALOGFILE, baseIndex, sizeof(tableNum));
	fPos.fileName = CATALOGFILE;
	fPos.blockNo = baseIndex + tableNum;
	TableManager table(fPos,tabName,NEWTABLE);
	tableMap[tabName] = fPos;
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