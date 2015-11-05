#pragma once
#include <string>
#include <vector>
#include "FilePosition.h"
using namespace std;

typedef vector<FilePosition> TupleResults;

static const int ATTRIMAXNUM = 32; // max number of attributes
static const bool NEWTABLE = true; // flag to new a table
static const bool NEWTUPLE = true; // flag to new a tuple
static const string TupleFilePostfix = "DATA"; // tuple file postfix
static const int TABLENAMELENGTH = 32; //max length of table name
static const int ATTRIBUTENAMELENGTH = 32; //max length of attribute name

static const int baseIndex = 1;

//operation flag
static const int EQUAL = 0;
static const int NOEQUAL = 1;
static const int LESSTHAN = 2;
static const int GREATERTHAN = 3;
static const int NOGREATERTHAN = 4;
static const int NOLESSTHAN = 5;

// type flag
static const  int INT = 0;
static const  int CHAR = 1;
static const  int FLOAT = 2;