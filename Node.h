#pragma once
template<class T>
class Node //basic
{
public:
	Node();
	~Node();
	virtual void insert_node() = 0;
	virtual void delte_node() = 0;
protected:
	T* key_value;
	Node* ptr;
};

template<class T>
class InternalNode :public Node
{
public:
	InternalNode();
	virtual ~InternalNode();
private:
	
};

template<class T>
class LeafNode :public Node
{
public:
	LeafNode();
	virtual ~LeafNode();
private:
	
};

