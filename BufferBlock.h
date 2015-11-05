#pragma once
#include <string>
#include <iostream>
#include "FilePosition.h"
#include "ConstantTable.h"
using namespace std;

class BufferBlock
{
private:
	static const int BLOCKSIZE = 4096;
	FilePosition  filePos;
	bool dirtyFlag;
	bool freeFlag;
	bool pinFlag; 
	char data[BLOCKSIZE];
public:
	BufferBlock();
	~BufferBlock();
	void getData(void* cont, const int size);
	void setData(const void* cont, FilePosition fPos, int size);
	void flush();
	void readFile(const FilePosition& fPos);
	void writeFile();
	bool isFree();
	FilePosition getFilePos();
	string diskFile();
};

