#pragma once
#include "MeterReport.h"
#include "CalibratorInfo.h"
#include "MeterDataInfo.h"

class CalibrationReport
{
public:
	CalibrationReport(CString reportTemplate);
	CalibrationReport(CString reportTemplate, MeterReport* meterReport, CCalibratorInfo* calibratorInfo);
	virtual ~CalibrationReport();

	MeterReport* meterReport;
	MeterDataInfo* refMeter;
	CCalibratorInfo* calibratorInfo;
	CString reportTemplate;

	void SetMeterReport(MeterReport* meterReport);
	void SetCalibratorInfo(CCalibratorInfo* calibratorInfo);
	void SetReportTemplate(CString reportTemplate);
	void SetRefMeter(MeterDataInfo* refMeterData);

	CMap<CString, LPCTSTR, CString, LPCTSTR>* GetDataMap();

	CString GetContent();
};

