#include "stdafx.h"
#include "DataAccessFactory.h"
#include "DataBaseAccess.h"
#include "MeterReportDBAccess.h"
#include "MeterDataDBAccess.h"

CDataAccessFactory::CDataAccessFactory()
{
}


CDataAccessFactory::~CDataAccessFactory()
{
}

CMeterReportAccess* CDataAccessFactory::GetMeterReportAccess(char* accessType){
	if (strcmp(accessType, "DataBase") == 0){
		CMeterReportDBAccess* access = new CMeterReportDBAccess();
		return access;
	}
	return nullptr;
}
CMeterDataAccess* CDataAccessFactory::GetMeterDataAccess(char* accessType){
	if (strcmp(accessType, "DataBase") == 0){
		CMeterDataDBAccess* access = new CMeterDataDBAccess();
		return access;
	}
	return nullptr;
}
