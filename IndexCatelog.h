#pragma once
#include"BufferPool.h"
#include<string>
using namespace std;

class IndexCatelog
{
public:
	int root;
	int size; //���� 
	int KeyType; //��������
	int index; //��¼д����һ����

	IndexCatelog();
	~IndexCatelog();
	IndexCatelog(int size_set, int KeyType_set);
	int nextIndex();
	void LoadIndexCatelog(string filename);
	void SaveIndexCatelog(string filename);
};

