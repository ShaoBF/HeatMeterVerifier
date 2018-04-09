#include "stdafx.h"
#include "MeterDataInfo.h"


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
	if (report != nullptr){
		delete report;
		report = nullptr;
	}
	if (startFrame != nullptr&&endFrame != nullptr){
		report = new MeterReport(startFrame, endFrame);
	}
	else{
		report = nullptr;
	}
	return report;
}
