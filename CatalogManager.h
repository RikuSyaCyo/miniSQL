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
	int tableNum; //总数量，包括已经删除的。没有为0
	map<string, FilePosition> tableMap; //每一个table所在的文件位置
	map<string, string>	IndexToTableMap; //index name 对应的 table name
	map<string, string> IndexToAttributeMap; //index name 对应的 attribute name
	void writeBackTableNum();
public:
	friend class TableManager;
	CatalogManager();
	~CatalogManager();
	TableManager getTable(string tabName);//查找返回一个table
	TableManager createTable(string tabName);//新建一个table
	bool deleteTable(string tabName);//删除一个table
	bool findTable(string tabName);//判断table是否存在
	void deleteIndex(string indexName);
	void CreateIndexOn(string tableName,string attributeName,string indexName);
	bool hasIndex(string indexName);
};

