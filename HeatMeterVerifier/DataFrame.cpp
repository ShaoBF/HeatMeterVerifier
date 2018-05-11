#include "stdafx.h"
#include "DataFrame.h"
#include "Converter.h"
#include "MeterWizard.h"

extern CMeterWizard wizard;

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
	bool highByteFirst = frame->highFirst;//!wizard.isLowByteFirst();//CJ188::LookUpByteOrder(meterType,vandorID);

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

	billingDayHeat.SetData(&(dataArea[current]), 5, true, 2, highByteFirst, _T(NAME_BILLING_DAY_HEAT));
	current += 5;

	//5字节当前热量
	//4字节BCD码数值，XXXXXX.XX格式
	/*currentHeat = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	currentHeat /= 100.0;
	//1字节单位
	//currentHeatUnit = GetUnit(dataArea[current++]);
	currentHeatUnit = dataArea[current++];*/

	currentHeat.SetData(&(dataArea[current]), 5, true, 2, highByteFirst, _T(NAME_CURRENT_HEAT));
	current += 5;


	//5字节热功率
	//4字节BCD码数值，XXXXXX.XX格式
	/*heatPower = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	heatPower /= 100.0;
	//1字节单位
	//heatPowerUnit = GetUnit(dataArea[current++]);
	heatPowerUnit = dataArea[current++];*/

	heatPower.SetData(&(dataArea[current]), 5, true, 2, highByteFirst, _T(NAME_HEAT_POWER));
	current += 5;

	//5字节瞬时流速
	//4字节BCD码数值，XXXX.XXXX格式
	/*flowRate = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	flowRate /= 10000.0;
	//1字节单位
	//flowRateUnit = GetUnit(dataArea[current++]);
	flowRateUnit = dataArea[current++];*/

	flowRate.SetData(&(dataArea[current]), 5, true, 4, highByteFirst, _T(NAME_FLOW_RATE));
	current += 5;


	//5字节累计流量
	//4字节BCD码数值，XXXXXX.XX格式
	/*billingDayCapacity = (double)Converter::BcdToNumber(&(dataArea[current]), 4);
	current += 4;
	billingDayCapacity /= 100.0;
	//1字节单位
	//billingDayCapacityUnit = GetUnit(dataArea[current++]);
	billingDayCapacityUnit = dataArea[current++];*/

	billingDayCapacity.SetData(&(dataArea[current]), 5, true, 2, highByteFirst, _T(NAME_BILLING_DAY_CAPACITY));
	current += 5;

	//3字节供水温度，BCD码数值，XXXX.XX格式，默认单位℃
	/*temperatureIn = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;
	temperatureIn /= 100.0;*/

	temperatureIn.SetData(&(dataArea[current]), 3, false, 2, highByteFirst, _T(NAME_TEMPERATURE_IN));
	current += 3;


	//3字节出水温度，BCD码数值，XXXX.XX格式，默认单位℃
	/*temperatureOut = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;
	temperatureOut /= 100.0;*/

	temperatureOut.SetData(&(dataArea[current]), 3, false, 2, highByteFirst, _T(NAME_TEMPERATURE_OUT));
	current += 3;


	//3字节工作时间，BCD码数值，默认单位小时
	/*totalWorkHours = (double)Converter::BcdToNumber(&(dataArea[current]), 3);
	current += 3;*/

	totalWorkHours.SetData(&(dataArea[current]), 3, false, 0, highByteFirst, _T(NAME_WORK_HOURS));
	current += 3;


	//7字节当前时间，BCD码，格式为YYYYMMDDhhmmss，无单位字节
	currentTime.SetData(&(dataArea[current]), CJ188_TIME_DATA_LENGTH, false, -CJ188_TIME_DATA_LENGTH, highByteFirst, _T(NAME_CURRENT_TIME));
	//currentTime = Converter::HexToString(&(dataArea[current]), 7, 0);
	//currentTime.value.puc = new UCHAR[CJ188_TIME_DATA_LENGTH];
	/*
	if (highByteFirst){
		memcpy(currentTime.value.puc, &(dataArea[current]), CJ188_TIME_DATA_LENGTH);
	}
	else{
		for (int i = 0; i < CJ188_TIME_DATA_LENGTH; i++){
			currentTime.value.puc[CJ188_TIME_DATA_LENGTH - i - 1] = dataArea[current + i];
		}
	}*/
	current += CJ188_TIME_DATA_LENGTH;

	//2字节状态字，第一字节为固定意义，第二字节厂家自定义
	statusData.SetData(&(dataArea[current]), CJ188_STATUS_LENGTH, false, -CJ188_STATUS_LENGTH, highByteFirst, _T(NAME_STATUS));
	/*if (highByteFirst){
		memcpy(statusData.value.puc, &(dataArea[current]), CJ188_STATUS_LENGTH);
	}
	else{
		for (int i = 0; i < CJ188_STATUS_LENGTH; i++){
			statusData.value.puc[CJ188_STATUS_LENGTH - i - 1] = dataArea[current + i];
		}
	}
	statusData.SetName(_T(NAME_STATUS));*/
	current += 2;
}


CString DataFrame::GetAddressStr(){
	bool highFirst = frame->highFirst;
	UCHAR padding = wizard.getPaddingDigit();
	return GetPureAddressStr(highFirst, padding) + GetFactoryIdStr(highFirst);

	//根据设置改变显示顺序

	//前置补位f变为0
}


CString DataFrame::GetPureAddressStr(bool highFirst, UCHAR padding){
	UCHAR* pureAddress;
	//根据设置改变显示顺序
	if (!highFirst){
		pureAddress = Converter::ChangeByteOrder(frame->address, PURE_METER_ADDRESS_LENGTH);
	}
	else{
		pureAddress = new UCHAR[PURE_METER_ADDRESS_LENGTH];
		memcpy(pureAddress, frame->address, PURE_METER_ADDRESS_LENGTH);
	}
	//前置补位f变为0
	if (padding == 0xF){
		for (int i = 0; i < PURE_METER_ADDRESS_LENGTH; i++){
			//取出1字节处理
			UCHAR uc = pureAddress[i];
			//若高四位为F
			if ((uc & 0xF0) == 0xF0){
				//高四位置0
				pureAddress[i] &= 0x0F;
				//若低四位为F
				if ((uc & 0x0F) == 0x0F){
					//低四位置0
					pureAddress[i] &= 0x0;
				}
				else{//否则结束处理
					break;
				}
			}
			else{//否则结束处理
				break;
			}

		}
	}
	CString result = Converter::HexToString(pureAddress, PURE_METER_ADDRESS_LENGTH, 0);
	delete pureAddress;
	return result;
}

CString DataFrame::GetFactoryIdStr(bool highFirst){
	//根据设置改变显示顺序
	UCHAR* pureID;
	if (highFirst){
		pureID = new UCHAR[PURE_FACTORY_ID_LENGTH];
		memcpy(pureID, &(frame->address[PURE_METER_ADDRESS_LENGTH]), PURE_FACTORY_ID_LENGTH);
	}
	else{
		pureID = Converter::ChangeByteOrder(&(frame->address[PURE_METER_ADDRESS_LENGTH]), PURE_FACTORY_ID_LENGTH);
	}
	CString result = Converter::HexToString(pureID, PURE_FACTORY_ID_LENGTH, 0);
	delete pureID;
	return result;
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
	bool highByteFirst = !wizard.isLowByteFirst();//CJ188::LookUpByteOrder(frame->meterType, &(frame->address[5]));
	return Converter::BcdToDateTimeStr(currentTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
}

CString DataFrame::GetStatusStr(){
	return Converter::HexToString(statusData.value.puc, CJ188_STATUS_LENGTH, ' ');
}

CString DataFrame::GetRawDataStr(){
	return Converter::HexToString(rawData, rawDataLength);
}