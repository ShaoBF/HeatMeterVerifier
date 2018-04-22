#pragma once
#include "DataItem.h"
#include "DataFrame.h"

class MeterReport
{
public:
	MeterReport();
	MeterReport(DataFrame* startFrame, DataFrame* endFrame);
	virtual ~MeterReport();

	bool highByteFirst = true;

	DataFrame* startFrame, *endFrame;

	MeterInfo* meter;
	//报告ID
	UINT64 id;
	//表号
	UCHAR address[7];
	//规定时间内总热量
	DataItem heat;
	//热功率
	DataItem heatPower;
	//规定时间内总流量
	DataItem capacity;
	//流速
	DataItem flowRate;
	//进水温度
	DataItem temperatureIn;
	//出水温度
	DataItem temperatureOut;
	//开始时间
	DataItem startTime;
	//结束时间
	DataItem endTime;
	//持续时间
	DataItem duration;
	bool isQualified;

	void Calculate(DataFrame* startFrame,DataFrame* endFrame);
	void CalculateQualified(MeterReport* refData);

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
	CString GetQualifiedStr();

};

