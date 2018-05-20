#pragma once
#include "DataAccess.h"
#include <afxdb.h>

class CDataBaseAccess :
	public CDataAccess
{
public:
	CDataBaseAccess();
	virtual ~CDataBaseAccess();

	CDatabase* db;
	LPCTSTR lpszConnect;

	virtual BOOL Save(void* data) = 0;
	virtual BOOL Load(void* data) = 0;
	virtual BOOL Open(LPCTSTR src);
	virtual BOOL Close();

};

