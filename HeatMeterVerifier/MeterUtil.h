#pragma once
#include "MeterInfo.h"
#include <vector>

using namespace std;
class MeterUtil
{
public:
	MeterUtil();
	virtual ~MeterUtil();

	static void ReadMetersID(vector<MeterInfo*>* meterInfoList);

};

