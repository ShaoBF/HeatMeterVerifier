#include "stdafx.h"
#include "MeterDataInfo.h"
#include "Converter.h"

MeterDataInfo::MeterDataInfo()
{
	startFrame = nullptr;
	endFrame = nullptr;
	report = nullptr;
}
MeterDataInfo::MeterDataInfo(CString com, UCHAR* code):MeterInfo(com,code){

}

MeterDataInfo::~MeterDataInfo()
{/*
	if (startFrame != nullptr){
		delete startFrame;
	}
	if (endFrame != nullptr){
		delete endFrame;
	}
	if (report != nullptr){
		delete report;
	}*/
}

void MeterDataInfo::SetStartFrame(DataFrame* frame){
	startFrame = frame;
}
void MeterDataInfo::SetEndFrame(DataFrame* frame){
	endFrame = frame;
}

DataFrame* MeterDataInfo::GetStartFrame(){
	return startFrame;
}
DataFrame* MeterDataInfo::GetEndFrame(){
	return endFrame;
}
MeterReport* MeterDataInfo::GetReport(){
	if (report == nullptr){
		report = new MeterReport(startFrame, endFrame);
		report->meter = this;
	}
	return report;
}

MeterReport* MeterDataInfo::CalculateReport(MeterReport* refData){
	if (report != nullptr){
		delete report;
		report = nullptr;
	}
	if (startFrame != nullptr&&endFrame != nullptr){
		report = new MeterReport(startFrame, endFrame);
		report->meter = this;
	}
	else{
		report = nullptr;
	}
	report->CalculateQualified(refData);
	return report;

}

CString MeterDataInfo::GetAddressStr(){
	return Converter::HexToString(address, 7, 0);
}

CString MeterDataInfo::GetHeatStr(){
	return report->heat.ToString();
}

CString MeterDataInfo::GetHeatPowerStr(){
	return report->heatPower.ToString();
}

CString MeterDataInfo::GetFlowRateStr(){
	return report->flowRate.ToString();
}

CString MeterDataInfo::GetCapacityStr(){
	return report->capacity.ToString();
}

CString MeterDataInfo::GetTemperatureInStr(){
	return report->temperatureIn.ToString() + " " + CELSIUS;
}

CString MeterDataInfo::GetTemperatureOutStr(){
	return report->temperatureOut.ToString() + " " + CELSIUS;
}

CString MeterDataInfo::GetStartTimeStr(){
	return Converter::BcdToDateTimeStr(report->startTime.value.puc, CJ188_TIME_DATA_LENGTH, report->highByteFirst);
	//return Converter::HexToString(startTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
}

CString MeterDataInfo::GetEndTimeStr(){
	return Converter::BcdToDateTimeStr(report->endTime.value.puc, CJ188_TIME_DATA_LENGTH, report->highByteFirst);
	//return Converter::HexToString(endTime.value.puc, CJ188_TIME_DATA_LENGTH, highByteFirst);
}

CString MeterDataInfo::GetDurationStr(){
	return report->duration.ToString() + " sec";
}


CString MeterDataInfo::GetMeterTypeStr(){
	CString str;
	UCHAR meterType = startFrame->frame->meterType;
	switch (meterType){
	case ColdWater:
		str = "冷水表";
		break;
	case LifeHotWater:
		str = "生活热水";
		break;
	case DrinkWater:
		str = "饮用水";
		break;
	case ReclaimedWater:
		str = "循环水";
		break;
	case HeatMeter:
		str = "热水表";
		break;
	case CoolMeter:
		str = "凉水表";
		break;
	case HeatColdMeter:
		str = "冷热水表";
		break;
	case GasMeter:
		str = "燃气表";
		break;
	case MiscMeters:
		str = "其他";
		break;
	default:
		str = "未定义";
		break;
	}
	return str;
}
