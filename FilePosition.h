#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class FilePosition
{
public:
	static const int FILENAMELENGTH = 32; // max length of file name
	char fileName[FILENAMELENGTH];		
	int blockNo;
	FilePosition();
	~FilePosition();
	string Hash() const
	{
		stringstream strNo;
		strNo << blockNo;
		return filePath() + strNo.str();
	}
	string filePath() const
	{
		string str = fileName;
		return str;
	}
	void setFilePath(string filePath)
	{
		strcpy_s(fileName,filePath.c_str());
	}
};

