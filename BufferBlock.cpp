#include "BufferBlock.h"
#include <fstream>
#include <iostream>
using namespace std;

BufferBlock::BufferBlock() :dirtyFlag(false), freeFlag(true), pinFlag(false)
{
}


BufferBlock::~BufferBlock()
{
}

void BufferBlock::getData(void* cont, const int size)
{
	memcpy(cont,data,size);
}

void BufferBlock::setData(const void* cont, FilePosition fPos, int size)
{
	memset(data,0,BLOCKSIZE);
	memcpy(data,cont,size);
	filePos = fPos;
	dirtyFlag = true;
	freeFlag = false;
}

FilePosition BufferBlock::getFilePos()
{
	return filePos;
}

bool BufferBlock::isFree()
{
	return freeFlag;
}

void BufferBlock::readFile(const FilePosition& fPos)
{
	ifstream infile(fPos.fileName,ios::binary);
	if (!infile)
	{
		cerr << "open error!" << endl;
		abort();
	}
	infile.seekg(BLOCKSIZE*(fPos.blockNo-baseIndex),ios::beg);
	infile.read(data,BLOCKSIZE);
	filePos = fPos;
	dirtyFlag = false;
	freeFlag = false;
	pinFlag = false;
	infile.close();
}

void BufferBlock::writeFile()
{
	ofstream outfile(filePos.fileName, ios::in|ios::out|ios::binary);
	if (!outfile)
	{
		outfile.open(filePos.fileName);
	}
	outfile.seekp(BLOCKSIZE*(filePos.blockNo - baseIndex), ios::beg);
	outfile.write(data, BLOCKSIZE);
	dirtyFlag = false;
	outfile.close();
}

void BufferBlock::flush()
{
	if (dirtyFlag == true)
	{
		writeFile();
		//cout << "abc" << endl;
	}
	filePos.fileName = "";
	dirtyFlag = false;
	freeFlag = true;
	pinFlag = false;	
}


