#pragma once
#include "MainFrm.h"
#include "CalibratorConfig.h"


class CDBConfig :public CalibratorConfig
{
public:
	CDBConfig(CMainFrame* main, CString configPath);
	CDBConfig(CString configPath);
	virtual ~CDBConfig();

	//[DataBaseConfig]
	//type = ODBC
	//DSN = HeatMeterDS32
	//UID = ShaoBF
	//PWD = shbofe

	CString type;
	CString dsn;
	CString uid;
	CString pwd;

	BOOL SaveConfig(CString configPath);
	BOOL ReadConfig(CString configPath);
	BOOL SaveConfig();
	BOOL ReadConfig();
	LPCTSTR ToString();
};

