#include "BPTree.h"
//int -1
//float 0
//char 1~255

BufferPool buffer;
struct node_int
{
	bool isLeaf;   
	int index;   
	int size;  
	int* keys; 
	int* ptr;  
};
struct node_float
{
	bool isLeaf;   
	int index;   
	int size;  
	float* keys; 
	int* ptr;
};
struct node_string
{
	bool isLeaf; 
	int index; 
	int size;  
	char* keys; 
	int* ptr;
};

//BPsave
void saveBPnode(string filename, Node<int> node, IndexCatelog indexcatelog)
{
	struct node_int node_save;
	node_save.isLeaf = node.isLeaf;
	node_save.index = node.index;
	node_save.size = node.size;

	node_save.keys = new int[node.size];
	node_save.ptr = new int[node.size + 1];

	for (int i = 0; i < node.size; i++)
	{
		node_save.keys[i] = node.keys[i];
		node_save.ptr[i] = node.ptr[i];
	}
	node_save.ptr[node.size] = node.ptr[node.size];
	buffer.writeFileBlock(&node_save, filename, node.index, sizeof(node_save));
}
void saveBPnode(string filename, Node<float> node, IndexCatelog indexcatelog)
{
	struct node_float node_save;
	node_save.isLeaf = node.isLeaf;
	node_save.index = node.index;
	node_save.size = node.size;

	node_save.keys = new float[node.size];
	node_save.ptr = new int[node.size + 1];

	for (int i = 0; i < node.size; i++)
	{
		node_save.keys[i] = node.keys[i];
		node_save.ptr[i] = node.ptr[i];
	}
	node_save.ptr[node.size] = node.ptr[node.size];
	buffer.writeFileBlock(&node_save, filename, node.index, sizeof(node_save));
}
void saveBPnode(string filename, Node<string> node, IndexCatelog indexcatelog)
{
	struct node_string node_save;
	node_save.isLeaf = node.isLeaf;
	node_save.index = node.index;
	node_save.size = node.size;

	node_save.keys = new char[indexcatelog.KeyType*node.size];
	node_save.ptr = new int[node.size + 1];

	for (int i = 0; i < node.size; i++)
	{
		for (int k = 0; k < (indexcatelog.KeyType + 1); k++)
		{
			node_save.keys[i*(indexcatelog.KeyType + 1) +k] = node.keys[i][k];
		}
		
		node_save.ptr[i] = node.ptr[i];
	}
	node_save.ptr[node.size] = node.ptr[node.size];
	buffer.writeFileBlock(&node_save, filename, node.index, sizeof(node_save));
}

//BPload
Node<int> loadBPnode(string FileName, int nodeindex, IndexCatelog indexcatelog, int flag)
{
	struct node_int node_read;
	node_read.keys = new int[indexcatelog.size];
	node_read.ptr = new int[indexcatelog.size + 1];

	buffer.readFileBlock(&node_read, FileName, nodeindex, sizeof(node_read));
	Node<int> node;
	node.isLeaf = node_read.isLeaf;
	node.index = node_read.index;
	node.size = node_read.size;
	for (int i = 0; i < node.size; i++)
	{
		node.keys.push_back(node_read.keys[i]);
		node.ptr.push_back(node_read.ptr[i]);
	}
	node.ptr.push_back(node_read.ptr[node.size]);
	
	return node;
}
Node<float> loadBPnode(string FileName, int nodeindex, IndexCatelog indexcatelog, float flag)
{
	struct node_float node_read;
	node_read.keys = new float[indexcatelog.size];
	node_read.ptr = new int[indexcatelog.size + 1];

	buffer.readFileBlock(&node_read, FileName, nodeindex, sizeof(node_read));
	Node<float> node;
	node.isLeaf = node_read.isLeaf;
	node.index = node_read.index;
	node.size = node_read.size;
	for (int i = 0; i < node.size; i++)
	{
		node.keys.push_back(node_read.keys[i]);
		node.ptr.push_back(node_read.ptr[i]);
	}
	node.ptr.push_back(node_read.ptr[node.size]);

	return node;
}
Node<string> loadBPnode(string FileName, int nodeindex, IndexCatelog indexcatelog, string flag)
{
	struct node_string node_read;
	node_read.keys = new char[indexcatelog.size*indexcatelog.KeyType];
	node_read.ptr = new int[indexcatelog.size + 1];

	buffer.readFileBlock(&node_read, FileName, nodeindex, sizeof(node_read));
	Node<string> node;
	node.isLeaf = node_read.isLeaf;
	node.index = node_read.index;
	node.size = node_read.size;
	for (int i = 0; i < node.size; i++)
	{
		for (int k = 0; k < (indexcatelog.KeyType + 1); k++)
		{
			node.keys[i][k] = node_read.keys[i*(indexcatelog.KeyType + 1) + k];
		}
		node.ptr.push_back(node_read.ptr[i]);
	}
	node.ptr.push_back(node_read.ptr[node.size]);

	return node;
}

//新建索引
void createIndex(int size, int KeyType, string FileName)
{
	IndexCatelog IndexCatelog(size, KeyType);
	if (KeyType == -1)
	{
		Node<int> node;
		node.index = IndexCatelog.nextIndex();
		saveBPnode(FileName, node, IndexCatelog);
	}
	else if (KeyType == 0)
	{
		Node<float> node;
		node.index = IndexCatelog.nextIndex();
		saveBPnode(FileName, node, IndexCatelog);
	}
	else
	{
		Node<string> node;
		node.index = IndexCatelog.nextIndex();
		saveBPnode(FileName, node, IndexCatelog);
	}
}

//删除索引
void dropIndex(string filename)
{
	//buffer.delete(filename);
}


//search
template<class KeyType>
int searchIndex(string filename, IndexCatelog indexcatelog, KeyType key)
{
	int next_index;
	KeyType flag;

	Node<KeyType> node = loadBPnode(filename, indexcatelog.root, indexcatelog, flag);
	while (node.isLeaf !=1)
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
		node = loadBPnode(filename, next_index, indexcatelog, flag);
	}
	//搜索到叶节点
	int i = 0;
	for (i = 0; i < node.size; i++)
	{
		if (node.keys[i] = key) break;
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
vector<int> searchLessThan(string filename, IndexCatelog indexcatelog, KeyType key, int closure)
{
	int next_index;
	KeyType flag;

	Node<KeyType> node = loadBPnode(filename, indexcatelog.root, indexcatelog, flag);
	while (node.isLeaf != 1)
	{
		next_index = node.ptr[0];
		node = loadBPnode(filename, next_index, indexcatelog, flag);
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
			node = loadBPnode(filename, next_index, indexcatelog, flag);
		}	
		else
			break;
	}
	return result;
}

template<class KeyType>
vector<int> searchBiggerThan(string filename, IndexCatelog indexcatelog, KeyType key, int closure)
{
	int next_index;
	KeyType flag;
	vector<int> result;

	Node<KeyType> node = loadBPnode(filename, indexcatelog.root, indexcatelog, flag);
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
		node = loadBPnode(filename, next_index, indexcatelog, flag);
	}
	//搜索到叶节点
	int i = 0;
	for (i = 0; i < node.size; i++)
	{
		if (node.keys[i] >= key) break;
	}
	if (i != node.size) //叶节有符合搜索值的值
	{
		if (closure == 1)  //包含
		{
			result.push_back(node.ptr[i]);
		}
		while (next_index != -1)
		{
			for (int k = i; k < node.size; k++)
				result.push_back(node.ptr[k]);
			i = 0;
			next_index = node.ptr[node.size];
			node = loadBPnode(filename, next_index, indexcatelog, flag);
		}
	}
	return result;
}

//insert index
template<class KeyType>
int insertIndex(string filename, IndexCatelog indexcatelog, KeyType key, int ptr)  //ptr指的是索引所在的行信息存在磁盘的哪一块中
{
	Node<KeyType> emptyNode;
	emptyNode.empty = 1;
	int result;
	result = BP_insert(filename, indexcatelog.root, emptyNode, indexcatelog, key, ptr);
	return result;
}

template<class KeyType>
int insert_in_leaf(Node<KeyType> node, KeyType key, int ptr)
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
int BP_insert(string filename, int index, Node<KeyType> parent, IndexCatelog indexcatelog, KeyType key, int ptr)
{
	int flag = 0;
	int result = -1;
	Node<KeyType> node = loadBPnode(filename, index, indexcatelog, flag);
	
	if (node.isLeaf == 1)
		result = insert_in_leaf(node, key, ptr);
	else
	{
		int i; //position
		for (i = 0; i < node.size; i++)
			if (node.keys[i]>key) break;
		result = BP_insert(filename, node.ptr[i], node, indexcatelog, key, ptr);
		if (result == 0)
		{
			Node<KeyType> son = loadBPnode(filename, node.ptr[i], indexcatelog, flag);
			if (node.keys[i] != son.keys[0])
				node.keys[i] = son.keys[0];
		}
	}
	if (node.size > indexcatelog.size)
		node.split(parent, filename, indexcatelog);
	saveBPnode(filename, node, indexcatelog);
	return result;
}

//delete index
int deleteIndex(string filename, int key, IndexCatelog indexcatelog)
{

}
