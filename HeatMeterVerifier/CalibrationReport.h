#pragma once
#include "MeterReport.h"
#include "CalibratorInfo.h"

class CalibrationReport
{
public:
	CalibrationReport(CString reportTemplate);
	CalibrationReport(CString reportTemplate, MeterReport* meterReport, CCalibratorInfo* calibratorInfo);
	virtual ~CalibrationReport();

	MeterReport* meterReport;
	CCalibratorInfo* calibratorInfo;
	CString reportTemplate;

	void SetMeterReport(MeterReport* meterReport);
	void SetCalibratorInfo(CCalibratorInfo* calibratorInfo);
	void SetReportTemplate(CString reportTemplate);

	CMap<WCHAR *, WCHAR *, CString, CString>* GetDataMap();

	CString GetContent();
};

