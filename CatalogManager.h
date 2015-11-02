#pragma once
#include <map>
#include <iostream>
#include <string>
#include "BufferPool.h"
#include "FilePosition.h"
#include "TableManager.h"
#include "ConstantTable.h"
using namespace std;

extern BufferPool buffer;

class CatalogManager
{
private:
	const string CATALOGFILE = "catalogFile";
	int tableNum; //�������������Ѿ�ɾ���ġ�û��Ϊ0
	map<string, FilePosition> tableMap;
	void writeBackTableNum();
public:
	CatalogManager();
	~CatalogManager();
	TableManager getTable(string tabName);
	TableManager createTable(string tabName);
	bool deleteTable(string tabName);
	bool findTable(string tabName);
};

