#include "BufferBlock.h"


BufferBlock::BufferBlock() :dirtyFlag(0), freeFlag(1), pinFlag(0)
{
}


BufferBlock::~BufferBlock()
{
}


