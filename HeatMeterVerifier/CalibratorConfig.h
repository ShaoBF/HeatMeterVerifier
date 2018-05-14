#pragma once
#include "MainFrm.h"


class CalibratorConfig
{
public:
	CalibratorConfig(CString configPath);
	CalibratorConfig(CMainFrame* main, CString configPath);
	virtual ~CalibratorConfig();


	virtual BOOL SaveConfig(CString configPath) = 0;
	virtual BOOL ReadConfig(CString configPath) = 0;
	BOOL SaveConfig();
	BOOL ReadConfig();

protected:
	CString configPath;

};

