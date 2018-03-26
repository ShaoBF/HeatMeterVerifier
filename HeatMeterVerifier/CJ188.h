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
	//����Ϊ������
	CJ188ReadCurrentData	= 0x901F,//��ȡ��ǰ����
	CJ188ReadHistory1		= 0xD120,//��ȡ��ʷ��N��ǰ������
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
	//����Ϊд����
	CJ188WritePrice			= 0xA010,
	CJ188WriteBillingDay,
	CJ188WriteReadingDay,
	CJ188WritePurchasedAmount,
	CJ188WriteKey,
	CJ188WriteStandardTime,
	CJ188WriteMeterBaseData,//д����ͬ������
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
	UCHAR start;		// ��ʼ�ֽڣ�һ��Ϊ68H
	UCHAR meterType;	// ������ͣ����enum MeterType
	UCHAR address[7];	// ��ַ��Ϊ7�ֽ���ɣ�A0~A6����ÿ���ֽ�2λBCD�룬���ֽ���ǰ�����ֽ��ں�A5A6Ϊ���̴���
	// ��ַ��ΪAAAAAAAAAAAAAAHʱ��Ϊ�㲥��ַ�����ڵ�Ե�ͨ�š�
	UCHAR controlCode;		// 8λ��D7~D0����
	//		D7���ͷ���0��������ָ�1���ӡ���Ӧ��
	//		D6ͨѶ״̬��0��������1���쳣��
	//		D5~D0���enum CJ188ControlCode
	UCHAR dataLength;	//	���ݳ���Ϊ��������ֽ�����������ʱ��64H��д����ʱ��32H��0Ϊ��������
	UCHAR* data;		// �����򣬰������ݱ�ʶ�����кź����ݣ��������仯
	UCHAR cs;			// У���룬һ�ֽڣ�ֵΪ��֡��ʼ�ֽڵ�У����֮ǰ�����ֽ���ӣ�ȡ����Ͱ�λ
	UCHAR end;			// �����ֽڣ�һ��Ϊ16H
};

struct CJ188FrameInBuffer :public CJ188Frame {
	//CJ188Frame *frame;
	DWORD bufferStart;	//��֡����ʼ�ֽ���buffer�е��±�
	DWORD bufferEnd;	//��֡�Ľ����ֽ���buffer�е��±�
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
	// MFC���������
	CMutex* g_clsMutex;
};

