#pragma once
#include <string>
#include "CSerial.h"
//#include "CJ188.h"

class CJ188;
class MeterInfo
{
public:
	MeterInfo();
	MeterInfo(CString com, CString code=L"");
	virtual ~MeterInfo();
	void setCJ188(CJ188* cj);
	CString MeterToString();
	void SetActive(bool tf);
	bool IsActive();
	CString GetCode();
	CString GetCom();

public:
	CString com;
	CString code;
	bool selected = false;
	bool active = false;
	CSerial serial;
	CJ188 *cj188;
};

