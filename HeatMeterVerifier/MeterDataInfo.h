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
	MeterReport* CalculateReport(MeterReport* refData);

	CString GetAddressStr();
	CString GetHeatStr();
	CString GetHeatPowerStr();
	CString GetCapacityStr();
	CString GetFlowRateStr();
	CString GetTemperatureInStr();
	CString GetTemperatureOutStr();
	CString GetStartTimeStr();
	CString GetEndTimeStr();
	CString GetDurationStr();
	CString GetMeterTypeStr();


protected:
	MeterReport* report;
	DataFrame* startFrame;
	DataFrame* endFrame;

};

