#pragma once
#include <string>
#include <iostream>
#include "BufferPool.h"
#include "FilePosition.h"
#include "Attribute.h"
#include "TupleManager.h"
#include "ConstantTable.h"
#include "TupleResults.h"
#include "BPTree.h"
#include "IndexCatelog.h"
#include <math.h>
using namespace std;

extern BufferPool buffer;
extern CatalogManager catalog;

class CatalogManager;

class TableManager
{
private:
	char  tableName[TABLENAMELENGTH]; //table name
	FilePosition filePos; //��table���ļ���block
	int attriArrayTop; //���Զ���βָ�롣û��Ϊ0
	Attribute attr[ATTRIMAXNUM];//����
	bool delFlag;//ɾ��flag
	int tupleCount;//tuple������û��Ϊ0

	TableManager(FilePosition fPos, string tabName, bool flag);
	void readTableBlock();//��ȡtable��Ϣ
	void writeTableBlock();//�洢table��Ϣ
	void setName(string tabName);//����table name
	bool hasIndexOn(int attrIndex);
	template <typename T>
	TupleResults SelWithIndex(string attr, int op, T value);//ͨ��Index����
	//TupleResults SelWithIndex(string attr, int op, int value);
	//TupleResults SelWithIndex(string attr, int op, float value);
	//TupleResults SelWithIndex(string attr, int op, string value);
	template <typename T>
	TupleResults SelWithoutIndex(string attr, int op, T value);//��ͨ��index����
	Attribute getAttri(int attrIndex);//�����±�ΪattrIndex������
	int getAttrOffset(int attrIndex);//�����±�0~attrIndex�����ԵĴ洢���ֽ���
	int getAttriCount();//���ص�ǰ���Ը���
	void Delete();//ɾ��table
	void _DeleteIndex(string attrName);
	void DeleteIndex(string attrName);//ɾ��index
	void _CreateIndex(string attrName,string indexName);
	void CreateIndex(string attrName, string indexName);//����Index 
public:
	friend class CatalogManager;
	friend class TupleManager;
	TableManager(FilePosition fPos);
	~TableManager();
	void InsertAttribute(string attname, int type,int uniqueInf); //�������ԣ�int��float��
	void InsertAttribute(string attname, int type, int charLength,int uniqueInf);//�������ԣ�char��
	TupleManager CreateNewTuple();//����һ��tuple
	template <typename T>
	TupleResults selectTuples(string attrName, int op, T value);//��ѯtuple
	TupleResults selectTuples(string attrName, int op, string value);
	template <typename T>
	TupleResults deleteTuples(string attr, int op, T value);//ɾ��tuple����
	TupleResults getAllTuples();//��������tuples
	string strName();//����table name
	bool isDelete();//table�Ƿ���ɾ��
	void disTableInf();//��ʾtable��Ϣ
	void disAllTuples();//��ʾ����tuple
	int attributeCount();
	int getAttributeType(int attriIndex);
	string getAttriName(int attriIndex);
	bool isUnique(int attrIndex);
	bool hasIndexOn(string attr);//����attr���Ƿ���Index
	int getAttrIndex(string attr);//��������attr�����������е��±�
};


template <typename T>
TupleResults TableManager::selectTuples(string attr, int op, T value)
{
	readTableBlock();
	if (hasIndexOn(attr))
	{
		return SelWithIndex(attr, op, value);
	}
	else{
		return SelWithoutIndex(attr, op, value);
	}
}

template <typename T>
TupleResults TableManager::SelWithIndex(string attr, int op, T value) //�������Ƶ�����ʹ���ַ����������˴�T�п���Ϊconst char*
{
	string fileName = strName() + attr + IndexFilePostfix;
	TupleResults results(strName());
	int n = 0;
	switch (op)
	{
	case EQUAL:
		n = searchIndex(fileName, value);
		if (n>=0)
			results.insert(n);
		break;
	case NOEQUAL:
		return SelWithoutIndex(attr,op,value);
		break;
	case LESSTHAN:
		results.Set(searchLessThan(fileName, value, 0));
		break;
	case GREATERTHAN:
		results.Set(searchBiggerThan(fileName, value, 0));
		break;
	case NOGREATERTHAN:
		results.Set(searchLessThan(fileName, value, 1));
		break;
	case NOLESSTHAN:
		results.Set(searchBiggerThan(fileName, value, 1));
		break;
	default:
		break;
	}
	return results;
}

template <typename T>
TupleResults TableManager::SelWithoutIndex(string attr, int op, T value)
{
	FilePosition fPos;
	fPos.setFilePath(strName() + TupleFilePostfix);
	TupleResults results(strName());
	int attrIndex = getAttrIndex(attr);
	for (int i = 0; i < tupleCount; i++)
	{
		fPos.blockNo = baseIndex + i;
		TupleManager tuple(strName(), fPos);
		if (tuple.isDelete() == false)
		{
			if (tuple.satisfy(attrIndex, op, value))
				results.insert(i);		
		}
	}
	return results;
}

template <typename T>
TupleResults TableManager::deleteTuples(string attr, int op, T value)
{
	TupleResults results = selectTuples(attr, op, value);
	for (int i = 0; i < results.size(); i++)
	{
		TupleManager tuple(strName(), results.getTupleIndexAt(i));
		tuple.Delete();
	}
	writeTableBlock();
	return results;
}

