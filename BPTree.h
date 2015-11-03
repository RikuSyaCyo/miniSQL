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
	void split(Node<KeyType> parent, string filename, IndexCatelog indexcatelog);
	void merge(Node<KeyType> parent, string filename, IndexCatelog indexcatelog);
};
/*****************define of class Node****************/
template<class KeyType>
inline Node<KeyType>::Node()
{
	size = 0;
	isLeaf = 1;
}

template<class KeyType>
inline Node<KeyType>::~Node()
{
}

template<class KeyType>
inline void Node<KeyType>::split(Node<KeyType> parent, string filename, IndexCatelog indexcatelog)
{
	if (parent.empty == 1)
	{
		parent.empty == 0;
		parent.ptr.push_back(index);
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
	bro.size = (size + 1) / 2;
	for (i = 0; i < bro.size; i++)
	{
		bro.keys[i] = keys[bro.size + i];
		bro.ptr[i] = ptr[bro.size + i];
	}
	bro.ptr[i] = ptr[bro.size + i];
	bro.isLeaf = isLeaf;
	bro.index = indexcatelog.nextIndex();

	//set self
	size -= bro.size;
	ptr[size] = bro.index;

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

//template<class KeyType>
inline void Node<KeyType>::merge(Node<KeyType> parent, string filename, IndexCatelog indexcatelog)
{
	KeyType flag;
	if (parent.size == 1)
	{
		indexcatelog.root = index;
	}
	else
	{
		int i = 0; //position
		for (i = 0; i < parent.size; i++)
			if (parent.keys[i]>keys[0]) break;
		if (i == parent.size)
		{
			Node<KeyType> bro = loadBPnode(filename, parent.ptr[i - 1], indexcatelog, flag);
			if (bro.size + size < indexcatelog.size)
			{
				bro.ptr.pop_back();
				int k = 0;
				for ( k = 0; k < size; k++)
				{
					bro.keys.push_back(keys[k]);
					bro.ptr.push_back(ptr[k]);
				}
				bro.ptr.push_back(ptr[k]);
				bro.size += size;
				parent.keys.erase(keys.begin() + i -1);
				parent.ptr.erase(ptr.begin() + i);
				parent.size -= 1;
			}
			else
			{
				int k = 0;
				for (k = 0; k < size; k++)
				{
					keys.push_back(bro.keys[k]);
				}
			}
		}
	}
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
template<class KeyType>
int insertIndex(string filename, IndexCatelog indexcatelog, KeyType key, int ptr);