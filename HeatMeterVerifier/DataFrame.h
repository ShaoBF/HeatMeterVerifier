#pragma once

#include "CJ188.h"
#include "DataItem.h"


class DataFrame
{
public:
	DataFrame();
	DataFrame(CJ188Frame* frame);
	DataFrame(CJ188Frame* frame, UCHAR* rawData, DWORD rawDataLength);
	virtual ~DataFrame();


public:
	CJ188Frame* frame;

	UCHAR* rawData;
	DWORD rawDataLength;

	//����������
	DataItem billingDayHeat;
	//double currentHeat;				//��ǰ����
	//UCHAR currentHeatUnit;			//��ǰ������λ
	DataItem currentHeat;
	//double heatPower;				//�ȹ���
	//UCHAR heatPowerUnit;			//�ȹ��ʵ�λ
	DataItem heatPower;
	//double flowRate;				//����
	//UCHAR flowRateUnit;				//���ٵ�λ
	DataItem flowRate;
	//double billingDayCapacity;		//�������ۼ�����
	//UCHAR billingDayCapacityUnit;	//�������ۼ�������λ
	DataItem billingDayCapacity;
	DataItem temperatureIn;			//��ˮ�¶�
	DataItem temperatureOut;			//��ˮ�¶�
	DataItem totalWorkHours;			//�ۼƹ���ʱ��

	//��ǰʱ��
	//UCHAR currentTime[7];
	DataItem currentTime;

	DataItem statusData;
	//UCHAR status[2];

	//tm currentTime;//�Ժ���ܿ�����tm�ṹ

	/*
	struct tm {
		int tm_sec;   // �룬������Χ�� 0 �� 59���������� 61
		int tm_min;   // �֣���Χ�� 0 �� 59
		int tm_hour;  // Сʱ����Χ�� 0 �� 23
		int tm_mday;  // һ���еĵڼ��죬��Χ�� 1 �� 31
		int tm_mon;   // �£���Χ�� 0 �� 11
		int tm_year;  // �� 1900 ���������
		int tm_wday;  // һ���еĵڼ��죬��Χ�� 0 �� 6��������������
		int tm_yday;  // һ���еĵڼ��죬��Χ�� 0 �� 365���� 1 �� 1 ������
		int tm_isdst; // ����ʱ
	}
	*/

	int readingDate;
public:
	void ParseData();
	CString GetUnit(UCHAR code);
	CString GetAddressStr();
	CString GetBillingDayHeatStr();
	CString GetCurrentHeatStr();
	CString GetHeatPowerStr();
	CString GetFlowRateStr();
	CString GetBillingDayCapacityStr();
	CString GetTemperatureInStr();
	CString GetTemperatureOutStr();
	CString GetWorkHoursStr();
	CString GetCurrentTimeStr();
	CString GetStatusStr();
};

