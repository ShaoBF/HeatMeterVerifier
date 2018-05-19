#include "stdafx.h"
#include "DataAccessFactory.h"
#include "DataBaseAccess.h"
#include "MeterReportAccess.h"
#include "MeterReportDBAccess.h"

CDataAccessFactory::CDataAccessFactory()
{
}


CDataAccessFactory::~CDataAccessFactory()
{
}

CMeterReportAccess* CDataAccessFactory::GetMeterReportAccess(char* accessType){
	if (strcmp(accessType, "DataBase") == 0){
		CMeterReportDBAccess* access = new CMeterReportDBAccess();
		return new CMeterReportDBAccess();
	}
	return nullptr;
}
