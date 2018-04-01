#pragma once
class BinData
{
public:
	BinData();
	BinData(UCHAR* data,int length);
	virtual ~BinData();

public:
	UCHAR* data;
	int length;
};

