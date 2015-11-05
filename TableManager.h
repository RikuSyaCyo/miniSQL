#pragma once
#include <string>
#include <iostream>
#include "BufferPool.h"
#include "FilePosition.h"
#include "Attribute.h"
#include "TupleManager.h"
#include "ConstantTable.h"
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
public:
	//TableManager(){}
	TableManager(FilePosition fPos);
	TableManager(FilePosition fPos, string tabName, bool flag);
	~TableManager();
	void InsertAttribute(string attname, int type); //�������ԣ�int��float��
	void InsertAttribute(string attname, int type, int charLength);//�������ԣ�char��
	Attribute getAttri(int attrIndex);//�����±�ΪattrIndex������
	int getAttrOffset(int attrIndex);//�����±�0~attrIndex�����ԵĴ洢���ֽ���
	int getAttriCount();//���ص�ǰ���Ը���
	void CreateIndex(string attr);//����Index
	void DeleteIndex(string attr);//ɾ��index
	TupleManager CreateNewTuple();//����һ��tuple
	void Delete();//ɾ��table
	template <typename T>
	TupleResults selectTuples(string attr, int op, T value);//��ѯtuple
	template <typename T>
	TupleResults deleteTuples(string attr, int op, T value);//ɾ��tuple����
	string strName();//����table name
	bool isDelete();//table�Ƿ���ɾ��
	void disTableInf();//��ʾtable��Ϣ
	void disAllTuples();//��ʾ����tuple
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
TupleResults TableManager::SelWithIndex(string attr, int op, T value)
{
	TupleResults results;
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
}

template <typename T>
TupleResults TableManager::SelWithoutIndex(string attr, int op, T value)
{
	FilePosition fPos;
	fPos.setFilePath(strName() + TupleFilePostfix);
	TupleResults results;
	int attrIndex = getAttrIndex(attr);
	for (int i = 0; i < tupleCount; i++)
	{
		fPos.blockNo = baseIndex + i;
		TupleManager tuple(strName(), fPos);
		if (tuple.isDelete() == false)
		{
			if (tuple.satisfy(attrIndex, op, value))
				results.push_back(fPos);		
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
		TupleManager tuple(strName(), results[i]);
		tuple.Delete();
	}
	//��Ҫ��֪b+��
	writeTableBlock();
	return results;
}

