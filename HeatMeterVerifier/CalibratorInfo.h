#pragma once
#include "CalibratorConfig.h"

class CCalibratorInfo:public CalibratorConfig
{
public:
	CCalibratorInfo(CMainFrame* main, CString configPath);
	CCalibratorInfo(CString configPath);
	virtual ~CCalibratorInfo();

	BOOL SaveConfig(CString configPath);
	BOOL ReadConfig(CString configPath);
	BOOL SaveConfig();
	BOOL ReadConfig();

	CString name;
	CString calibrateMode=L"对比参考值";
	CString tester;
	CString reviewer;
};

