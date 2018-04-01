#include "stdafx.h"
#include "BinData.h"


BinData::BinData()
{
}
BinData::BinData(UCHAR* data, int length){
	this->data = data;
	this->length = length;
}

BinData::~BinData()
{
}
