#include "BufferPool.h"
#include <string>


BufferPool::BufferPool() 
{
	for (int i = 0; i < BLOCKPOOLSIZE; i++)
	{
		freeBlockVec.push_back(i);
	}
}

BufferPool::~BufferPool()
{
	flushBufferPool();
}

void BufferPool::readFileBlock(void* cont, const string& fileName, const int blockNo, const int size)
{
	FilePosition fPos;
	int bufBlockIndex;
	fPos.setFilePath(fileName);
	fPos.blockNo = blockNo;
	if (blockMap.count(fPos.Hash()) > 0)
	{
		bufBlockIndex = blockMap[fPos.Hash()];
	}
	else
	{
		bufBlockIndex=newBufBlock();
		bufPool[bufBlockIndex].readFile(fPos);
		blockMap[fPos.Hash()] = bufBlockIndex;
		freeBlockVec.erase(find(freeBlockVec.begin(), freeBlockVec.end(), bufBlockIndex));
	}
	bufPool[bufBlockIndex].getData(cont, size);
	if (find(visitRecordVec.begin(), visitRecordVec.end(), bufBlockIndex) != visitRecordVec.end())
	{
		visitRecordVec.erase(find(visitRecordVec.begin(), visitRecordVec.end(), bufBlockIndex));
	}
	visitRecordVec.push_back(bufBlockIndex);
}

void BufferPool::writeFileBlock(const void* cont, const string& fileName, const int blockNo, const int size)
{
	FilePosition fPos;
	int bufBlockIndex;
	fPos.setFilePath(fileName);
	fPos.blockNo = blockNo;
	if (blockMap.count(fPos.Hash()) > 0)
	{
		bufBlockIndex = blockMap[fPos.Hash()];
	}
	else
	{
		bufBlockIndex = newBufBlock();
		freeBlockVec.erase(find(freeBlockVec.begin(), freeBlockVec.end(), bufBlockIndex));
	}
	bufPool[bufBlockIndex].setData(cont, fPos, size);
	blockMap[fPos.Hash()] = bufBlockIndex;	
	if (find(visitRecordVec.begin(), visitRecordVec.end(), bufBlockIndex) != visitRecordVec.end())
	{
		visitRecordVec.erase(find(visitRecordVec.begin(), visitRecordVec.end(), bufBlockIndex));
	}
	visitRecordVec.push_back(bufBlockIndex);
}

int BufferPool::newBufBlock()
{
	int bufBlockIndex;
	if ((bufBlockIndex = findFreeBlock()) == NO_FREE_BLOCK)
	{
		bufBlockIndex = findLRUBlock(); 
		blockMap.erase(bufPool[bufBlockIndex].getFilePos().Hash());		
		bufPool[bufBlockIndex].flush();
		freeBlockVec.push_back(bufBlockIndex);
	}
	return bufBlockIndex;
}

int BufferPool::findFreeBlock()
{
	if (freeBlockVec.size() == 0)
		return NO_FREE_BLOCK;
	else
		return freeBlockVec[0];
}

int BufferPool::findLRUBlock()
{
	int LRUIndex = visitRecordVec[0];
	visitRecordVec.erase(visitRecordVec.begin());
	return LRUIndex;
}

void BufferPool::flushBufferPool()
{
	for (int i = 0; i < BLOCKPOOLSIZE;i++)
		if (bufPool[i].isFree()==false)
		{
			blockMap.erase(bufPool[i].getFilePos().Hash());
			bufPool[i].flush();
			freeBlockVec.push_back(i);
		}
}

void BufferPool::deleteFile(string fileName)
{
	for (int i = 0; i < BLOCKPOOLSIZE; i++)
	{
		if (bufPool[i].isFree() == false)
			if (bufPool[i].diskFile() == fileName)
			{
				bufPool[i].flush();
				freeBlockVec.push_back(i);
			}
	}
	remove(fileName.c_str());	
}