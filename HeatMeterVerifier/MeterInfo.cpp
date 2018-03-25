#include "stdafx.h"
#include "MeterInfo.h"


MeterInfo::MeterInfo()
{
	com = L"";
	code = L"";
}


MeterInfo::~MeterInfo()
{
}

MeterInfo::MeterInfo(CString com, CString code){
	this->com = com;
	this->code = code;
}

void MeterInfo::setCJ188(CJ188* cj){
	cj188 = cj;
}

CString MeterInfo::MeterToString(){
	CString meterStr = code + "(" + com + ")";
	return meterStr;
}

void MeterInfo::SetActive(bool tf){
	active = tf;
}

bool MeterInfo::IsActive(){
	return active;
}

CString MeterInfo::GetCode(){
	return code;
}

CString MeterInfo::GetCom(){
	return com;
}
