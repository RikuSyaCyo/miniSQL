#pragma once
#include "ConstantTable.h"

class Attribute
{
public:
	char name[ATTRIBUTENAMELENGTH];//attribute name
	int type;//int or float or string
	int strLength; //int,float��strlengthΪ0��>0Ϊchar�� ��char(n)����strLength=n+1
	bool primary;//�Ƿ�Ϊprimary key
	bool unique;//�Ƿ�unique
	bool hasIndex;//�������Ƿ���index
	char indexName[INDEXNAMELENGTH];
	int Bytes() const;//������ռ�ֽ���
};

