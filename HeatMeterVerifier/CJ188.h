#pragma once
#include "MeterInfo.h"
#include "ComDataReciever.h"
#include <mutex>

enum MeterType {
	ColdWater = 0x10,
	LifeHotWater,
	DrinkWater,
	ReclaimedWater,
	HeatMeter = 0x20,
	CoolMeter,
	GasMeter = 0x30,
	MiscMeters = 0x40
};
#define CONTROL_MASK 0x3F
enum CJ188ControlCode{
	CJ188Reserved		= 0x00,
	CJ188ReadData		= 0x01,
	CJ188ReadAddress	= 0x03,
	CJ188WriteData		= 0x04,
	CJ188ReadKeyVersion = 0x09,
	CJ188WriteAddress	= 0x15,
	CJ188WriteMeterBase = 0x16,
	CJ188CustomBegin	= 0x20
};

#define CJ188_RESPONSE_BIT 0x80

enum CJ188DI{
	//以下为读请求
	CJ188ReadCurrentData	= 0x901F,//读取当前数据
	CJ188ReadHistory1		= 0xD120,//读取历史第N月前的数据
	CJ188ReadHistory2,
	CJ188ReadHistory3,
	CJ188ReadHistory4,
	CJ188ReadHistory5,
	CJ188ReadHistory6,
	CJ188ReadHistory7,
	CJ188ReadHistory8,
	CJ188ReadHistory9,
	CJ188ReadHistory10,
	CJ188ReadHistory11,
	CJ188ReadHistory12,
	CJ188ReadPrice			= 0x8102,
	CJ188ReadBillingDay,
	CJ188ReadReadingDay,
	CJ188ReadPurchasedAmount,
	CJ188ReadKeyVersionDI,
	CJ188ReadCurrendAddress	= 0x810A,
	//以下为写请求
	CJ188WritePrice			= 0xA010,
	CJ188WriteBillingDay,
	CJ188WriteReadingDay,
	CJ188WritePurchasedAmount,
	CJ188WriteKey,
	CJ188WriteStandardTime,
	CJ188WriteMeterBaseData,//写机电同步数据
	CJ188WriteValveControl	= 0xA017,
	CJ188WriteCurrentAddress,
	CJ188WriteEnabled
};


#define MAXCOM 256


#define CONTROL 0
#define ACK		1
#define CJ188_START_BYTE	0x68
#define CJ188_END_BYTE		0x16

#define CJ188_ADDRESS_LENGTH 7
#define CJ188_FRAME_LENGTH_BEFORE_DATA 11
#define CJ188_FRAME_LENGTH_AFTER_DATA 2

#define CJ188_CONTROL_CODE_COUNT		8

#define CJ188_READ_DATA_DI_COUNT		17
#define CJ188_WRITE_DATA_DI_COUNT		8
#define CJ188_READ_ADDRESS_DI_COUNT		1
#define CJ188_WRITE_ADDRESS_DI_COUNT	1
#define CJ188_READ_VER_DI_COUNT			1
#define CJ188_WRITE_BASE_DI_COUNT		1

#define CJ188_FRAME_INIT_LENGTH			6

#define CJ188_READ_DATA_DI_OFFSET		0
#define CJ188_READ_VER_DI_OFFSET		17
#define CJ188_READ_ADDRESS_DI_OFFSET	18
#define CJ188_WRITE_DATA_DI_OFFSET		19
#define CJ188_WRITE_ADDRESS_DI_OFFSET	27
#define CJ188_WRITE_BASE_DI_OFFSET		28

#define CJ188_FRAME_PREFIX_BYTE			0xfe

#define MAX_BUFFER						2048


struct KeyValue{
	char* value;
	UINT16 key;
};


struct CJ188Frame{
	UCHAR start;		// 起始字节，一般为68H
	UCHAR meterType;	// 表计类型，详见enum MeterType
	UCHAR address[7];	// 地址域为7字节组成（A0~A6），每个字节2位BCD码，低字节在前，高字节在后，A5A6为厂商代码
	// 地址域为AAAAAAAAAAAAAAH时，为广播地址，用于点对点通信。
	UCHAR controlCode;		// 8位（D7~D0）：
	//		D7传送方向（0：主→从指令，1：从→主应答）
	//		D6通讯状态（0：正常，1：异常）
	//		D5~D0详见enum CJ188ControlCode
	UCHAR dataLength;	//	数据长度为数据域的字节数，读数据时≤64H，写数据时≤32H，0为无数据域
	UCHAR* data;		// 数据域，包括数据标识、序列号和数据，随控制码变化
	UCHAR cs;			// 校验码，一字节，值为从帧起始字节到校验码之前所有字节相加，取结果低八位
	UCHAR end;			// 结束字节，一般为16H
};

struct CJ188FrameInBuffer :public CJ188Frame {
	//CJ188Frame *frame;
	DWORD bufferStart;	//该帧的起始字节在buffer中的下标
	DWORD bufferEnd;	//该帧的结束字节在buffer中的下标
};


class CJ188:public ComDataReciever
{
public:
	CJ188(MeterInfo* info);
	virtual ~CJ188();
	static UCHAR CreateCS(CJ188Frame &frame);
	static void ToHexCommand(char* command, CJ188Frame *frame, int &length, int prefixLen = CJ188_FRAME_INIT_LENGTH);
	void ReadAddress(MeterInfo* meterInfo, CJ188DataReciever* reciever);
	void ReadMeterData(MeterInfo* meterInfo, CJ188DataReciever* reciever);
	static CJ188Frame* CreateRequestFrame(UCHAR* addr, UCHAR control, UCHAR* di);
	static UCHAR* CreateData(UCHAR* di, int dataLength);
	static UCHAR NextSerial();
	void OnDataRecieved(UCHAR* buf, DWORD bufferLen);
	static CJ188Frame* RawDataToFrame(UCHAR* buf, DWORD bufferLen);
	static bool IsValidFrame(CJ188Frame *frame);
	void SendFrame(CJ188Frame* frame);
	void AppendToBuffer(UCHAR* buf, DWORD bufferLen);
	void CleanBuffer(DWORD bufferEnd);
	bool IsBufferEmpty();

//protected:
	//CJ188FrameInBuffer *frameIB;
	static UCHAR ser;
	UCHAR* buffer;
	DWORD bufferCurrent;
	CJ188DataReciever* cjReciever;
	ComDataReciever* cdReciever;
	MeterInfo* meterInfo;
	// MFC互斥类对象
	CMutex* g_clsMutex;
};

