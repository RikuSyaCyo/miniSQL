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
	createIndex(6,-1, filename);
	srand((unsigned)time(NULL));
	int insert[20];
	for (int i = 0; i < 200000; i++)
	{
		insert[i] = rand() % 100;
	}
	//string insert[10] = { "aaaa","bbbb","cccc","dddd","eeee","ffff","gggg","hhhh","iiii","jjjj" };
    for (int i = 0; i <20; i++)
	{
	    insertIndex(filename, insert[i], insert[i]);
		cout << insert[i] << endl;
		int flag = 1;
		BP_show(filename, flag);
		system("pause");
	}
	//deleteIndex(filename, insert[9]);
	//deleteIndex(filename, insert[0]);
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);
	cout << indexcatelog.root << endl;

	//search
	/*int result = searchIndex(filename, insert[1]);
	cout << result << endl;

	vector<int> result_less;
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