#pragma once
#include "ReportGenerator.h"
class CTextReportGenerator :
	public CReportGenerator
{
public:
	CTextReportGenerator();
	virtual ~CTextReportGenerator();

	void* GenerateReport(void* templateContent, CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap);
};

