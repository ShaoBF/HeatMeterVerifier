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
	if (decimalPoint == 0){//小数点在第0位为整数
		value.dwv = Converter::BcdToNumber(rawData->data, dataLen, highByteFirst);
	}
	else if(decimalPoint>0){//大于0为小数
		value.dv = (double)Converter::BcdToNumber(rawData->data, dataLen, highByteFirst);
		for (int i = 0; i < decimalPoint; i++){
			value.dv /= 10.0;
		}
	}
	else{//小于0为UCHAR数组，绝对值为长度
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
	if (hasUnit)
		return(str + " " + CJ188::GetUnit(unit));
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
