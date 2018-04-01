#pragma once
#include "BinData.h"

class DataItem
{
public:
	DataItem();
	DataItem(BinData* data, bool hasUnit, int decimalPoint = 0, bool highByteFirst=true);
	virtual ~DataItem();

public:
	union Value{
		double dv;
		DWORD dwv;
	} value;
	UCHAR unit;
	BinData* rawData;
protected:
	bool hasUnit;
	int decimalPoint;
	bool highByteFirst;

public:
	void SetData(UCHAR* raw, int dataLength, bool hasUnit, int decimalPoint, bool highByteFirst);
	void SetData(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst);
	virtual void Parse(bool highByteFirst);
	CString ToString();
};

