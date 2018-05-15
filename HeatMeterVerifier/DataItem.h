#pragma once
#include "BinData.h"

union UnionValue{
	double dv;
	DWORD dwv;
	UCHAR* puc;
	UINT16 ui16v;
};
class DataItem
{
public:
	DataItem();
	DataItem(BinData* data, bool hasUnit, int decimalPoint = 0, bool highByteFirst = true, CString name = L"");
	virtual ~DataItem();

public:
	UnionValue value;
	UCHAR unit;
	CString name;
	int decimalPoint;//非负表示数值型的小数点位置，-1表示value为duc，-2表示puc
	bool hasUnit;

	BinData* rawData;

protected:
	bool highByteFirst;

public:
	void SetData(UCHAR* raw, int dataLength, bool hasUnit, int decimalPoint, bool highByteFirst, CString name = L"");
	void SetData(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst, CString name = L"");
	void SetData(UnionValue v, int dec, bool hasUnit, UCHAR unit = 0);
	void SetName(CString name);
	CString GetName();
	virtual void Parse(bool highByteFirst);
	CString ToString();
	CString GetValueStr();
	double GetUpperBound(double rate);
	double GetLowerBound(double rate);
	double* GetBounds(double rate);//返回double[2],其中[0]为参考下界，[1]为参考上界。
	CString GetItemRangeStr(double rate);
	double GetValue();
	CString VerifyWith(DataItem* refItem, double rate);//与给定参考量refItem对比，rate为浮动率，返回1(过高)，0(合格)，-1(过低)，左开右闭区间
	CString GetUnitStr();
};

