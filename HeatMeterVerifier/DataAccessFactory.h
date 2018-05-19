#pragma once
#include "MeterReportAccess.h"

class CDataAccessFactory
{
public:
	CDataAccessFactory();
	virtual ~CDataAccessFactory();

	static CMeterReportAccess* GetMeterReportAccess(char* accessType);
};

