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
	map<string, FilePosition> tableMap; //ÿһ��table���ڵ��ļ�λ��
	map<string, string>	IndexToTableMap; //index name ��Ӧ�� table name
	map<string, string> IndexToAttributeMap; //index name ��Ӧ�� attribute name
	void writeBackTableNum();
public:
	friend class TableManager;
	CatalogManager();
	~CatalogManager();
	TableManager getTable(string tabName);//���ҷ���һ��table
	TableManager createTable(string tabName);//�½�һ��table
	bool deleteTable(string tabName);//ɾ��һ��table
	bool findTable(string tabName);//�ж�table�Ƿ����
	void deleteIndex(string indexName);
	void CreateIndexOn(string tableName,string attributeName,string indexName);
	bool hasIndex(string indexName);
};

