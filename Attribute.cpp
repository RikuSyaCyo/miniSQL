#include "Attribute.h"


int Attribute::Bytes()const
{
	switch (type)
	{
	case INT:
		return sizeof(int);
		break;
	case FLOAT:
		return sizeof(float);
		break;
	case CHAR:
		return sizeof(char)*strLength;
		break;
	default:
		break;
	}
}
