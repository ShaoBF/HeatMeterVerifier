#include "stdafx.h"
#include "MeterInfo.h"
#include "Converter.h"
#include "CJ188.h"

MeterInfo::MeterInfo()
{
	com = L"";
}


MeterInfo::~MeterInfo()
{
}

MeterInfo::MeterInfo(CString com, UCHAR* code){
	this->com = com;
	this->address = code;
}

void MeterInfo::SetCJ188(CJ188* cj){
	cj188 = cj;
}

CJ188* MeterInfo::GetCJ188(){
	if (cj188 == nullptr){
		cj188 = new CJ188(this);
	}
	return cj188;
}

CString MeterInfo::MeterToString(){
	CString meterStr = GetAddressString() + "(" + com + ")";
	return meterStr;
}

void MeterInfo::SetActive(bool tf){
	active = tf;
}

bool MeterInfo::IsActive(){
	return active;
}

CString MeterInfo::GetAddressString(){
	return Converter::HexToString(address, 7, 0);
}

UCHAR* MeterInfo::GetAddress(){
	return address;
}

CString MeterInfo::GetCom(){
	return com;
}

void MeterInfo::SetAddress(UCHAR* code){
	this->address = new UCHAR[7];
	memcpy(this->address, code, 7);
}

