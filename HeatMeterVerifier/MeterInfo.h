#pragma once
#include <string>
#include "CSerial.h"
//#include "CJ188.h"

class CJ188;
class MeterInfo
{
public:
	MeterInfo();
	MeterInfo(CString com, UCHAR* code);
	virtual ~MeterInfo();
	void SetCJ188(CJ188* cj);
	CJ188* GetCJ188();
	CString MeterToString();
	void SetActive(bool tf);
	bool IsActive();
	CString GetAddressString();
	UCHAR* GetAddress();
	CString GetCom();
	void SetAddress(UCHAR* code);

public:
	CString com;
	UCHAR* address;
	bool selected = false;
	bool active = false;
	CSerial serial;
	CJ188 *cj188;
};

