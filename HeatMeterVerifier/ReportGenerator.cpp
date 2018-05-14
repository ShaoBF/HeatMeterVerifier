#include "stdafx.h"
#include "ReportGenerator.h"


CReportGenerator::CReportGenerator()
{
}


CReportGenerator::~CReportGenerator()
{
}

CString CReportGenerator::GenerateReport(CString templateContent, CMap<WCHAR *, WCHAR *, CString, CString>* dataMap){
	//
	CString reportContent = templateContent;
	return reportContent;
}
