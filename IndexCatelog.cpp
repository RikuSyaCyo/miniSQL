#include "IndexCatelog.h"
struct saveIndexCatelog
{
	int root;
	int size; //叉数 
	int KeyType; //数据类型
	int index; //记录写到哪一块了
};
extern BufferPool buffer;

IndexCatelog::IndexCatelog()
{
}

IndexCatelog::~IndexCatelog()
{
}

IndexCatelog::IndexCatelog(int size_set, int KeyType_set)
{
	size = size_set;
	KeyType = KeyType_set;
	index = 0;
	root = 1;
}

int IndexCatelog::nextIndex()
{
	index++;
	return index;
}

void IndexCatelog::LoadIndexCatelog(string filename)
{
	struct saveIndexCatelog IndexCatelog;
	buffer.readFileBlock(&IndexCatelog, filename, 0, sizeof(IndexCatelog));
	index = IndexCatelog.index;
	KeyType = IndexCatelog.KeyType;
	root = IndexCatelog.root;
	size = IndexCatelog.size;
}

void IndexCatelog::SaveIndexCatelog(string filename)
{
	struct saveIndexCatelog IndexCatelog;
	IndexCatelog.index = index;
	IndexCatelog.KeyType = KeyType;
	IndexCatelog.root = root;
	IndexCatelog.size = size;
	buffer.writeFileBlock(&IndexCatelog, filename, 0, sizeof(IndexCatelog));
}
