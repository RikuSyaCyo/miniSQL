#include"BPtree.h"
#include"BufferPool.h"
#include"IndexCatelog.h"
#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;
BufferPool buffer;
int a = 0;
int main()
{
	/*wating for tes*/

	string filename = "file.txt";
	createIndex(4,-1, filename);
	srand((unsigned)time(NULL));
	int insert[100];
	for (int i = 0; i < 100; i++)
	{
	    insert[i] = rand() % 1000;
		
	}
	cout << insert[0] << " ";
	cout << insert[1] << " ";
	cout << insert[2] << " ";
	cout << insert[3] << " ";
	cout << insert[4] << " ";
	cout << insert[5] << " ";
	cout << insert[6] << " ";
	//string insert[10] = { "aaaa","bbbb","cccc","dddd","eeee","ffff","gggg","hhhh","iiii","jjjj" };
    for (int i = 0; i <100; i++)
	{
	    insertIndex(filename, insert[i], insert[i]);
		//cout << insert[i] << endl;
	}
	int flag = 1;
	BP_show(filename, flag);
	system("pause");
	for (int i = 0; i < 100; i++)
	{
		deleteIndex(filename, insert[i]);
		//deleteIndex(filename, 593);
		BP_show(filename, flag);
		cout << endl;
	}
	//deleteIndex(filename, insert[9]);
	//deleteIndex(filename, insert[0]);
	BP_show(filename, flag);
	system("pause");

	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);
	cout << indexcatelog.root << endl;

	//search
	//int result = searchIndex(filename, 7378);
	//cout << result << endl;
	//system("pause");
	/*vector<int> result_less;
	result_less = searchLessThan(filename, insert[9], 1);
	for (int i = 0; i < result_less.size(); i++)
	{
		cout << result_less[i] << " ";
	}
	cout << endl;

	vector<int> result_bigger;
	result_bigger = searchBiggerThan(filename, insert[1], 1);
	for (int i = 0; i < result_bigger.size(); i++)
	{
		cout << result_bigger[i] << " ";
	}
	cout << endl;*/

}