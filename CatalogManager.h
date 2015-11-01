#pragma once
#include "TableManager.h"
#include <iostream>
#include <string>
#include "Tuple.h"
#include "SelectResults.h"
#include "DeleteResults.h"
#include "FilePosition.h"
#include <map>
#include "BufferPool.h"
#include "FilePosition.h"
using namespace std;

extern BufferPool buffer;
static const bool NEWTABLE = true;

class CatalogManager
{
private:
	const string CATALOGFILE = "catalogFile";
	int tableNum; //�������������Ѿ�ɾ���ġ�û��Ϊ0
	map<string, FilePosition> tableMap;
public:
	CatalogManager();
	~CatalogManager();
	TableManager getTable(string tabName);
	TableManager createTable(string tabName);
	bool deleteTable(string tabName);
	bool findTable(string tabName);
};

