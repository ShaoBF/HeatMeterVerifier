#pragma once
#include <windows.h>
//#include "CJ188.h"

class CJ188;
struct CJ188Frame;

class ComDataReciever{
public:
	virtual void OnDataRecieved(UCHAR* data, DWORD bufferLen) = 0;
};

class CJ188DataReciever{
public:
	virtual void OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188) = 0;

};

