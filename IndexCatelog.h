#pragma once
#include"BufferPool.h"
#include<string>
using namespace std;

class IndexCatelog
{
public:
	int root;
	int size; //叉数 
	int KeyType; //数据类型
	int index; //记录写到哪一块了

	IndexCatelog();
	~IndexCatelog();
	IndexCatelog(int size_set, int KeyType_set);
	int nextIndex();
	void LoadIndexCatelog(string filename);
	void SaveIndexCatelog(string filename);
};

