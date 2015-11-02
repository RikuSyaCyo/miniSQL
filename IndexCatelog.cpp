#include "IndexCatelog.h"


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
