#pragma once
#include<iostream>
#include<vector>
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
	void split(Node<KeyType> parent, string filename, IndexCatelog &indexcatelog);
	void merge(Node<KeyType> parent, string filename, IndexCatelog &indexcatelog);
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
inline void Node<KeyType>::split(Node<KeyType> parent, string filename, IndexCatelog &indexcatelg)
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

template<class KeyType>
inline void Node<KeyType>::merge(Node<KeyType> parent, string filename, IndexCatelog &indexcatelog)
{
	KeyType flag;
	if (parent.size == 0)
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
			Node<KeyType> bro;
			loadBPnode(filename, bro, parent.ptr[i - 1], indexcatelog, flag);
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
				parent.keys.erase(parent.keys.begin() + i -1);
				parent.ptr.erase(parent.ptr.begin() + i);
				parent.size -= 1;
			}
			else
			{
				keys.insert(keys.begin(), bro.keys[bro.size - 1]);
				ptr.insert(ptr.begin(), bro.ptr[bro.size - 1]);
				bro.keys.erase(bro.keys.begin() + bro.size - 1);
				bro.ptr.erase(bro.ptr.begin() + bro.size - 1);
				size += 1;
				bro.size -= 1;
				parent.keys[i - 1] = bro.keys[0];
			}
			saveBPnode(filename, bro, indexcatelog);
		}
		else
		{
			Node<KeyType> bro;
			loadBPnode(filename, bro, parent.ptr[i + 1], indexcatelog, flag);
			if (bro.size + size < indexcatelog.size)
			{
				ptr.pop_back();
				int k = 0;
				for (k = 0; k < bro.size; k++)
				{
					keys.push_back(bro.keys[k]);
					ptr.push_back(bro.ptr[k]);
				}
				ptr.push_back(bro.ptr[k]);
				size += bro.size;
				parent.keys.erase(parent.keys.begin() + i);
				parent.ptr.erase(parent.ptr.begin() + i + 1);
				parent.size -= 1;
			}
			else
			{
				keys.push_back(bro.keys[0]);
				ptr.insert(ptr.begin() + size, bro.ptr[0]);
				bro.keys.erase(bro.keys.begin());
				bro.ptr.erase(bro.ptr.begin());
				size += 1;
				bro.size -= 1;
				parent.keys[i] = bro.keys[0];
			}
			saveBPnode(filename, bro, indexcatelog);
		}
		saveBPnode(filename, parent, indexcatelog);
	}
}

/*********************************statement of BPtree*******************************************/
void createIndex(int size, int KeyType, string FileName);
void dropIndex(string filename);
template<class KeyType>
int searchIndex(string filename, KeyType key);
template<class KeyType>
vector<int> searchLessThan(string filename, KeyType key, int closure);
template<class KeyType>
vector<int> searchBiggerThan(string filename, KeyType key, int closure);
template<class KeyType>
int insertIndex(string filename, KeyType key, int ptr);
template<class KeyType>
int deleteIndex(string filename, KeyType key);
template<class KeyType>
void BP_show(string filename, KeyType flag);