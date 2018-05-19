#pragma once
#include "MeterReportAccess.h"
#include <afxdb.h>
#include "MeterReport.h"

class CMeterReportDBAccess :
	public CMeterReportAccess
{
public:
	CMeterReportDBAccess();
	CMeterReportDBAccess(void* accessor);
	virtual ~CMeterReportDBAccess();

	CDatabase* db;
	LPCTSTR lpszConnect;


	BOOL Save(void* data);
	BOOL Load(void* data);
	BOOL Open(LPCTSTR src);
	BOOL Close();

	UINT64 GetReportID(MeterReport* report);

};

