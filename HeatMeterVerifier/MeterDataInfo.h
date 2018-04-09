#pragma once
#include "MeterInfo.h"
#include "MeterReport.h"
#include "DataFrame.h"

class MeterDataInfo :
	public MeterInfo
{
public:
	MeterDataInfo();
	MeterDataInfo(CString com, UCHAR* code);
	virtual ~MeterDataInfo();

	void SetStartFrame(DataFrame* frame);
	void SetEndFrame(DataFrame* frame);

	DataFrame* GetStartFrame();
	DataFrame* GetEndFrame();
	MeterReport* GetReport();

protected:
	MeterReport* report;
	DataFrame* startFrame;
	DataFrame* endFrame;
};

