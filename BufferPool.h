#pragma once
#include "BufferBlock.h"
#include <map>
#include <vector>
#include "stdio.h"
using namespace std;

class BufferPool 
{
private:	
	static const int BLOCKPOOLSIZE = 1000; // block pool size 缓冲区块数
	static const int NO_FREE_BLOCK = -1; 
	//int counter; //时间戳
	BufferBlock bufPool[BLOCKPOOLSIZE]; //缓冲池
	//long bufLastVisit[BLOCKPOOLSIZE]; //缓冲块最后访问的时间戳
	map<string, int> blockMap;//记录文件block是否在缓冲区中，文件block(FilePosition)对应的缓冲区block(int)
	vector<int> freeBlockVec;//记录所有free block的index
	vector<int> visitRecordVec;//缓冲块使用记录<队列>
	int newBufBlock(); //申请一个缓冲块
	int findFreeBlock();  //修改实现方式，使用free_vector
	int findLRUBlock(); //修改实现方式，使用hash
public:
	BufferPool();
	~BufferPool();
	void readFileBlock(void* cont, const string& filePath, const int blockNo,const int size); //读入文件块。 cont:读入数据地址， fileName:文件名称， blockNo：读取第几块， size:读入数据大小(单位;Byte)
	void writeFileBlock(const void* cont, const string& filePath, const int blockNo,const int size); //写入文件块。 cont:写入数据地址， fileName:文件名称， blockNo：写进第几块， size:写入数据大小(单位;Byte)
	void flushBufferPool(); //清空缓冲区，数据写入硬盘
	void deleteFile(string fileName);
};

