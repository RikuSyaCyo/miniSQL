#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class FilePosition
{
public:
	string fileName;
	int blockNo;
	FilePosition();
	~FilePosition();
	string Hash()
	{
		stringstream strNo;
		strNo << blockNo;
		return fileName + strNo.str();
	}
};

