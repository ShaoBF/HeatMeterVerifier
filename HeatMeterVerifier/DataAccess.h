#pragma once
class CDataAccess
{
public:
	CDataAccess();
	virtual ~CDataAccess();



	virtual BOOL Save(void* data)=0;
	virtual BOOL Load(void* data) = 0;
	virtual BOOL Open(LPCTSTR src) = 0;
	virtual BOOL Close() = 0;

};

