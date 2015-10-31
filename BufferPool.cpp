#include "BufferPool.h"


BufferPool::BufferPool()
{
	for (int i = 0; i < BLOCKPOOLSIZE; i++)
	{
		bufLastVisit[i] = time(NULL);
	}
}


BufferPool::~BufferPool()
{
}

void BufferPool::readFileBlock(void* cont, const string& fileName, const int blockNo, const int size)
{
	FilePosition fPos;
	int bufBlockIndex;
	fPos.fileName = fileName;
	fPos.blockNo = blockNo;
	if (blockMap.count(fPos) > 0)
	{
		bufBlockIndex = blockMap[fPos];
	}
	else
	{
		bufBlockIndex=newBufBlock();
		bufPool[bufBlockIndex].readFile(fPos);
		blockMap[fPos] = bufBlockIndex;
	}
	bufPool[bufBlockIndex].getData(cont, size);
	bufLastVisit[bufBlockIndex] = time(NULL);
}

void BufferPool::writeFileBlock(const void* cont, const string& fileName, const int blockNo, const int size)
{
	FilePosition fPos;
	int bufBlockIndex;
	fPos.fileName = fileName;
	fPos.blockNo = blockNo;
	if (blockMap.count(fPos) > 0)
	{
		bufBlockIndex = blockMap[fPos];
	}
	else
	{
		bufBlockIndex = newBufBlock();
	}
	bufPool[bufBlockIndex].setData(cont, fPos, size);
	blockMap[fPos] = bufBlockIndex;
	bufLastVisit[bufBlockIndex] = time(NULL);
}

int BufferPool::newBufBlock()
{
	int bufBlockIndex;
	if ((bufBlockIndex = findFreeBlock()) == NO_FREE_BLOCK)
	{
		bufBlockIndex = findLRUBlock();
		bufPool[bufBlockIndex].flush();
	}
	return bufBlockIndex;
}

int BufferPool::findFreeBlock()
{
	for (int i = 0; i < BLOCKPOOLSIZE; i++)
		if (bufPool[i].isFree())
			return i;
	return NO_FREE_BLOCK;
}

int BufferPool::findLRUBlock()
{
	time_t leastTime;
	int LRUIndex;
	leastTime = bufLastVisit[0];
	LRUIndex = 0;
	for (int i = 1; i < BLOCKPOOLSIZE;i++)
		if (bufLastVisit[i] < leastTime)
		{
			leastTime = bufLastVisit[i];
			LRUIndex = i;
		}
	return LRUIndex;
}