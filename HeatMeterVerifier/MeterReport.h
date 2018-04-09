#pragma once
#include "DataItem.h"
#include "DataFrame.h"

class MeterReport
{
public:
	MeterReport();
	MeterReport(DataFrame* startFrame, DataFrame* endFrame);
	virtual ~MeterReport();

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

	void Calculate(DataFrame* startFrame,DataFrame* endFrame);
};

