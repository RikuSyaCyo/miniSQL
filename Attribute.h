#pragma once

const static int ATTRIBUTENAMELENGTH=128;
const static int INT = 0;
const static int CHAR = 1;
const static int FLOAT = 2;

class Attribute
{
public:
	char name[ATTRIBUTENAMELENGTH];
	//int nameLength;
	int type;
	int charLength;
	bool primary;
	bool unique;
	bool hasIndex;
};

