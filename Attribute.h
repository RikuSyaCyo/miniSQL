#pragma once
#include "ConstantTable.h"

class Attribute
{
public:
	const static int ATTRIBUTENAMELENGTH = 128;
	char name[ATTRIBUTENAMELENGTH];
	int type;
	int strLength; //int,float��strlengthΪ0��>0Ϊchar�� ��char(n)����strLength=n+1
	bool primary;
	bool unique;
	bool hasIndex;
	int Bytes() const;//������ռ�ֽ���
};

