#pragma once
#include "BufferBlock.h"
#include <map>
#include <vector>
#include "stdio.h"
using namespace std;

class BufferPool 
{
private:	
	static const int BLOCKPOOLSIZE = 1000; // block pool size ����������
	static const int NO_FREE_BLOCK = -1; 
	//int counter; //ʱ���
	BufferBlock bufPool[BLOCKPOOLSIZE]; //�����
	//long bufLastVisit[BLOCKPOOLSIZE]; //����������ʵ�ʱ���
	map<string, int> blockMap;//��¼�ļ�block�Ƿ��ڻ������У��ļ�block(FilePosition)��Ӧ�Ļ�����block(int)
	vector<int> freeBlockVec;//��¼����free block��index
	vector<int> visitRecordVec;//�����ʹ�ü�¼<����>
	int newBufBlock(); //����һ�������
	int findFreeBlock();  //�޸�ʵ�ַ�ʽ��ʹ��free_vector
	int findLRUBlock(); //�޸�ʵ�ַ�ʽ��ʹ��hash
public:
	BufferPool();
	~BufferPool();
	void readFileBlock(void* cont, const string& filePath, const int blockNo,const int size); //�����ļ��顣 cont:�������ݵ�ַ�� fileName:�ļ����ƣ� blockNo����ȡ�ڼ��飬 size:�������ݴ�С(��λ;Byte)
	void writeFileBlock(const void* cont, const string& filePath, const int blockNo,const int size); //д���ļ��顣 cont:д�����ݵ�ַ�� fileName:�ļ����ƣ� blockNo��д���ڼ��飬 size:д�����ݴ�С(��λ;Byte)
	void flushBufferPool(); //��ջ�����������д��Ӳ��
	void deleteFile(string fileName);
};

