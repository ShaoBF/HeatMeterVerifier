#include "stdafx.h"
#include "ComConfig.h"


ComConfig::ComConfig()
{
}


ComConfig::~ComConfig()
{
}

int ComConfig::GetBaudRateIndex(){
	int index = 0;
	int br = baudRate / BAUD_RATE_BASE;
	while (br != 1){
		index++;
		br /= 2;
	}
	return index;
}

int ComConfig::GetDataBitsIndex(){
	return 8 - dataBits;
}

int ComConfig::GetStopBitsIndex(){
	return stopBits - 1;
}

int ComConfig::GetParityIndex(){
	return parityIndex;
}