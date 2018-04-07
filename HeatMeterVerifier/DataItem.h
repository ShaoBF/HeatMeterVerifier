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
	int decimalPoint;//�Ǹ���ʾ��ֵ�͵�С����λ�ã�-1��ʾvalueΪduc��-2��ʾpuc
	bool highByteFirst;

public:
	void SetData(UCHAR* raw, int dataLength, bool hasUnit, int decimalPoint, bool highByteFirst, CString name = L"");
	void SetData(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst, CString name = L"");
	void SetName(CString name);
	CString GetName();
	virtual void Parse(bool highByteFirst);
	CString ToString();
};

