#pragma once
#include "MainFrm.h"

#define BAUD_RATE_BASE 300
#define FULL_FILE_PATH L"D:\\ShaoBF\\IniTest\\ComConfig.ini"
#define LP_APPNAME L"ComConfig"

class ComConfig
{
public:
	ComConfig(CMainFrame* main);
	ComConfig();
	virtual ~ComConfig();

	int parityIndex = EVENPARITY;
	int dataBits = 8;
	int stopBits = 1;
	int baudRate = 2400;

	int GetBaudRateIndex();
	int GetDataBitsIndex();
	int GetStopBitsIndex();
	int GetParityIndex();

	BOOL SaveConfig();
	BOOL ReadConfig();
};

