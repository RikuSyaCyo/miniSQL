#pragma once
#include <vector>
#include <iostream>
using namespace std;

class TupleResults
{
private:
	vector<int> tuples;
	string belongTable;
public:
	TupleResults(string tableName);
	~TupleResults();
	void insert(int tupleIndex);
	int getTupleIndexAt(int vecIndex);
	int size() const;
	string tableName();
	void And(const TupleResults& res);
	void Set(const vector<int>& res);
};

