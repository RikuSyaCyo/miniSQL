#include "BufferBlock.h"
#include <fstream>
#include <iostream>
using namespace std;

BufferBlock::BufferBlock() :dirtyFlag(false), freeFlag(true), pinFlag(false)
{
}


BufferBlock::~BufferBlock()
{
	//cout << "~BufferBlock" << endl;
}

void BufferBlock::getData(void* cont, const int size)
{
	memcpy(cont,data,size);
}

void BufferBlock::setData(const void* cont, FilePosition fPos, int size)
{
	pinFlag = true;
	memset(data,0,BLOCKSIZE);
	memcpy(data,cont,size);
	filePos = fPos;
	dirtyFlag = true;
	freeFlag = false;
	pinFlag = false;
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
	pinFlag = true;
	ifstream infile(fPos.filePath(),ios::binary);
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
	infile.close();
	pinFlag = false;
}

void BufferBlock::writeFile()
{
	while (pinFlag){
	}
	ofstream outfile(filePos.filePath(), ios::in|ios::out|ios::binary);
	if (!outfile)
	{
		outfile.open(filePos.filePath());
	}
	outfile.seekp(BLOCKSIZE*(filePos.blockNo - baseIndex), ios::beg);
	outfile.write(data, BLOCKSIZE);
	dirtyFlag = false;
	outfile.close();
}

void BufferBlock::flush()
{
	if (isFree() == true) return;
	if (dirtyFlag == true)
	{
		writeFile();
		//cout << "abc" << endl;
	}
	filePos.setFilePath("");
	dirtyFlag = false;
	freeFlag = true;
	pinFlag = false;	
}

string BufferBlock::diskFile()
{
	return filePos.filePath();
}

