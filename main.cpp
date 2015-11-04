#include"BPtree.h"
#include"BufferPool.h"
#include"IndexCatelog.h"
#include<iostream>
BufferPool buffer;
int main()
{
	/*wating for test*/
	string filename = "file.txt";
	createIndex(4, -1, filename);
	for (int i = 0; i < 10; i++)
	{
		insertIndex(filename, i, i);
	}
	int flag=0;
	BP_show(filename, flag);
}