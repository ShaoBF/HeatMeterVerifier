#pragma once
#include "MeterDataAccess.h"
#include "DataBaseAccess.h"

class CMeterDataDBAccess :
	public CMeterDataAccess,public CDataBaseAccess
{
public:
	CMeterDataDBAccess();
	virtual ~CMeterDataDBAccess();

	BOOL Save(void* data, UINT64 testID);
	BOOL Save(void* data);
	BOOL Load(void* data);
	BOOL Open(LPCTSTR src);
	BOOL Close();
};

