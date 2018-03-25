#pragma once

#define BAUD_RATE_BASE 300

class ComConfig
{
public:
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
};

