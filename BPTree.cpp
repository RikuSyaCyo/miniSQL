#include "BPTree.h"
#include"ConstantTable.h"
#include<math.h>
//int -1
//float 0
//char 1~255
extern BufferPool buffer;
struct node_int
{
	bool isLeaf;
	int index;
	int size;
	int keys[200];
	int ptr[200];
};
struct node_float
{
	bool isLeaf;
	int index;
	int size;
	float keys[200];
	int ptr[200];
};
struct node_string
{
	bool isLeaf;
	int index;
	int size;
	char keys[800];
	int ptr[200];
};

//BPsave
void saveBPnode(string filename, Node<int> node)
{

	struct node_int node_save;
	node_save.isLeaf = node.isLeaf;
	node_save.index = node.index;
	node_save.size = node.size;
	
	//node_save.keys = new int[node.size];
	//node_save.ptr = new int[node.size + 1];

	for (int i = 0; i < node.size; i++)
	{
		node_save.keys[i] = node.keys[i];
		node_save.ptr[i] = node.ptr[i];
	}
	node_save.ptr[node.size] = node.ptr[node.size];
	//cout << sizeof(node_save) << endl;
	//cout << "before write" << endl;
	buffer.writeFileBlock(&node_save, filename, baseIndex + node.index, sizeof(node_save));
	//cout << "after write" << endl;

}
void saveBPnode(string filename, Node<float> node)
{
	struct node_float node_save;
	node_save.isLeaf = node.isLeaf;
	node_save.index = node.index;
	node_save.size = node.size;

	//node_save.keys = new float[node.size];
	//node_save.ptr = new int[node.size + 1];

	for (int i = 0; i < node.size; i++)
	{
		node_save.keys[i] = node.keys[i];
		node_save.ptr[i] = node.ptr[i];
	}
	node_save.ptr[node.size] = node.ptr[node.size];

	//node_save.ptr[node.size] = node.ptr[node.size];
	buffer.writeFileBlock(&node_save, filename, baseIndex + node.index, sizeof(node_save));
}
void saveBPnode(string filename, Node<string> node)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	struct node_string node_save;
	node_save.isLeaf = node.isLeaf;
	node_save.index = node.index;
	node_save.size = node.size;

	//node_save.keys = new char[indexcatelog.KeyType*node.size];
	//node_save.ptr = new int[node.size + 1];

	for (int i = 0; i < node.size; i++)
	{
		for (int k = 0; k < (indexcatelog.KeyType + 1); k++)
		{
			node_save.keys[i*(indexcatelog.KeyType + 1) + k] = node.keys[i][k];
		}

		node_save.ptr[i] = node.ptr[i];
	}
	node_save.ptr[node.size] = node.ptr[node.size];
	
	buffer.writeFileBlock(&node_save, filename, baseIndex + node.index, sizeof(node_save));
	indexcatelog.SaveIndexCatelog(filename);
}

//BPload
void loadBPnode(string FileName, Node<int>& node, int nodeindex, int flag)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(FileName);

	struct node_int node_read;
	//node_read.keys = new int[indexcatelog.size];
	//node_read.ptr = new int[indexcatelog.size + 1];

	buffer.readFileBlock(&node_read, FileName, baseIndex + nodeindex, sizeof(node_read));
	node.isLeaf = node_read.isLeaf;
	node.index = node_read.index;
	node.size = node_read.size;
	for (int i = 0; i < node.size; i++)
	{
		node.keys.push_back(node_read.keys[i]);
		node.ptr.push_back(node_read.ptr[i]);
	}
	node.ptr.push_back(node_read.ptr[node.size]);
	indexcatelog.SaveIndexCatelog(FileName);
	//cout << nodeindex << endl;
}
void loadBPnode(string FileName, Node<float> &node, int nodeindex, float flag)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(FileName);

	struct node_float node_read;
	//node_read.keys = new float[indexcatelog.size];
	//node_read.ptr = new int[indexcatelog.size + 1];

	buffer.readFileBlock(&node_read, FileName, baseIndex + nodeindex, sizeof(node_read));
	node.isLeaf = node_read.isLeaf;
	node.index = node_read.index;
	node.size = node_read.size;
	for (int i = 0; i < node.size; i++)
	{
		node.keys.push_back(node_read.keys[i]);
		node.ptr.push_back(node_read.ptr[i]);
	}
	node.ptr.push_back(node_read.ptr[node.size]);
	indexcatelog.SaveIndexCatelog(FileName);
}
void loadBPnode(string FileName, Node<string> &node, int nodeindex, string flag)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(FileName);

	struct node_string node_read;
	//node_read.keys = new char[indexcatelog.size*indexcatelog.KeyType];
	//node_read.ptr = new int[indexcatelog.size + 1];

	buffer.readFileBlock(&node_read, FileName, baseIndex + nodeindex, sizeof(node_read));
	node.isLeaf = node_read.isLeaf;
	node.index = node_read.index;
	node.size = node_read.size;
	string read = "";
	char* p= node_read.keys;
	for (int i = 0; i < node.size; i++)
	{
		read.insert(0,p);
		node.keys.push_back(read);
		node.ptr.push_back(node_read.ptr[i]);
		read = "";
		p += (indexcatelog.KeyType + 1);
	}
	node.ptr.push_back(node_read.ptr[node.size]);
	indexcatelog.SaveIndexCatelog(FileName);
}

//新建索引
void createIndex(int size, int KeyType, string FileName)
{
	IndexCatelog IndexCatelog(size, KeyType);
	if (KeyType == INT_BP)
	{
		Node<int> node;
		node.index = IndexCatelog.nextIndex();
		node.ptr.push_back(-1);
		saveBPnode(FileName, node);
	}
	else if (KeyType == FLOAT_BP)
	{
		Node<float> node;
		node.index = IndexCatelog.nextIndex();
		node.ptr.push_back(-1);
		saveBPnode(FileName, node);
	}
	else
	{
		Node<string> node;
		node.index = IndexCatelog.nextIndex();
		node.ptr.push_back(-1);
		saveBPnode(FileName, node);
	}
	IndexCatelog.SaveIndexCatelog(FileName);
}

//删除索引
void dropIndex(string filename)
{
	//buffer.delete(filename);
}

int searchIndex(string filename, string key)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;
	string flag = "";

	Node<string> node;
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
vector<int> searchLessThan(string filename, string key, int closure)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;
	string flag="";

	Node<string> node;
	loadBPnode(filename, node, indexcatelog.root, flag);

	while (node.isLeaf != 1)
	{
		next_index = node.ptr[0];
		node.keys.clear();
		node.ptr.clear();
		loadBPnode(filename, node, next_index, flag);
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
			loadBPnode(filename, node, next_index, flag);
		}
		else
			break;
	}
	return result;
}
vector<int> searchBiggerThan(string filename, string key, int closure)
{
	IndexCatelog indexcatelog;
	indexcatelog.LoadIndexCatelog(filename);

	int next_index;
	string flag="";
	vector<int> result;

	Node<string> node;
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
			loadBPnode(filename, node, next_index, flag);
		}
	}
	return result;
}
int BP_insert(string filename, int index, Node<string>& parent, string key, int ptr, IndexCatelog &indexcatelog)
{
	string flag = "";
	int result = -1;
	Node<string> node;
	loadBPnode(filename, node, index, flag);

	if (node.isLeaf == 1)
	{
		result = insert_in_leaf(node, key, ptr);
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
	if (node.size > indexcatelog.size - 1)
		node.split(parent, filename, indexcatelog);
	saveBPnode(filename, node);
	return result;
}
int BP_delete(string filename, int index, Node<string>& parent, string key, IndexCatelog &indexcatelog)
{
	string flag="";
	int result = -1;
	Node<string> node;
	loadBPnode(filename, node, index, flag);
	if (node.isLeaf == 1)
		delete_in_leaf(filename, key, node);
	else
	{
		int i;
		for (i = 0; i < node.size; i++)
			if (node.keys[i]>key) break;
		result = BP_delete(filename, node.ptr[i], node, key, indexcatelog);
	}
	if (node.isLeaf == 1)
	{
		if (node.size < ceil((double)(indexcatelog.size - 1) / 2))
			node.merge(parent, filename, indexcatelog);
	}
	else if (parent.empty == 1)
	{
		return 1;
	}
	else
	{
		if (node.size < ceil((double)(indexcatelog.size / 2))-1)
			node.merge(parent, filename, indexcatelog);
	}

	saveBPnode(filename, node);

	return result;
}