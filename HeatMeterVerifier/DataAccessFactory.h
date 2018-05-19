#pragma once
#include "MeterReportAccess.h"
#include "MeterDataAccess.h"

class CDataAccessFactory
{
public:
	CDataAccessFactory();
	virtual ~CDataAccessFactory();

	static CMeterReportAccess* GetMeterReportAccess(char* accessType);
	static CMeterDataAccess* GetMeterDataAccess(char* accessType);
};

