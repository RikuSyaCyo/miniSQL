#pragma once
#include "BufferBlock.h"
#include <map>
#include <ctime>

#define baseIndex=1; //�ļ���һ�����ݿ�ı��

class BufferPool 
{
private:	
	static const int BLOCKPOOLSIZE = 5;
	static const int NO_FREE_BLOCK = 5;
	BufferBlock bufPool[BLOCKPOOLSIZE];
	time_t bufLastVisit[BLOCKPOOLSIZE];
	map<FilePosition, int> blockMap;//��¼�ļ�block�Ƿ��ڻ������У��ļ�block(FilePosition)��Ӧ�Ļ�����block(int)
	int newBufBlock(); //����һ�������
	int findFreeBlock();
	int findLRUBlock();
public:
	BufferPool();
	~BufferPool();
	void readFileBlock(void* cont, const string& fileName, const int blockNo,const int size); //�����ļ��顣 cont:�������ݵ�ַ�� fileName:�ļ����ƣ� blockNo����ȡ�ڼ��飬 size:�������ݴ�С(��λ;Byte)
	void writeFileBlock(const void* cont, const string& fileName, const int blockNo,const int size); //д���ļ��顣 cont:д�����ݵ�ַ�� fileName:�ļ����ƣ� blockNo��д���ڼ��飬 size:д�����ݴ�С(��λ;Byte)
};

