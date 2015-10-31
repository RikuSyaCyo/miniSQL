#pragma once
#include "BufferBlock.h"

class BufferPool
{
	static const int BLOCKPOOLSIZE = 1000;
	BufferBlock bufPool[BLOCKPOOLSIZE];
public:
	BufferPool();
	~BufferPool();
	void readBlock(void* cont, string fileName, int blockNo, int size); //cont:读入数据地址， fileName:文件名称， blockNo：读取第几块， size:读入数据大小(单位;Byte)
	void writeBlock(const void* cont, string fileName, int blockNo, int size); //cont:写入数据地址， fileName:文件名称， blockNo：读取第几块， size:读入数据大小(单位;Byte)
};

