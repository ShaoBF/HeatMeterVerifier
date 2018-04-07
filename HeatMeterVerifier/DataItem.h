#pragma once
#include "BinData.h"

class DataItem
{
public:
	DataItem();
	DataItem(BinData* data, bool hasUnit, int decimalPoint = 0, bool highByteFirst = true, CString name = L"");
	virtual ~DataItem();

public:
	union Value{
		double dv;
		DWORD dwv;
		UCHAR* puc;
	} value;
	UCHAR unit;
	CString name;

	BinData* rawData;

protected:
	bool hasUnit;
	int decimalPoint;//非负表示数值型的小数点位置，-1表示value为duc，-2表示puc
	bool highByteFirst;

public:
	void SetData(UCHAR* raw, int dataLength, bool hasUnit, int decimalPoint, bool highByteFirst, CString name = L"");
	void SetData(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst, CString name = L"");
	void SetName(CString name);
	CString GetName();
	virtual void Parse(bool highByteFirst);
	CString ToString();
};

