#pragma once

#include "MeterReport.h"

class CReportGenerator
{
public:
	CReportGenerator();
	virtual ~CReportGenerator();

	//CString GenerateReport(CString templateContent, MeterReport* meterReport);
	//CString GenerateReport(CString templateContent, CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap);
	//virtual void* GenerateReport(void* templateContent, CMap<CString, LPCTSTR, void*, void*>* dataMap) = 0;
	virtual void* GenerateReport(void* templateContent, CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap) = 0;
};

