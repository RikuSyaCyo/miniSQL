#pragma once
#include "BufferBlock.h"

class BufferPool
{
	static const int BLOCKPOOLSIZE = 1000;
	BufferBlock bufPool[BLOCKPOOLSIZE];
public:
	BufferPool();
	~BufferPool();
	void readBlock(void* cont, string fileName, int blockNo, int size); //cont:�������ݵ�ַ�� fileName:�ļ����ƣ� blockNo����ȡ�ڼ��飬 size:�������ݴ�С(��λ;Byte)
	void writeBlock(const void* cont, string fileName, int blockNo, int size); //cont:д�����ݵ�ַ�� fileName:�ļ����ƣ� blockNo����ȡ�ڼ��飬 size:�������ݴ�С(��λ;Byte)
};

