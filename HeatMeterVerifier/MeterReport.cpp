#include "stdafx.h"
#include "MeterReport.h"
#include "Converter.h"


MeterReport::MeterReport()
{
}


MeterReport::~MeterReport()
{
}
MeterReport::MeterReport(DataFrame* startFrame, DataFrame* endFrame){
	
	Calculate(startFrame, endFrame);
}

void MeterReport::Calculate(DataFrame* startFrame, DataFrame* endFrame){
	DataItem* start;
	DataItem* end;
	UnionValue value;

	start = &(startFrame->currentHeat);
	end = &(endFrame->currentHeat);
	value.dv = end->value.dv - start->value.dv;
	heat.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	heat.name = L"总热量";

	start = &(startFrame->billingDayCapacity);
	end = &(endFrame->billingDayCapacity);
	value.dv = end->value.dv - start->value.dv;
	capacity.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	capacity.name = L"总流量";

	//FIXME:数值取值具体方式待定
	start = &(startFrame->heatPower);
	end = &(endFrame->heatPower);
	value.dv = end->value.dv; //- start->value.dv;
	heatPower.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	heatPower.name = L"热功率";

	//FIXME:数值取值具体方式待定
	start = &(startFrame->flowRate);
	end = &(endFrame->flowRate);
	value.dv = end->value.dv; //- start->value.dv;
	flowRate.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	flowRate.name = L"流速";

	start = &(startFrame->temperatureIn);
	value.dv = start->value.dv;
	temperatureIn.SetData(value, start->decimalPoint, start->hasUnit, start->unit);
	temperatureIn.name = L"进水温度";

	start = &(startFrame->temperatureOut);
	value.dv = start->value.dv;
	temperatureOut.SetData(value, start->decimalPoint, start->hasUnit, start->unit);
	temperatureOut.name = L"出水温度";

	start = &(startFrame->currentTime);
	end = &(endFrame->currentTime);
	value.puc = start->value.puc;
	startTime.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	startTime.name = L"开始时间";

	value.puc = end->value.puc;
	endTime.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	endTime.name = L"结束时间";

	/*UCHAR meterType = endFrame->frame->meterType;
	UCHAR* vandorID = &(endFrame->frame->address[5]);*/
	//bool highByteFirst = CJ188::LookUpByteOrder(meterType, vandorID);


	time_t s = Converter::BcdToTimeT(start->value.puc, 7, highByteFirst);
	time_t e = Converter::BcdToTimeT(end->value.puc, 7, highByteFirst);
	duration.value.dwv = e - s;
	duration.decimalPoint = 0;
	duration.hasUnit = 0;
	duration.name = L"持续时间";

	memcpy(address, startFrame->frame->address, CJ188_ADDRESS_LENGTH);
	isQualified = false;
}

void MeterReport::CalculateQualified(MeterReport* refData){
	//bool qualified = false;
	double deltaHeat = abs(heat.value.dv - refData->heat.value.dv);
	double deltaCapacity = abs(capacity.value.dv - refData->capacity.value.dv);
	double deltaTempIn = abs(temperatureIn.value.dv - refData->temperatureIn.value.dv);
	double deltaTempOut = abs(temperatureOut.value.dv - refData->temperatureOut.value.dv);
	double deltaDur = abs((double)(duration.value.dwv - refData->duration.value.dwv));

	if ((deltaHeat / refData->heat.value.dv) >= 0.01){
		isQualified = false;
	}
	else if ((deltaCapacity / refData->capacity.value.dv) >= 0.01){
		isQualified = false;
	}
	else if ((deltaTempIn / refData->temperatureIn.value.dv) >= 0.01){
		isQualified = false;
	}
	else if ((deltaTempOut / refData->temperatureOut.value.dv) >= 0.01){
		isQualified = false;
	}
	else if ((deltaDur / (double)(refData->duration.value.dwv)) >= 0.01){
		isQualified = false;
	}
	else{
		isQualified = true;
	}
	//return qualified;
}

CString MeterReport::GetAddressStr(){
	return Converter::HexToString(address, 7, 0);
}

CString MeterReport::GetHeatStr(){
	return heat.ToString();
}

CString MeterReport::GetHeatPowerStr(){
	return heatPower.ToString();
}

CString MeterReport::GetFlowRateStr(){
	return flowRate.ToString();
}

CString MeterReport::GetCapacityStr(){
	return capacity.ToString();
}

CString MeterReport::GetTemperatureInStr(){
	return temperatureIn.ToString() + " " + CELSIUS;
}

CString MeterReport::GetTemperatureOutStr(){
	return temperatureOut.ToString() + " " + CELSIUS;
}

CString MeterReport::GetStartTimeStr(){
	return Converter::BcdToDateTimeStr(startTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
	//return Converter::HexToString(startTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
}

CString MeterReport::GetEndTimeStr(){
	return Converter::BcdToDateTimeStr(endTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
	//return Converter::HexToString(endTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
}

CString MeterReport::GetDurationStr(){
	return duration.ToString()+" sec";
}

CString MeterReport::GetQualifiedStr(){
	if (isQualified){
		return L"√";
	}
	else{
		return L"×";
	}
}
