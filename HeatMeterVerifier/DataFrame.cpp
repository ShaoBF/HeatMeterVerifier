#include "stdafx.h"
#include "DataFrame.h"
#include "Converter.h"

#define CELSIUS "℃"

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

void DataFrame::ParseData(){//解析数据，以汇中热能表为基准的原型程序
	//取出frame中的data部分
	UCHAR* dataArea=new UCHAR[frame->dataLength];
	memcpy(dataArea, frame->data,frame->dataLength);
	//按照协议标准拆分计算每段的数值并赋值给对应变量

	//分析厂家号、表类型，根据厂家说明区分高低字节顺序
	UCHAR meterType = frame->meterType;
	UCHAR* vandorID;
	vandorID = &(frame->address[5]);
	bool highByteFirst = CJ188::LookUpByteOrder(meterType,vandorID);

	//前三字节为响应指令部分
	/*UCHAR* refDI=CJ188::GetDI(refFrame);
	bool highByteFirst = true;
	if (refDI[0] == dataArea[1]){

	}*/

	int current = 3;
	
	//5字节结算日热量
	//4字节BCD码数值，XXXXXX.XX格式
	/*billingDayHeat = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	billingDayHeat /= 100.0;
	//1字节单位
	//billingDayHeatUnit = GetUnit(dataArea[current++]);
	billingDayHeatUnit = dataArea[current++];*/

	billingDayHeat.SetData(&(dataArea[current]), 5, true, 2,highByteFirst);
	current += 5;

	//5字节当前热量
	//4字节BCD码数值，XXXXXX.XX格式
	/*currentHeat = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	currentHeat /= 100.0;
	//1字节单位
	//currentHeatUnit = GetUnit(dataArea[current++]);
	currentHeatUnit = dataArea[current++];*/

	currentHeat.SetData(&(dataArea[current]), 5, true, 2, highByteFirst);
	current += 5;


	//5字节热功率
	//4字节BCD码数值，XXXXXX.XX格式
	/*heatPower = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	heatPower /= 100.0;
	//1字节单位
	//heatPowerUnit = GetUnit(dataArea[current++]);
	heatPowerUnit = dataArea[current++];*/

	heatPower.SetData(&(dataArea[current]), 5, true, 2, highByteFirst);
	current += 5;

	//5字节瞬时流速
	//4字节BCD码数值，XXXX.XXXX格式
	/*flowRate = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	flowRate /= 10000.0;
	//1字节单位
	//flowRateUnit = GetUnit(dataArea[current++]);
	flowRateUnit = dataArea[current++];*/

	flowRate.SetData(&(dataArea[current]), 5, true, 4, highByteFirst);
	current += 5;


	//5字节累计流量
	//4字节BCD码数值，XXXXXX.XX格式
	/*billingDayCapacity = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	billingDayCapacity /= 100.0;
	//1字节单位
	//billingDayCapacityUnit = GetUnit(dataArea[current++]);
	billingDayCapacityUnit = dataArea[current++];*/

	billingDayCapacity.SetData(&(dataArea[current]), 5, true, 2, highByteFirst);
	current += 5;

	//3字节供水温度，BCD码数值，XXXX.XX格式，默认单位℃
	/*temperatureIn = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;
	temperatureIn /= 100.0;*/

	temperatureIn.SetData(&(dataArea[current]), 3, false, 2, highByteFirst);
	current += 3;


	//3字节出水温度，BCD码数值，XXXX.XX格式，默认单位℃
	/*temperatureOut = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;
	temperatureOut /= 100.0;*/

	temperatureOut.SetData(&(dataArea[current]), 3, false, 2, highByteFirst);
	current += 3;


	//3字节工作时间，BCD码数值，默认单位小时
	/*totalWorkHours = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;*/

	totalWorkHours.SetData(&(dataArea[current]), 3, false, 0, highByteFirst);
	current += 3;


	//7字节当前时间，BCD码，格式为YYYYMMDDhhmmss，无单位字节
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

	//2字节状态字，第一字节为固定意义，第二字节厂家自定义
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