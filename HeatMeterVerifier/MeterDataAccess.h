#pragma once
#include "DataAccess.h"


class CMeterDataAccess :
	public CDataAccess
{
public:
	CMeterDataAccess();
	virtual ~CMeterDataAccess();

	virtual BOOL Save(void* data) = 0;
	virtual BOOL Save(void* data,UINT64 testID) = 0;
	virtual BOOL Load(void* data) = 0;
};

