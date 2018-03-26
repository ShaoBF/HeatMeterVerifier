#pragma once
#include <windows.h>
#include "ComDataReciever.h"

typedef void(*DataRecievedCallBack)(char*);
typedef void(*DataRecievedCB)(void*, char*, DWORD);

class CSerial
{
public:
	CSerial(void);
	~CSerial(void);

	//�򿪴���
	BOOL OpenSerialPort(TCHAR* port, UINT baud_rate, BYTE date_bits, BYTE stop_bit, BYTE parity, 
		//DataRecievedCB dataRecievedCallBack, 
		ComDataReciever* reciever);
	BOOL CloseSerialPort();

	//��������
	BOOL SendData(char* data, int len);
	void DataRecieved(char* buf);
	bool IsOpened();

public:
	HANDLE m_hComm;
	bool opened;
	//DataRecievedCB dataRecievedCallBack;
	ComDataReciever* reciever;
	//void* reciever;
};
