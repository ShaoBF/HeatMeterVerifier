#pragma once

#include "MeterReport.h"

class CReportGenerator
{
public:
	CReportGenerator();
	virtual ~CReportGenerator();

	//CString GenerateReport(CString templateContent, MeterReport* meterReport);
	CString GenerateReport(CString templateContent, CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap);
};

