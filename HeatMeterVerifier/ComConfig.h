#pragma once
#include "MainFrm.h"
#include "CalibratorConfig.h"

#define BAUD_RATE_BASE 300

class ComConfig :public CalibratorConfig
{
public:
	ComConfig(CMainFrame* main,CString configPath);
	ComConfig(CString configPath);
	virtual ~ComConfig();

	int parityIndex = EVENPARITY;
	int dataBits = 8;
	int stopBits = 1;
	int baudRate = 2400;

	int GetBaudRateIndex();
	int GetDataBitsIndex();
	int GetStopBitsIndex();
	int GetParityIndex();

	BOOL SaveConfig(CString configPath);
	BOOL ReadConfig(CString configPath);
	BOOL SaveConfig();
	BOOL ReadConfig();

};

