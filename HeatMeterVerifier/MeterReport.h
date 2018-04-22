#pragma once
#include "DataItem.h"
#include "DataFrame.h"

class MeterReport
{
public:
	MeterReport();
	MeterReport(DataFrame* startFrame, DataFrame* endFrame);
	virtual ~MeterReport();

	bool highByteFirst = true;

	DataFrame* startFrame, *endFrame;

	MeterInfo* meter;
	//����ID
	UINT64 id;
	//���
	UCHAR address[7];
	//�涨ʱ����������
	DataItem heat;
	//�ȹ���
	DataItem heatPower;
	//�涨ʱ����������
	DataItem capacity;
	//����
	DataItem flowRate;
	//��ˮ�¶�
	DataItem temperatureIn;
	//��ˮ�¶�
	DataItem temperatureOut;
	//��ʼʱ��
	DataItem startTime;
	//����ʱ��
	DataItem endTime;
	//����ʱ��
	DataItem duration;
	bool isQualified;

	void Calculate(DataFrame* startFrame,DataFrame* endFrame);
	void CalculateQualified(MeterReport* refData);

	CString GetAddressStr();
	CString GetHeatStr();
	CString GetHeatPowerStr();
	CString GetCapacityStr();
	CString GetFlowRateStr();
	CString GetTemperatureInStr();
	CString GetTemperatureOutStr();
	CString GetStartTimeStr();
	CString GetEndTimeStr();
	CString GetDurationStr();
	CString GetQualifiedStr();

};

