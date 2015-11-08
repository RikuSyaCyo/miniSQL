#include "TupleResults.h"


TupleResults::TupleResults(string tableName)
{
	belongTable = tableName;
}

TupleResults::~TupleResults()
{
}

void TupleResults::insert(int tupleIndex)
{
	tuples.push_back(tupleIndex);
}

int TupleResults::getTupleIndexAt(int vecIndex)
{
	return tuples[vecIndex];
}

int TupleResults::size() const
{
	return tuples.size();
}

string TupleResults::tableName()
{
	return belongTable;
}

void TupleResults::And(const TupleResults& res)
{
	/*if (res.size() < tuples.size())
	{
		vector<int> temp = res.tuples;
		res.tuples = tuples;
		tuples = temp;
	}*/
	int thisSize = tuples.size();
	vector<int>::iterator iter;
	for (iter = tuples.begin(); iter != tuples.end();)
	{
		if (find(res.tuples.begin(), res.tuples.end(), *iter) == res.tuples.end())
		{
			iter = tuples.erase(iter);
		}
		else
			iter++;
	}
}

void TupleResults::Set(const vector<int>& res)
{
	tuples = res;
}