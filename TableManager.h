#pragma once
#include <string>
#include <iostream>
#include "BufferPool.h"
#include "FilePosition.h"
#include "Attribute.h"
#include "TupleManager.h"
#include "ConstantTable.h"
#include "TupleResults.h"
using namespace std;

extern BufferPool buffer;

class TableManager
{
private:
	char  tableName[TABLENAMELENGTH]; //table name
	FilePosition filePos; //��table���ļ���block
	int attriArrayTop; //���Զ���βָ�롣û��Ϊ0
	Attribute attr[ATTRIMAXNUM];//����
	bool delFlag;//ɾ��flag
	int tupleCount;//tuple������û��Ϊ0

	void readTableBlock();//��ȡtable��Ϣ
	void writeTableBlock();//�洢table��Ϣ
	void setName(string tabName);//����table name
	bool hasIndexOn(string attr);//����attr���Ƿ���Index
	template <typename T>
	TupleResults SelWithIndex(string attr, int op, T value);//ͨ��Index����
	template <typename T>
	TupleResults SelWithoutIndex(string attr, int op, T value);//��ͨ��index����
	int getAttrIndex(string attr);//��������attr�����������е��±�
	Attribute getAttri(int attrIndex);//�����±�ΪattrIndex������
	int getAttrOffset(int attrIndex);//�����±�0~attrIndex�����ԵĴ洢���ֽ���
	int getAttriCount();//���ص�ǰ���Ը���
	void Delete();//ɾ��table

public:
	friend class CatalogManager;
	friend class TupleManager;

	TableManager(FilePosition fPos);
	TableManager(FilePosition fPos, string tabName, bool flag);
	~TableManager();
	void InsertAttribute(string attname, int type,int uniqueInf); //�������ԣ�int��float��
	void InsertAttribute(string attname, int type, int charLength,int uniqueInf);//�������ԣ�char��
	void CreateIndex(string attr);//����Index
	void DeleteIndex(string attr);//ɾ��index
	TupleManager CreateNewTuple();//����һ��tuple
	template <typename T>
	TupleResults selectTuples(string attr, int op, T value);//��ѯtuple
	template <typename T>
	TupleResults deleteTuples(string attr, int op, T value);//ɾ��tuple����
	TupleResults getAllTuples();//******************************
	string strName();//����table name
	bool isDelete();//table�Ƿ���ɾ��
	void disTableInf();//��ʾtable��Ϣ
	void disAllTuples();//��ʾ����tuple
	int attributeCount();
	int getAttributeType(int attriIndex);
	string getAttriName(int attriIndex);
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
	TupleResults results(strName());
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
	//��Ҫ��֪b+��
	writeTableBlock();
	return results;
}

