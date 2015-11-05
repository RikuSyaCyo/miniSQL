#pragma once
#include<iostream>
#include<vector>
#include"IndexCatelog.h"
using namespace std;
extern int a;

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
	void split(Node<KeyType>& parent, string filename, IndexCatelog &indexcatelog);
	void merge(Node<KeyType>& parent, string filename, IndexCatelog &indexcatelog);
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
inline void Node<KeyType>::split(Node<KeyType>& parent, string filename, IndexCatelog &indexcatelog)
{
	if (parent.empty == 1)
	{
		parent.empty = 0;
		parent.ptr.push_back(index);
		//parent.keys.push_back(node.keys[node.size / 2 - 1]);
		//parent.ptr.push_back(-1);
		parent.isLeaf = 0;
		parent.size = 0;
		parent.index = indexcatelog.nextIndex();
		indexcatelog.root = parent.index;
	}

	//set brother node
	Node<KeyType> bro;
	int i = 0;
	bro.size = (size + 1) / 2;
	if (isLeaf == 1)
	{
		for (i = 0; i < bro.size; i++)
		{
			bro.keys.push_back(keys[bro.size + i]);
			bro.ptr.push_back(ptr[bro.size + i]);
		}
		bro.ptr.push_back(ptr[bro.size + i]);
		bro.isLeaf = isLeaf;
		bro.index = indexcatelog.nextIndex();
		size -= bro.size;
	}
	else
	{
		for (i = 0; i < bro.size - 1; i++)
		{
			bro.keys.push_back(keys[bro.size + i + 1]);
			bro.ptr.push_back(ptr[bro.size + i + 1]);
		}
		bro.ptr.push_back(ptr[bro.size + i + 1]);
		bro.isLeaf = isLeaf;
		bro.index = indexcatelog.nextIndex();
		size -= bro.size;
		bro.size -= 1;
	}

	//set self	
	if (isLeaf == 1)
	{
		ptr[size] = bro.index;
	}

	//set parent
	int j = 0;
	if (parent.size != 0)
	{
		for (j = 0; j < parent.size; j++)
			if (parent.keys[j]>bro.keys[0]) break;
	}
	parent.keys.insert(parent.keys.begin() + j, bro.keys[0]);
	parent.ptr.insert(parent.ptr.begin() + j + 1, bro.index);
	parent.size += 1;

    saveBPnode(filename, parent);
	saveBPnode(filename, bro);
}

template<class KeyType>
inline void Node<KeyType>::merge(Node<KeyType>& parent, string filename, IndexCatelog &indexcatelog)
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
				for (k = 0; k < size; k++)
				{
					bro.keys.push_back(keys[k]);
					bro.ptr.push_back(ptr[k]);
				}
				bro.ptr.push_back(ptr[k]);
				bro.size += size;
				parent.keys.erase(parent.keys.begin() + i - 1);
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
void saveBPnode(string filename, Node<int> node);
void saveBPnode(string filename, Node<float> node);
void saveBPnode(string filename, Node<string> node);
void loadBPnode(string FileName, Node<int>& node, int nodeindex, int flag);
void loadBPnode(string FileName, Node<float> &node, int nodeindex, float flag);
void loadBPnode(string FileName, Node<string> &node, int nodeindex, string flag);
void createIndex(int size, int KeyType, string FileName);
void dropIndex(string filename);
//search
template<class KeyType>
int searchIndex(string filename, KeyType key)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;
	KeyType flag=0;

	Node<KeyType> node;
	loadBPnode(filename, node, indexcatelog.root, flag);

	while (node.isLeaf != 1)
	{
		int i = 0;
		for (i = 0; i < node.size; i++)
		{
			if (node.keys[i] > key) break;
		}

		if (i == node.size) //比非叶节点上任何一个搜索码值都要大
			next_index = node.ptr[node.size];
		else
			next_index = node.ptr[i];
		node.keys.clear();
		node.ptr.clear();
		loadBPnode(filename, node, next_index, flag);
	}
	//搜索到叶节点
	int i = 0;
	for (i = 0; i < node.size; i++)
	{
		if (node.keys[i] == key) break;
	}
	if (i == node.size) //叶节点没有符合搜索值的值
	{
		return -1;
	}
	else
	{
		return node.ptr[i];
	}
}

//search by op
template<class KeyType>
vector<int> searchLessThan(string filename, KeyType key, int closure)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;
	KeyType flag;

	Node<KeyType> node;
	loadBPnode(filename, node, indexcatelog.root, flag);
	while (node.isLeaf != 1)
	{
		next_index = node.ptr[0];
		node = loadBPnode(filename, node, next_index, flag);
	}
	vector<int> result;
	int i = 0;
	while (next_index != -1)
	{
		for (i = 0; i < node.size; i++)
		{
			if (closure == 0)  //不包含
			{
				if (node.keys[i] <= key)
					result.push_back(node.keys[i]);
			}
			else if (closure == 1) //包含
			{
				if (node.keys[i] < key)
					result.push_back(node.keys[i]);
			}
			else break;
		}
		if (i = node.size)
		{
			next_index = node.ptr[node.size];
			loadBPnode(filename, node, next_index, flag);
		}
		else
			break;
	}
	return result;
}

template<class KeyType>
vector<int> searchBiggerThan(string filename, KeyType key, int closure)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;
	KeyType flag;
	vector<int> result;

	Node<KeyType> node;
	loadBPnode(filename, node, indexcatelog.root, flag);
	while (node.isLeaf != 1)
	{
		int i = 0;
		for (i = 0; i < node.size; i++)
		{
			if (node.keys[i] > key) break;
		}

		if (i = node.size) //比非叶节点上任何一个搜索码值都要大
			next_index = node.ptr[node.size];
		else
			next_index = node.ptr[i];
		loadBPnode(filename, node, next_index, flag);
	}
	//搜索到叶节点
	int i = 0;
	for (i = 0; i < node.size; i++)
	{
		if (closure == 1) //包含
		{
			if (node.keys[i] >= key) break;
		}
		else
			if (node.keys[i] > key) break;
	}
	if (i != node.size) //叶节有符合搜索值的值
	{
		while (next_index != -1)
		{
			for (int k = i; k < node.size; k++)
				result.push_back(node.ptr[k]);
			i = 0;
			next_index = node.ptr[node.size];
			loadBPnode(filename, node, next_index, flag);
		}
	}
	return result;
}

//insert index
template<class KeyType>
int insertIndex(string filename, KeyType key, int ptr)  //ptr指的是索引所在的行信息存在磁盘的哪一块中
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	Node<KeyType> emptyNode;
	emptyNode.empty = 1;
	int result;
	result = BP_insert(filename, indexcatelog.root, emptyNode, key, ptr, indexcatelog);

	indexcatelog.SaveIndexCatelog(filename);
	return result;
}

template<class KeyType>
int insert_in_leaf(Node<KeyType> &node, KeyType key, int ptr)
{
	int i = 0;
	for (i = 0; i < node.size; i++)
		if (node.keys[i]>key) break;
	node.ptr.insert(node.ptr.begin() + i, ptr);
	node.keys.insert(node.keys.begin() + i, key);
	node.size += 1;
	return i;
}

template<class KeyType>
int BP_insert(string filename, int index, Node<KeyType>& parent, KeyType key, int ptr, IndexCatelog &indexcatelog)
{
	//a++;
	//cout << a << endl;

	KeyType flag=0;
	int result = -1;
	Node<KeyType> node;
	loadBPnode(filename, node, index, flag);

	if (node.isLeaf == 1)
	{
		result = insert_in_leaf(node, key, ptr);
		//cout << "insert key" <<key<< endl;
	}
		
	else
	{
		int i; //position
		for (i = 0; i < node.size; i++)
		{
			if (node.keys[i]>key) 
				break;
		}
			
		result = BP_insert(filename, node.ptr[i], node, key, ptr, indexcatelog);
		if (result == 0)
		{
			Node<KeyType> son;
			loadBPnode(filename, son, node.ptr[i], flag);
			if (node.keys[i] != son.keys[0])
				node.keys[i] = son.keys[0];
		}
	}
	if (node.size > indexcatelog.size-1)
		node.split(parent, filename, indexcatelog);
	//cout << "before save" << index <<endl;
	saveBPnode(filename, node);
	//cout << "after save" << index <<endl;
	//cout << a << endl;
	return result;
}

//delete index
template<class KeyType>
int deleteIndex(string filename, KeyType key)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	Node<KeyType> emptyNode;
	emptyNode.empty = 1;
	int result;
	result = BP_delete(filename, indexcatelog.root, emptyNode, key, indexcatelog);

	indexcatelog.SaveIndexCatelog(filename);
	return result;
}

template<class KeyType>
int delete_in_leaf(string filename, KeyType key, Node<KeyType> &node)
{
	int i; //position
	for (i = 0; i < node.size; i++)
		if (node.keys[i] == key) break;
	if (i = node.size)
		cout << "您想要删除的值不存在哦" << endl;
	if (node.isLeaf == 0 || node.size < 1 || node.keys[i] != key)
		return -1;
	node.keys.erase(node.keys.begin() + i);
	node.ptr.erase(node.ptr.begin() + i);
	node.size -= 1;
	return i;
}
template<class KeyType>
int BP_delete(string filename, int index, Node<KeyType> parent, KeyType key, IndexCatelog &indexcatelog)
{

	KeyType flag;
	int result = -1;
	Node<KeyType> node;
	loadBPnode(filename, node, index, flag);
	if (node.isLeaf == 1)
		delete_in_leaf(filename, key, node);
	else
	{
		int i;
		for (i = 0; i < node.size; i++)
			if (node.keys[i]>key) break;
		result = BP_delete(filename, node.ptr[i], node, key);
		if (result == 0 && i < node.size)
		{
			Node<KeyType> son;
			loadBPnode(filename, son, node.ptr[i], flag);
			if (node.keys[i - 1] != son.keys[0])
				node.keys[i - 1] = son.keys[0];
		}
	}
	if (node.size < (indexcatelog.size - 1) / 2)
		node.merge(parent, filename, indexcatelog);

	saveBPnode(filename, node);

	return result;
}
template<class KeyType>
void BP_show(string filename, KeyType flag)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);
	dfs(filename, 0, indexcatelog.root, indexcatelog, flag);
}

template<class KeyType>
void dfs(string filename, int level, int index, IndexCatelog indexcatelog, KeyType flag)
{
	Node<KeyType> node;
	
	loadBPnode(filename, node, index, flag);
	if (node.isLeaf == 1)
		cout << "leaf:" << level<<" ";
	else
		cout << "level:" << level<<" ";
	cout << "index:"<<node.index << " ";
	cout << "size: "<<node.size <<" "<< "isLeaf?" << node.isLeaf << " ";
	for (int i = 0; i < node.size; i++)
	{
		cout << node.ptr[i] << "/" << node.keys[i] << "/";
	}
	cout << node.ptr[node.size] << endl;
    if(node.isLeaf!=1)
	{
		for (int i = 0; i < node.size + 1; i++)
			dfs(filename, level + 1, node.ptr[i], indexcatelog, flag);
	}
}