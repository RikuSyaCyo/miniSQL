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
	//cout << "~bufferPool" << endl;
	flushBufferPool();
}

void BufferPool::readFileBlock(void* cont, const string& fileName, const int blockNo, const int size)
{
	/*cout << "readFileBlock" << endl;
	cout << blockNo << " " << fileName << endl;*/
	FilePosition fPos;
	int bufBlockIndex;
	fPos.setFilePath(fileName);
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
		freeBlockVec.erase(find(freeBlockVec.begin(), freeBlockVec.end(), bufBlockIndex));
	}
	bufPool[bufBlockIndex].getData(cont, size);

	/*counter++;
	bufLastVisit[bufBlockIndex] = counter;*/
	visitRecordVec.push_back(bufBlockIndex);
}

void BufferPool::writeFileBlock(const void* cont, const string& fileName, const int blockNo, const int size)
{
	/*cout << "writeFileBlock" << endl;
	cout << blockNo << " " << fileName << endl;*/
	FilePosition fPos;
	int bufBlockIndex;
	fPos.setFilePath(fileName);
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
		freeBlockVec.erase(find(freeBlockVec.begin(), freeBlockVec.end(), bufBlockIndex));
		//cout << "Hash not find! bufBLockIndex: " << bufBlockIndex << endl;
	}
	bufPool[bufBlockIndex].setData(cont, fPos, size);
	blockMap[fPos.Hash()] = bufBlockIndex;	
	//counter++;
	//bufLastVisit[bufBlockIndex] = counter;
	visitRecordVec.push_back(bufBlockIndex);
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
	//time_t leastTime;
	//int LRUIndex;
	//leastTime = bufLastVisit[0];
	//LRUIndex = 0;
	//for (int i = 1; i < BLOCKPOOLSIZE;i++)
	//	if (bufLastVisit[i] < leastTime)
	//	{
	//		leastTime = bufLastVisit[i];
	//		LRUIndex = i;
	//		//cout << "***" << endl;
	//	}
	////cout << "time[0]: " << bufLastVisit[0] << endl;
	////cout << "time[1]: " << bufLastVisit[1] << endl;
	//return LRUIndex;
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
			/*cout << "____" << endl;
			cout << i << endl;
			cout << bufPool[i].getFilePos().fileName << endl;
			cout << bufPool[i].getFilePos().blockNo<<endl;*/
			bufPool[i].flush();
			freeBlockVec.push_back(i);
			/*cout << "___" << endl;*/
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