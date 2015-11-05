#include"BPtree.h"
#include"BufferPool.h"
#include"IndexCatelog.h"
#include<iostream>
#include<stdlib.h>
BufferPool buffer;
int a = 0;
int main()
{
	/*wating for test*/
	string filename = "file.txt";
	/*createIndex(4, -1, filename);
	for (int i = 0; i < 10; i++)
	{
		insertIndex(filename, i, i);
	}*/
	int result = searchIndex(filename, 9);
	cout << result << endl;
	int flag = 0;
	BP_show(filename, flag);
	system("pause");
}