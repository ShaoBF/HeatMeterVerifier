#include "stdafx.h"
#include "DataFrame.h"
#include "Converter.h"

#define CELSIUS "��"

DataFrame::DataFrame()
{
	frame = nullptr;
	rawData = nullptr;
	rawDataLength = 0;
}

DataFrame::DataFrame(CJ188Frame* frame)
{
	this->frame = frame;
	rawData = nullptr;
	rawDataLength = 0;
}
DataFrame::DataFrame(CJ188Frame* frame, UCHAR* rawData, DWORD rawDataLength){
	this->frame = frame;
	this->rawDataLength = rawDataLength;
	this->rawData = new UCHAR[rawDataLength];
	memcpy(this->rawData, rawData, rawDataLength);
}


DataFrame::~DataFrame()
{
	if (frame != nullptr){
		delete frame;
	}

}

void DataFrame::ParseData(){//�������ݣ��Ի������ܱ�Ϊ��׼��ԭ�ͳ���
	//ȡ��frame�е�data����
	UCHAR* dataArea=new UCHAR[frame->dataLength];
	memcpy(dataArea, frame->data,frame->dataLength);
	//����Э���׼��ּ���ÿ�ε���ֵ����ֵ����Ӧ����

	//�������Һš������ͣ����ݳ���˵�����ָߵ��ֽ�˳��
	UCHAR meterType = frame->meterType;
	UCHAR* vandorID;
	vandorID = &(frame->address[5]);
	bool highByteFirst = CJ188::LookUpByteOrder(meterType,vandorID);

	//ǰ���ֽ�Ϊ��Ӧָ���
	/*UCHAR* refDI=CJ188::GetDI(refFrame);
	bool highByteFirst = true;
	if (refDI[0] == dataArea[1]){

	}*/

	int current = 3;
	
	//5�ֽڽ���������
	//4�ֽ�BCD����ֵ��XXXXXX.XX��ʽ
	/*billingDayHeat = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	billingDayHeat /= 100.0;
	//1�ֽڵ�λ
	//billingDayHeatUnit = GetUnit(dataArea[current++]);
	billingDayHeatUnit = dataArea[current++];*/

	billingDayHeat.SetData(&(dataArea[current]), 5, true, 2,highByteFirst);
	current += 5;

	//5�ֽڵ�ǰ����
	//4�ֽ�BCD����ֵ��XXXXXX.XX��ʽ
	/*currentHeat = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	currentHeat /= 100.0;
	//1�ֽڵ�λ
	//currentHeatUnit = GetUnit(dataArea[current++]);
	currentHeatUnit = dataArea[current++];*/

	currentHeat.SetData(&(dataArea[current]), 5, true, 2, highByteFirst);
	current += 5;


	//5�ֽ��ȹ���
	//4�ֽ�BCD����ֵ��XXXXXX.XX��ʽ
	/*heatPower = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	heatPower /= 100.0;
	//1�ֽڵ�λ
	//heatPowerUnit = GetUnit(dataArea[current++]);
	heatPowerUnit = dataArea[current++];*/

	heatPower.SetData(&(dataArea[current]), 5, true, 2, highByteFirst);
	current += 5;

	//5�ֽ�˲ʱ����
	//4�ֽ�BCD����ֵ��XXXX.XXXX��ʽ
	/*flowRate = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	flowRate /= 10000.0;
	//1�ֽڵ�λ
	//flowRateUnit = GetUnit(dataArea[current++]);
	flowRateUnit = dataArea[current++];*/

	flowRate.SetData(&(dataArea[current]), 5, true, 4, highByteFirst);
	current += 5;


	//5�ֽ��ۼ�����
	//4�ֽ�BCD����ֵ��XXXXXX.XX��ʽ
	/*billingDayCapacity = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	billingDayCapacity /= 100.0;
	//1�ֽڵ�λ
	//billingDayCapacityUnit = GetUnit(dataArea[current++]);
	billingDayCapacityUnit = dataArea[current++];*/

	billingDayCapacity.SetData(&(dataArea[current]), 5, true, 2, highByteFirst);
	current += 5;

	//3�ֽڹ�ˮ�¶ȣ�BCD����ֵ��XXXX.XX��ʽ��Ĭ�ϵ�λ��
	/*temperatureIn = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;
	temperatureIn /= 100.0;*/

	temperatureIn.SetData(&(dataArea[current]), 3, false, 2, highByteFirst);
	current += 3;


	//3�ֽڳ�ˮ�¶ȣ�BCD����ֵ��XXXX.XX��ʽ��Ĭ�ϵ�λ��
	/*temperatureOut = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;
	temperatureOut /= 100.0;*/

	temperatureOut.SetData(&(dataArea[current]), 3, false, 2, highByteFirst);
	current += 3;


	//3�ֽڹ���ʱ�䣬BCD����ֵ��Ĭ�ϵ�λСʱ
	/*totalWorkHours = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;*/

	totalWorkHours.SetData(&(dataArea[current]), 3, false, 0, highByteFirst);
	current += 3;


	//7�ֽڵ�ǰʱ�䣬BCD�룬��ʽΪYYYYMMDDhhmmss���޵�λ�ֽ�
	//currentTime = Converter::HexToString(&(dataArea[current]), 7, 0);
	if (highByteFirst){
		memcpy(currentTime, &(dataArea[current]), CJ188_TIME_DATA_LENGTH);
	}
	else{
		for (int i = 0; i < CJ188_TIME_DATA_LENGTH; i++){
			currentTime[CJ188_TIME_DATA_LENGTH - i - 1] = dataArea[current + i];
		}
	}
	current += CJ188_TIME_DATA_LENGTH;

	//2�ֽ�״̬�֣���һ�ֽ�Ϊ�̶����壬�ڶ��ֽڳ����Զ���
	if (highByteFirst){
		memcpy(status, &(dataArea[current]), 2);
	}
	else{
		for (int i = 0; i < 2; i++){
			status[2 - i - 1] = dataArea[current + i];
		}
	}
	current += 2;
}


CString DataFrame::GetAddressStr(){
	return Converter::HexToString(frame->address, 7, 0);
}

CString DataFrame::GetBillingDayHeatStr(){
	return billingDayHeat.ToString();
}

CString DataFrame::GetCurrentHeatStr(){
	return currentHeat.ToString();
}

CString DataFrame::GetHeatPowerStr(){
	return heatPower.ToString();
}

CString DataFrame::GetFlowRateStr(){
	return flowRate.ToString();
}

CString DataFrame::GetBillingDayCapacityStr(){
	return billingDayCapacity.ToString();
}

CString DataFrame::GetTemperatureInStr(){
	return temperatureIn.ToString() + " " + CELSIUS;
}

CString DataFrame::GetTemperatureOutStr(){
	return temperatureOut.ToString() + " " + CELSIUS;
}

CString DataFrame::GetWorkHoursStr(){
	return totalWorkHours.ToString()+" h";
}

CString DataFrame::GetCurrentTimeStr(){
	return Converter::HexToString(currentTime, 7, 0);
}

CString DataFrame::GetStatusStr(){
	return Converter::HexToString(status, 2, ' ');
}