#include "stdafx.h"
#include "DataItem.h"
#include "Converter.h"
#include "CJ188.h"

DataItem::DataItem()
{
	SetData(nullptr, false, 0, true);
}
DataItem::DataItem(BinData* data, bool hasUnit, int decimalPoint, bool highByteFirst){
	SetData(data, hasUnit, decimalPoint, highByteFirst);
}


DataItem::~DataItem()
{
	if (rawData != nullptr)
		delete rawData;
}

void DataItem::SetData(UCHAR* raw, int dataLength, bool hasUnit, int decimalPoint, bool highByteFirst){
	BinData* data = new BinData(raw, dataLength);
	SetData(data, hasUnit, decimalPoint, highByteFirst);
}

void DataItem::SetData(BinData* data, bool hasUnit, int decimalPoint,bool highByteFirst){
	if (rawData != nullptr){
		delete rawData;
	}
	rawData = data;
	this->hasUnit = hasUnit;
	this->decimalPoint = decimalPoint;
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
	if (decimalPoint == 0){//小数点在第0位之后为整数
		value.dwv = Converter::BcdToNumber(rawData->data, dataLen, highByteFirst);
	}
	else{//否则为小数
		value.dv = (double)Converter::BcdToNumber(rawData->data, dataLen, highByteFirst);
		for (int i = 0; i < decimalPoint; i++){
			value.dv /= 10.0;
		}
	}
}

CString DataItem::ToString(){
	CString str;
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
	if (hasUnit)
		return(str + " " + CJ188::GetUnit(unit));
	else
		return str;
}