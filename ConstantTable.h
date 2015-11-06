#pragma once
#include <string>
#include <vector>
#include "FilePosition.h"
using namespace std;

static const float EPSINON = 0.00001;

static const int ATTRIMAXNUM = 32; // max number of attributes
static const bool NEWTABLE = true; // flag to new a table
static const bool NEWTUPLE = true; // flag to new a tuple
static const string TupleFilePostfix = "DATA"; // tuple file postfix
static const int TABLENAMELENGTH = 32; //max length of table name
static const int ATTRIBUTENAMELENGTH = 32; //max length of attribute name

static const int baseIndex = 1;

//operation flag
static const int EQUAL = 2; // ==
static const int NOEQUAL = 5;// !=
static const int LESSTHAN = 1;// <
static const int GREATERTHAN = 0;// >
static const int NOGREATERTHAN = 4;// <=
static const int NOLESSTHAN = 3;// >=

// type flag
static const  int INT = 0;
static const  int FLOAT = 1;
static const  int CHAR = 2;
