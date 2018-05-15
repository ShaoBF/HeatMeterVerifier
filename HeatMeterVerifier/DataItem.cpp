#include "stdafx.h"
#include "DataItem.h"
#include "Converter.h"
#include "CJ188.h"

DataItem::DataItem()
{
	SetData(nullptr, false, 0, true);
}
DataItem::DataItem(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst, CString name){
	SetData(data, hasUnit, decimalPoint, highByteFirst);
	SetName(name);
}


DataItem::~DataItem()
{
	if (rawData != nullptr)
		delete rawData;
}

void DataItem::SetData(UCHAR* raw, int dataLength, bool hasUnit, int decimalPoint, bool highByteFirst, CString name){
	BinData* data = new BinData(raw, dataLength);
	SetData(data, hasUnit, decimalPoint, highByteFirst, name);
}

void DataItem::SetData(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst, CString name){
	if (rawData != nullptr){
		delete rawData;
	}
	rawData = data;
	this->hasUnit = hasUnit;
	this->decimalPoint = decimalPoint;
	SetName(name);
	Parse(highByteFirst);
}

void DataItem::Parse(bool highByteFirst){
	if (rawData == nullptr){
		return;
	}
	int dataLen = rawData->length;
	if (hasUnit){
		dataLen--;
		unit = rawData->data[dataLen];
	}
	if (decimalPoint == 0){//С�����ڵ�0λΪ����
		value.dwv = Converter::BcdToNumber(rawData->data, dataLen, highByteFirst);
	}
	else if(decimalPoint>0){//����0ΪС��
		value.dv = (double)Converter::BcdToNumber(rawData->data, dataLen, highByteFirst);
		for (int i = 0; i < decimalPoint; i++){
			value.dv /= 10.0;
		}
	}
	else{//С��0ΪUCHAR���飬����ֵΪ����
		int length = -decimalPoint;
		value.puc = new UCHAR[length];
		if (highByteFirst){
			memcpy(value.puc, rawData->data, length);
		}
		else{
			for (int i = 0; i < length; i++){
				value.puc[length - i - 1] = rawData->data[i];
			}
		}
	}
}

CString DataItem::ToString(){
	CString str = GetValueStr();
	if (hasUnit)
		return(str + " " + GetUnitStr());
	else
		return str;
}

void DataItem::SetName(CString name){
	this->name = name;
}
CString DataItem::GetName(){
	return name;
}

void DataItem::SetData(UnionValue v, int dec, bool hasUnit, UCHAR unit){
	this->hasUnit = hasUnit;
	value = v;
	this->unit = unit;
	decimalPoint = dec;
}

CString DataItem::GetValueStr(){
	CString str;
	if (decimalPoint >= 0){
		switch (decimalPoint){
		case 0:
			str.Format(L"%d", value.dwv);
			break;
		case 1:
			str.Format(L"%.1f", value.dv);
			break;
		case 2:
			str.Format(L"%.2f", value.dv);
			break;
		case 3:
			str.Format(L"%.3f", value.dv);
			break;
		case 4:
			str.Format(L"%.4f", value.dv);
			break;
		case 5:
			str.Format(L"%.5f", value.dv);
			break;
		case 6:
			str.Format(L"%.6f", value.dv);
			break;
		default:
			str.Format(L"%f", value.dv);
			break;
		}
	}
	else{
		str = Converter::HexToString(value.puc, -decimalPoint);
	}
	return str;

}

double DataItem::GetUpperBound(double rate){
	double value = GetValue();
	double upper = value*(1.0 + rate);
	return upper;
}
double DataItem::GetLowerBound(double rate){
	double value = GetValue();
	double lower = value*(1.0 - rate);
	return lower;
}

//����double[2],����[0]Ϊ�ο��½磬[1]Ϊ�ο��Ͻ硣
double* DataItem::GetBounds(double rate){
	double value = GetValue();
	double* bounds = new double[2];
	bounds[0] = GetLowerBound(rate);
	bounds[1] = GetUpperBound(rate);
	return bounds;
}
CString DataItem::GetItemRangeStr(double rate){
	double* bounds = GetBounds(rate);
	CString str;
	str.Format(L"%.2f~%.2f", bounds[0], bounds[1]);
	delete []bounds;
	return str;
}
double DataItem::GetValue(){
	if (decimalPoint > 0){
		return value.dv;
	}
	else{
		return value.dwv;
	}
}

//������ο���refItem�Աȣ�rateΪ�����ʣ�����1(����)��0(�ϸ�)��-1(����)�����ұ�����
CString DataItem::VerifyWith(DataItem* refItem, double rate){
	double value = GetValue();
	double upper = refItem->GetUpperBound(rate);
	double lower = refItem->GetLowerBound(rate);
	if (value > upper){
		//return 1;
		return L"��";
	}
	if (value < lower){
		//return -1;
		return L"��";
	}
	//return 0;
	return L"-";
}

CString DataItem::GetUnitStr(){
	return CJ188::GetUnit(unit);
}