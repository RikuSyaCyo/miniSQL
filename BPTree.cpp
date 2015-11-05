#include "BPTree.h"
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

	if (node.size != 0)
	{
		for (int i = 0; i < node.size; i++)
		{
			node_save.keys[i] = node.keys[i];
			node_save.ptr[i] = node.ptr[i];
		}
		node_save.ptr[node.size] = node.ptr[node.size];
	}
	//cout << sizeof(node_save) << endl;
	//cout << "before write" << endl;
	buffer.writeFileBlock(&node_save, filename, node.index, sizeof(node_save));
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

	if (node.size != 0)
	{
		for (int i = 0; i < node.size; i++)
		{
			node_save.keys[i] = node.keys[i];
			node_save.ptr[i] = node.ptr[i];
		}
		node_save.ptr[node.size] = node.ptr[node.size];
	}
	//node_save.ptr[node.size] = node.ptr[node.size];
	buffer.writeFileBlock(&node_save, filename, node.index, sizeof(node_save));
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

	if (node.size != 0)
	{
		for (int i = 0; i < node.size; i++)
		{
			for (int k = 0; k < (indexcatelog.KeyType + 1); k++)
			{
				node_save.keys[i*(indexcatelog.KeyType + 1) + k] = node.keys[i][k];
			}

			node_save.ptr[i] = node.ptr[i];
		}
	}
	node_save.ptr[node.size] = node.ptr[node.size];
	buffer.writeFileBlock(&node_save, filename, node.index, sizeof(node_save));
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

	buffer.readFileBlock(&node_read, FileName, nodeindex, sizeof(node_read));
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

	buffer.readFileBlock(&node_read, FileName, nodeindex, sizeof(node_read));
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

	buffer.readFileBlock(&node_read, FileName, nodeindex, sizeof(node_read));
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
	indexcatelog.SaveIndexCatelog(FileName);
}

//新建索引
void createIndex(int size, int KeyType, string FileName)
{
	IndexCatelog IndexCatelog(size, KeyType);
	if (KeyType == -1)
	{
		Node<int> node;
		node.index = IndexCatelog.nextIndex();
		saveBPnode(FileName, node);
	}
	else if (KeyType == 0)
	{
		Node<float> node;
		node.index = IndexCatelog.nextIndex();
		saveBPnode(FileName, node);
	}
	else
	{
		Node<string> node;
		node.index = IndexCatelog.nextIndex();
		saveBPnode(FileName, node);
	}
	IndexCatelog.SaveIndexCatelog(FileName);
}

//删除索引
void dropIndex(string filename)
{
	//buffer.delete(filename);
}


