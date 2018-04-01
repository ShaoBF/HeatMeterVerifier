#include "stdafx.h"
#include "ComBuffer.h"


ComBuffer::ComBuffer(DWORD max)
{
	maxLength = max;
	curLength = 0;
	g_clsMutex = new CMutex(FALSE, L"DataBufferMutex", NULL);
	buffer = new UCHAR[maxLength];
}


ComBuffer::~ComBuffer()
{
	if (buffer != nullptr){
		delete buffer;
	}
	if (g_clsMutex){
		delete g_clsMutex;
	}
}
void ComBuffer::Append(UCHAR* buf, DWORD bufLen){
	g_clsMutex->Lock();
	for (int i = 0; i < bufLen; i++){
		buffer[curLength++] = buf[i];
	}
	g_clsMutex->Unlock();
}

void ComBuffer::Clean(DWORD bufferEnd){
	DWORD tempCur = 0;
	g_clsMutex->Lock();
	for (DWORD i = bufferEnd; i < curLength; i++){
		buffer[tempCur++] = buffer[i];
	}
	curLength = tempCur;
	g_clsMutex->Unlock();
}
bool ComBuffer::IsEmpty(){
	return curLength == 0;
}

DWORD ComBuffer::GetCurrentLength(){
	return curLength;
}

UCHAR* ComBuffer::GetData(){
	return buffer;
}