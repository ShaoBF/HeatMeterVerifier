#pragma once

#define MAX_BUFFER						2048


class ComBuffer
{
public:
	ComBuffer(DWORD max = MAX_BUFFER);
	virtual ~ComBuffer();
	void Append(UCHAR* buf, DWORD bufferLen);
	void Clean(DWORD bufferEnd);
	bool IsEmpty();
	DWORD GetCurrentLength();
	UCHAR* GetData();

protected:
	UCHAR* buffer;
	DWORD maxLength;
	DWORD curLength;
	CMutex* g_clsMutex;
};

