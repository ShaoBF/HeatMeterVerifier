#pragma once

#include "CJ188.h"
#include "DataItem.h"


class DataFrame
{
public:
	DataFrame();
	DataFrame(CJ188Frame* frame);
	DataFrame(CJ188Frame* frame, UCHAR* rawData, DWORD rawDataLength);
	virtual ~DataFrame();


public:
	CJ188Frame* frame;

	UCHAR* rawData;
	DWORD rawDataLength;

	//结算日热量
	DataItem billingDayHeat;
	//double currentHeat;				//当前热量
	//UCHAR currentHeatUnit;			//当前热量单位
	DataItem currentHeat;
	//double heatPower;				//热功率
	//UCHAR heatPowerUnit;			//热功率单位
	DataItem heatPower;
	//double flowRate;				//流速
	//UCHAR flowRateUnit;				//流速单位
	DataItem flowRate;
	//double billingDayCapacity;		//结算日累计流量
	//UCHAR billingDayCapacityUnit;	//结算日累计流量单位
	DataItem billingDayCapacity;
	DataItem temperatureIn;			//进水温度
	DataItem temperatureOut;			//出水温度
	DataItem totalWorkHours;			//累计工作时间

	//当前时间
	//UCHAR currentTime[7];
	DataItem currentTime;

	DataItem statusData;
	//UCHAR status[2];

	//tm currentTime;//以后可能考虑用tm结构

	/*
	struct tm {
		int tm_sec;   // 秒，正常范围从 0 到 59，但允许至 61
		int tm_min;   // 分，范围从 0 到 59
		int tm_hour;  // 小时，范围从 0 到 23
		int tm_mday;  // 一月中的第几天，范围从 1 到 31
		int tm_mon;   // 月，范围从 0 到 11
		int tm_year;  // 自 1900 年起的年数
		int tm_wday;  // 一周中的第几天，范围从 0 到 6，从星期日算起
		int tm_yday;  // 一年中的第几天，范围从 0 到 365，从 1 月 1 日算起
		int tm_isdst; // 夏令时
	}
	*/

	int readingDate;
public:
	void ParseData();
	CString GetUnit(UCHAR code);
	CString GetAddressStr();
	CString GetBillingDayHeatStr();
	CString GetCurrentHeatStr();
	CString GetHeatPowerStr();
	CString GetFlowRateStr();
	CString GetBillingDayCapacityStr();
	CString GetTemperatureInStr();
	CString GetTemperatureOutStr();
	CString GetWorkHoursStr();
	CString GetCurrentTimeStr();
	CString GetStatusStr();
};

