#pragma once
#include "MeterReportAccess.h"
#include <afxdb.h>
#include "MeterReport.h"
#include "DataBaseAccess.h"

class CMeterReportDBAccess :
	public CMeterReportAccess,public CDataBaseAccess
{
public:
	CMeterReportDBAccess();
	virtual ~CMeterReportDBAccess();

	//CDatabase* db;
	//LPCTSTR lpszConnect;


	BOOL Save(void* data);
	BOOL Load(void* data);
	BOOL Open(LPCTSTR src);
	BOOL Close();

	UINT64 GetReportID(MeterReport* report);

};

