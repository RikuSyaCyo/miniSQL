#include "BufferPool.h"
#include <string>


BufferPool::BufferPool() :counter(0)
{
	for (int i = 0; i < BLOCKPOOLSIZE; i++)
	{
		bufLastVisit[i] = counter;
	}
}


BufferPool::~BufferPool()
{
}

void BufferPool::readFileBlock(void* cont, const string& fileName, const int blockNo, const int size)
{
	/*cout << "readFileBlock" << endl;
	cout << blockNo << " " << fileName << endl;*/
	FilePosition fPos;
	int bufBlockIndex;
	fPos.fileName = fileName;
	fPos.blockNo = blockNo;
	if (blockMap.count(fPos.Hash()) > 0)
	{
		bufBlockIndex = blockMap[fPos.Hash()];
		//cout << "Hash find! bufBlockIndex:" << bufBlockIndex<<endl;
		//cout << "read bufferBlock find!" << endl;
		//cout << "Index: " << bufBlockIndex << endl;
		//cout <<"fileName"<<fPos.fileName << endl;
		//cout << "blockNo: " << blockNo << endl;
	}
	else
	{
		//cout << "read bufferBlock not find! " << endl;
		bufBlockIndex=newBufBlock();
		//cout << "bufBlockIndex: " << bufBlockIndex << endl;
		bufPool[bufBlockIndex].readFile(fPos);
		//cout << "Hash not find! bufBLockIndex: " << bufBlockIndex << endl;
		blockMap[fPos.Hash()] = bufBlockIndex;
	}
	bufPool[bufBlockIndex].getData(cont, size);
	counter++;
	bufLastVisit[bufBlockIndex] = counter;
}

void BufferPool::writeFileBlock(const void* cont, const string& fileName, const int blockNo, const int size)
{
	/*cout << "writeFileBlock" << endl;
	cout << blockNo << " " << fileName << endl;*/
	FilePosition fPos;
	int bufBlockIndex;
	fPos.fileName = fileName;
	fPos.blockNo = blockNo;
	if (blockMap.count(fPos.Hash()) > 0)
	{
		bufBlockIndex = blockMap[fPos.Hash()];
		//cout << "Hash find! bufBlockIndex:" << bufBlockIndex << endl;
	/*	cout << "write bufferBlock find!" << endl;
		cout << "Index: " << bufBlockIndex << endl;
		cout << "fileName" << fPos.fileName << endl;
		cout << "blockNo: " << blockNo << endl;*/
	}
	else
	{
		//cout << "write bufferBlock not find! " << endl;
		bufBlockIndex = newBufBlock();
		//cout << "Hash not find! bufBLockIndex: " << bufBlockIndex << endl;
	}
	bufPool[bufBlockIndex].setData(cont, fPos, size);
	blockMap[fPos.Hash()] = bufBlockIndex;
	counter++;
	bufLastVisit[bufBlockIndex] = counter;
}

int BufferPool::newBufBlock()
{
	//cout << "newBufBlock!" << endl;
	int bufBlockIndex;
	if ((bufBlockIndex = findFreeBlock()) == NO_FREE_BLOCK)
	{
		bufBlockIndex = findLRUBlock(); 
		//cout << "no free block! findBlockIndex: " <<bufBlockIndex<< endl;
		//cout << "findBlockIndex " <<bufBlockIndex<<endl;
		blockMap.erase(bufPool[bufBlockIndex].getFilePos().Hash());
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
			//cout << "***" << endl;
		}
	//cout << "time[0]: " << bufLastVisit[0] << endl;
	//cout << "time[1]: " << bufLastVisit[1] << endl;
	return LRUIndex;
}

void BufferPool::flushBufferPool()
{
	for (int i = 0; i < BLOCKPOOLSIZE;i++)
		if (!bufPool[i].isFree())
		{
			blockMap.erase(bufPool[i].getFilePos().Hash());
			/*cout << "____" << endl;
			cout << i << endl;
			cout << bufPool[i].getFilePos().fileName << endl;
			cout << bufPool[i].getFilePos().blockNo<<endl;*/
			bufPool[i].flush();
			/*cout << "___" << endl;*/
		}
}