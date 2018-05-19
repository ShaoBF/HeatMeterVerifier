#pragma once
#include "MeterDataAccess.h"
class CMeterDataDBAccess :
	public CMeterDataAccess
{
public:
	CMeterDataDBAccess();
	virtual ~CMeterDataDBAccess();

	BOOL Save(void* data);
	BOOL Load(void* data);
	BOOL Open(LPCTSTR src);
	BOOL Close();
};

