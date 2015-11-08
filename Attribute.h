#pragma once
#include "ConstantTable.h"

class Attribute
{
public:
	char name[ATTRIBUTENAMELENGTH];//attribute name
	int type;//int or float or string
	int strLength; //int,float的strlength为0，>0为char。 若char(n)，则strLength=n+1
	bool primary;//是否为primary key
	bool unique;//是否unique
	bool hasIndex;//属性上是否有index
	char indexName[INDEXNAMELENGTH];
	int Bytes() const;//属性所占字节数
};

