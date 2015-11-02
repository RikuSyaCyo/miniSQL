#pragma once
#include<iostream>
#include<vector>
#include"BufferPool.h"
#include"IndexCatelog.h"
using namespace std;

template<class KeyType>
class Node 
{
public:
	bool isLeaf;   //判断是否为叶节点
	int index;   //记录是哪一块
	int size;  //记录现在是第几个key
	int empty;  //空对象
	vector<KeyType> keys;  //索引键值
	vector<int> ptr;  //指针
	
	Node(); 
	~Node();
	void split(Node<KeyType> node, Node<KeyType> parent, string filename, IndexCatelog indexcatelog);
};
/*****************define of class Node****************/
template<class KeyType>
inline Node<KeyType>::Node()
{
	
}

template<class KeyType>
inline Node<KeyType>::~Node()
{
}

template<class KeyType>
inline void Node<KeyType>::split(Node<KeyType> node, Node<KeyType> parent, string filename, IndexCatelog indexcatelog)
{
	if (parent.empty == 1)
	{
		parent.empty == 0;
		parent.ptr.push_back(node.index);
		//parent.keys.push_back(node.keys[node.size / 2 - 1]);
		parent.ptr.push_back(-1);
		parent.isLeaf = 0;
		parent.size = 1;
		parent.index = indexcatelog.nextIndex();
		indexcatelog.root = parent.index;
	}
	
	//set brother node
	Node<KeyType> bro;
	int i = 0;
	bro.size = (node.size + 1) / 2;
	for (i = 0; i < bro.size; i++)
	{
		bro.keys[i] = node.keys[bro.size + i];
		bro.ptr[i] = node.ptr[bro.size + i];
	}
	bro.ptr[i] = node.ptr[bro.size + i];
	bro.isLeaf = node.isLeaf;
	bro.index = indexcatelog.nextIndex();

	//set self
	node.size -= bro.size;
	node.ptr[node.size] = bro.index;

	//set parent
	int j = 0;
	for (j = 0; j < parent.size; j++)
		if (parent.keys[j]>bro.keys[0]) break;
	parent.keys.insert(parent.keys.begin() + j, bro.keys[0]);
	parent.index.insert(parent.ptr.begin() + j + 1, bro.index);
	parent.size += 1;
	
	saveBPnode(filename, parent, indexcatelog);
	saveBPnode(filename, bro, indexcatelog);
}

/*********************************statement of BPtree*******************************************/
void saveBPnode(string filename, Node<int> node, IndexCatelog indexcatelog);
void saveBPnode(string filename, Node<float> node, IndexCatelog indexcatelog);
void saveBPnode(string filename, Node<string> node, IndexCatelog indexcatelog);
Node<int> loadBPnode(string FileName, int nodeindex, IndexCatelog indexcatelog, int flag);
Node<float> loadBPnode(string FileName, int nodeindex, IndexCatelog indexcatelog, float flag);
Node<string> loadBPnode(string FileName, int nodeindex, IndexCatelog indexcatelog, string flag);
void createIndex(int size, int KeyType, string FileName);
void dropIndex(string filename);
template<class KeyType>
int searchIndex(string filename, IndexCatelog indexcatelog, KeyType key);
template<class KeyType>
vector<int> searchLessThan(string filename, IndexCatelog indexcatelog, KeyType key, int closure);
template<class KeyType>
vector<int> searchBiggerThan(string filename, IndexCatelog indexcatelog, KeyType key, int closure);
