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
	KeyType min; //最小值
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
		bro.min = bro.keys[0];
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

		Node<KeyType> son;
		loadBPnode(filename, son, bro.ptr[0]);
		bro.min = son.min;
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
	parent.keys.insert(parent.keys.begin() + j, bro.min);
	parent.ptr.insert(parent.ptr.begin() + j + 1, bro.index);
	Node<KeyType> son;
	loadBPnode(filename, son, parent.ptr[0]);
	parent.min = son.min;
	parent.size += 1;

    saveBPnode(filename, parent);
	saveBPnode(filename, bro);
}

template<class KeyType>
inline void Node<KeyType>::merge(Node<KeyType>& parent, string filename, IndexCatelog &indexcatelog)
{
	int i = 0; //position
	for (i = 0; i < parent.size + 1; i++)
		if (parent.ptr[i]==index) break;
	if (i == parent.size)
	{
		Node<KeyType> bro;
		loadBPnode(filename, bro, parent.ptr[i - 1]);
		if (isLeaf == 1)
		{
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
				bro.min = bro.keys[0];
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
				bro.min = bro.keys[0];
				min = keys[0];
				parent.keys[i - 1] = keys[0];
			}
			saveBPnode(filename, bro);
			if ((parent.index == indexcatelog.root) && (parent.size == 0))
				indexcatelog.root = bro.index;
		}
		else
		{
			if (bro.size + size + 1 < indexcatelog.size)
			{
				int k = 0;

				Node<KeyType> son;
				loadBPnode(filename, son, ptr[0]);
				bro.keys.push_back(son.min);

				for (k = 0; k < size ; k++)
				{
					bro.keys.push_back(keys[k]);
					bro.ptr.push_back(ptr[k]);
				}
			
				bro.ptr.push_back(ptr[k]);
				bro.size += (size + 1);
				parent.keys.erase(parent.keys.begin() + i - 1);
				parent.ptr.erase(parent.ptr.begin() + i);
				parent.size -= 1;
			}
			else
			{
				Node<KeyType> son;
				loadBPnode(filename, son, ptr[0]);
				keys.insert(keys.begin(), son.min);

				//keys.insert(keys.begin(), bro.keys[bro.size - 2]);
				ptr.insert(ptr.begin(), bro.ptr[bro.size]);
				bro.keys.erase(bro.keys.begin() + bro.size - 1);
				bro.ptr.erase(bro.ptr.begin() + bro.size);
				size += 1;
				bro.size -= 1;

				loadBPnode(filename, son, ptr[0]);
				min = son.min;

				parent.keys[i - 1] = min;
			}
			saveBPnode(filename, bro);
			if ((parent.index == indexcatelog.root) && (parent.size == 0))
				indexcatelog.root = bro.index;
		}
	}
	else
	{
		Node<KeyType> bro;
		loadBPnode(filename, bro, parent.ptr[i + 1]);
		if (isLeaf == 1)
		{
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

				min = keys[0];
			}
			else
			{
				keys.push_back(bro.keys[0]);
				ptr.insert(ptr.begin() + size, bro.ptr[0]);
				bro.keys.erase(bro.keys.begin());
				bro.ptr.erase(bro.ptr.begin());
				size += 1;
				bro.size -= 1;

				bro.min = bro.keys[0];
				parent.keys[i] = bro.min;
			}
			saveBPnode(filename, bro);
			if ((parent.index == indexcatelog.root) && (parent.size == 0))
				indexcatelog.root =index;
		}
		else
		{
			if (bro.size + size + 1 < indexcatelog.size)
			{
				Node<KeyType> son;
				loadBPnode(filename, son, bro.ptr[0]);
				keys.push_back(son.min);

				int k = 0;
				for (k = 0; k < bro.size; k++)
				{
					keys.push_back(bro.keys[k]);
					ptr.push_back(bro.ptr[k]);
				}
				ptr.push_back(bro.ptr[k]);
				size += (bro.size+1);
				parent.keys.erase(parent.keys.begin() + i);
				parent.ptr.erase(parent.ptr.begin() + i + 1);
				parent.size -= 1;

				loadBPnode(filename, son, ptr[0]);
				min = son.min;
			}
			else
			{
				Node<KeyType> son;
				loadBPnode(filename, son, bro.ptr[0]);
				keys.push_back(son.min);

				ptr.insert(ptr.begin() + size + 1, bro.ptr[0]);
				bro.keys.erase(bro.keys.begin());
				bro.ptr.erase(bro.ptr.begin());
				size += 1;
				bro.size -= 1;

				loadBPnode(filename, son, bro.ptr[0]);
				bro.min = son.min;
				parent.keys[i] = bro.min;
			}
			saveBPnode(filename, bro);
			if ((parent.index == indexcatelog.root) && (parent.size == 0))
				indexcatelog.root = index;
		}
	}
	saveBPnode(filename, parent);
}

/*********************************statement of BPtree*******************************************/
void saveBPnode(string filename, Node<int> node);
void saveBPnode(string filename, Node<float> node);
void saveBPnode(string filename, Node<string> node);
void loadBPnode(string FileName, Node<int>& node, int nodeindex);
void loadBPnode(string FileName, Node<float> &node, int nodeindex);
void loadBPnode(string FileName, Node<string> &node, int nodeindex);
void createIndex(int size, int KeyType, string FileName);
void dropIndex(string filename);
//search
template<class KeyType>
int searchIndex(string filename, KeyType key)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;

	Node<KeyType> node;
	loadBPnode(filename, node, indexcatelog.root);

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
		loadBPnode(filename, node, next_index);
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

	Node<KeyType> node;
	loadBPnode(filename, node, indexcatelog.root);
	while (node.isLeaf != 1)
	{
		next_index = node.ptr[0];
		node.keys.clear();
		node.ptr.clear();
		loadBPnode(filename, node, next_index);
	}
	vector<int> result;
	int i = 0;
	while (next_index != -1)
	{
		for (i = 0; i < node.size; i++)
		{
			if (closure == 0)  //不包含
			{
				if (node.keys[i] < key)
					result.push_back(node.ptr[i]);
			}
			else if (closure == 1) //包含
			{
				if (node.keys[i] <= key)
					result.push_back(node.ptr[i]);
			}
			else break;
		}
		if (i == node.size)
		{
			next_index = node.ptr[node.size];
			node.keys.clear();
			node.ptr.clear();
			loadBPnode(filename, node, next_index);
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
	vector<int> result;

	Node<KeyType> node;
	loadBPnode(filename, node, indexcatelog.root);
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
		loadBPnode(filename, node, next_index);
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
			node.keys.clear();
			node.ptr.clear();
			loadBPnode(filename, node, next_index);
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
	if (i == 0)
	{
		node.min = key;
	}
	return i;
}

template<class KeyType>
int BP_insert(string filename, int index, Node<KeyType>& parent, KeyType key, int ptr, IndexCatelog &indexcatelog)
{
	//a++;
	//cout << a << endl;

	int result = -1;
	Node<KeyType> node;
	loadBPnode(filename, node, index);

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
int delete_in_leaf(string filename, KeyType key, Node<KeyType> &node, Node<KeyType> &parent)
{
	int i; //position
	for (i = 0; i < node.size; i++)
	{
		if (node.keys[i] == key) 
			break;
	}
		
	if (i == node.size)
		cout << "您想要删除的值不存在哦" << key <<endl;
	else
	{
		node.keys.erase(node.keys.begin() + i);
		node.ptr.erase(node.ptr.begin() + i);
		node.size -= 1;
		if (node.size != 0)
		{
			node.min = node.keys[0];
		}
	}
	/*if (i == 0 && (node.size!=0))
	{
		int k;
		for (k = 0; k < parent.size; k++)
		{
			if (parent.keys[k]>key)
				break;
		}
		if (k != 0)
		{
			parent.keys[k - 1] = node.keys[0];
		}
		else
		{
			parent.min = node.min;
		}
	}*/
	return i;
}
template<class KeyType>
int BP_delete(string filename, int index, Node<KeyType>& parent, KeyType key, IndexCatelog &indexcatelog)
{
	int result = -1;
	Node<KeyType> node;
	loadBPnode(filename, node, index);
	if (node.isLeaf == 1)
		delete_in_leaf(filename, key, node, parent );
	else
	{
		int i;
		for (i = 0; i < node.size; i++)
		{
			if (node.keys[i]>key) 
				break;
		}
		result = BP_delete(filename, node.ptr[i], node, key, indexcatelog);
	}
	if (node.isLeaf == 1 && (parent.empty!=1))
	{
		if (node.size < ceil((double)(indexcatelog.size - 1) / 2))
			node.merge(parent, filename, indexcatelog);
		saveBPnode(filename, node);
	}
	else if (parent.empty == 1)
	{
		saveBPnode(filename, node);
		return 1;
	}
	else
	{
		if (node.size < ceil((double)(indexcatelog.size / 2))-1)
			node.merge(parent, filename, indexcatelog);
		saveBPnode(filename, node);
	}
	Node<KeyType> test;
	loadBPnode(filename, test, parent.ptr[0]);
	parent.min = test.min;
	for (int i = 0; i < parent.size; i++)
	{
		loadBPnode(filename, test, parent.ptr[i+1]);
		parent.keys[i] = test.min;
	}
	saveBPnode(filename, parent);

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
	
	loadBPnode(filename, node, index);
	if (node.isLeaf == 1)
		cout << "leaf:" << level<<" ";
	else
		cout << "level:" << level<<" ";
	cout << "index:"<<node.index << " ";
	cout << "size: "<<node.size <<" "<< "isLeaf?" << node.isLeaf << " ";
	cout << "min:" << node.min << " ";
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