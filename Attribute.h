#pragma once
#include "ConstantTable.h"

class Attribute
{
public:
	const static int ATTRIBUTENAMELENGTH = 128;
	char name[ATTRIBUTENAMELENGTH];
	int type;
	int strLength; //int,float的strlength为0，>0为char。 若char(n)，则strLength=n+1
	bool primary;
	bool unique;
	bool hasIndex;
	int Bytes() const {//属性所占字节数
		switch (type)
		{
		case INT:
			return sizeof(int);
			break;
		case FLOAT:
			return sizeof(float);
			break;
		case CHAR:
			return sizeof(char)*strLength;
			break;
		default:
			break;
		}
	}
};

