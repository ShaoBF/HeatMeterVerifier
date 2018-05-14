#pragma once

#include "MeterReport.h"

class CReportGenerator
{
public:
	CReportGenerator();
	virtual ~CReportGenerator();

	CString GenerateReport(CString templateContent, CMap<WCHAR *, WCHAR *, CString, CString>* dataMap);
};

