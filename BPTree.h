#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include"BufferPool.h"
using namespace std;

BufferPool buffer;
struct info
{
	string filename;
	int n; 
}BP_info;
void read_info(string FILENAME, int N)
{
	BP_info.filename = FILENAME;
	BP_info.n = N;
}
template<class KeyType>
class Node 
{
private:
	KeyType* keys;  //索引键值
	int* line;  //行信息
	bool isLeaf;
	int count; //key number
	//int block; //block number
public:
	Node(); 
	~Node();
	void insertKey(KeyType key_value, int line);
	Node<KeyType> splitNode(KeyType key_value, int line);
	void coalesceNode(Node<KeyType> ano_node);
};

template<class KeyType>
class BPtree
{
private:
	typedef Node<KeyType> Node;
	bool head; //whether the tree is null
	int degree; //record high degree of tree
public:
	BPtree();
	~BPtree();
	void createBPtree(string filename, int n);
	void insertRecord(KeyType val,int line);
	void insert_in_leaf(Node node, KeyType key, int line, int k); //k表示是树的第几个结点
	void insert_in_parent(Node node,KeyType key, int line, int k); //
	void deleteRecord();
	void findKey();
};

/***********define of class Node*************/
template<class KeyType>
inline Node<KeyType>::Node()
{
	keys = new KeyType[BP_info.n];
	line=new int[BP_info.n]
	isLeaf = 1;
	count = -1;
}

template<class KeyType>
inline Node<KeyType>::~Node()
{
}

template<class KeyType>
inline void Node<KeyType>::insertKey(KeyType key_value, int line)
{
	if (count >= BP_info.n)
	{
		splitNode(KeyType key_value);
	}
	else
	{
		keys[count+1] = key_value;
		line[count + 1] = line;
		count++;
	}
}

template<class KeyType>
inline Node<KeyType> Node<KeyType>::splitNode(KeyType key_value, int line)
{
	Node<KeyType> newNode;
	count = (BP_info.n - 1) / 2;
	if (newNode == NULL)
	{
		cout << "Problems of memory in splitNode" << endl;
	}
	for (int i = count + 1; i < BP_info.n-1; i++)
	{
		newNode.keys[newNode.count+1] = keys[i];
		newNode.line[newNode.count + 1] = line[i];
		newNode.count++;
	}
	newNode.keys[newNode.count+1] = key_value;
	newNode.line[newNode.count + 1] = line;
	newNode.count++;
	return newNode;
}

template<class KeyType>
inline void Node<KeyType>::coalesceNode(Node<KeyType> ano_node)
{
	for (int i = 0; i <= ano_node.count; i++)
	{
		keys[count + 1] = ano_node.keys[i];
		count++;
	}
}

/*****************define of class BPtree****************/
template<class KeyType>
inline BPtree<KeyType>::BPtree()
{

}

template<class KeyType>
inline BPtree<KeyType>::~BPtree()
{
}

template<class KeyType>
inline void BPtree<KeyType>::createBPtree(string filename, int n)
{
	read_info(filename, n); //read basic information of BPtree
	head = 0; 
	degree = 0; //set the tree blank
}

template<class KeyType>
inline void BPtree<KeyType>::insertRecord(KeyType val,int line)
{
	Node newNode;
	Node comNode; //compare with this node
	if (head == 0)
	{
		newNode.insertKey(val,line);
		buffer.writeFileBlock(&newNode, BP_info.filename, baseIndex, sizeof(newNode));
		head = 1;
		degree = 1;
	}
	else
	{
		int k = 1;
		for (int i = 1; i < degree; i++)
		{
			int min_i = (pow(BP_info.n, i) - 1) / (BP_info.n - 1) + 1;  //i层第一块
			int min_ii = (pow(BP_info.n, i + 1) - 1) / (BP_info.n - 1) + 1; //i+1层第一块

			buffer.readFileBlock(&comNode, BP_info.filename, k, sizeof(comNode));
			for (int j = 0; j <= comNode.count; j++)
			{
				if (comNode.keys[j] > val)
				{
					k = min_ii + BP_info.n*(k - min_ii) + j;
					break;
				}
			}
			if(j==comNode.count)
				k = min_ii + BP_info.n*(k - min_ii) + j;
		}
		if (comNode.count >= BP_info.n - 1)
		{
			comNode.splitNode(val, line);
			insert_in_leaf(comNode, val, line, k);

			
		}
		else
		{
			insert_in_leaf(comNode,val,k);
		}
	}
}

template<class KeyType>
inline void BPtree<KeyType>::insert_in_leaf(Node node, KeyType key, int line, int k)
{
	int i;
	for (i = 0; i <= node.count; i++)
	{
		if (key < node.keys[i]) break;
	}
	for (int k = node.count; k >= i; k--)
	{
		node.keys[k + 1] = node.keys[k];
		node.line[k + 1] = node.keys[k];
	}
	node.keys[i] = key;
	node.line[i] = line;
}

template<class KeyType>
inline void BPtree<KeyType>::insert_in_parent(Node node, KeyType key, int line, int k)
{
	if (node.isLeaf == 0) //root
	{
		insertRecord(key, line);
	}
	else
	{
		Node newNode;
		
	}
}

