#pragma once
#include "BufferBlock.h"
#include <map>
#include <ctime>

#define baseIndex=1; //文件第一个数据块的标号

class BufferPool 
{
private:	
	static const int BLOCKPOOLSIZE = 5;
	static const int NO_FREE_BLOCK = 5;
	BufferBlock bufPool[BLOCKPOOLSIZE];
	time_t bufLastVisit[BLOCKPOOLSIZE];
	map<FilePosition, int> blockMap;//记录文件block是否在缓冲区中，文件block(FilePosition)对应的缓冲区block(int)
	int newBufBlock(); //申请一个缓冲块
	int findFreeBlock();
	int findLRUBlock();
public:
	BufferPool();
	~BufferPool();
	void readFileBlock(void* cont, const string& fileName, const int blockNo,const int size); //读入文件块。 cont:读入数据地址， fileName:文件名称， blockNo：读取第几块， size:读入数据大小(单位;Byte)
	void writeFileBlock(const void* cont, const string& fileName, const int blockNo,const int size); //写入文件块。 cont:写入数据地址， fileName:文件名称， blockNo：写进第几块， size:写入数据大小(单位;Byte)
};

