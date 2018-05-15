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
	int decimalPoint;//�Ǹ���ʾ��ֵ�͵�С����λ�ã�-1��ʾvalueΪduc��-2��ʾpuc
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
	double* GetBounds(double rate);//����double[2],����[0]Ϊ�ο��½磬[1]Ϊ�ο��Ͻ硣
	CString GetItemRangeStr(double rate);
	double GetValue();
	CString VerifyWith(DataItem* refItem, double rate);//������ο���refItem�Աȣ�rateΪ�����ʣ�����1(����)��0(�ϸ�)��-1(����)�����ұ�����
	CString GetUnitStr();
};

