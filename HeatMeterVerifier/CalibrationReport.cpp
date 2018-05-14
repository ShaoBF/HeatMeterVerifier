#include "stdafx.h"
#include "CalibrationReport.h"
#include "ReportGenerator.h"
#include "MeterDataInfo.h"

CalibrationReport::CalibrationReport(CString reportTemplate){
	this->reportTemplate = reportTemplate;
}

CalibrationReport::CalibrationReport(CString reportTemplate, MeterReport* report, CCalibratorInfo* calibratorInfo)
{
	this->reportTemplate = reportTemplate;
	this->meterReport = report;
	this->calibratorInfo = calibratorInfo;
}


CalibrationReport::~CalibrationReport()
{

}

void CalibrationReport::SetMeterReport(MeterReport* meterReport){
	this->meterReport = meterReport;
}
void CalibrationReport::SetCalibratorInfo(CCalibratorInfo* calibratorInfo){
	this->calibratorInfo = calibratorInfo;
}
void CalibrationReport::SetReportTemplate(CString reportTemplate){
	this->reportTemplate = reportTemplate;
}



CString CalibrationReport::GetContent(){
	if (reportTemplate.IsEmpty()){
		return L"";
	}
	CReportGenerator generator;
	CMap < WCHAR *, WCHAR *, CString, CString>* dataMap = GetDataMap();
	CString content = generator.GenerateReport(reportTemplate, dataMap);

	return content;
}
CMap<WCHAR *, WCHAR *, CString, CString>* CalibrationReport::GetDataMap(){
	//CMap<int, CString, WCHAR *, WCHAR *>* dataMap = new CMap<int, CString, WCHAR *, WCHAR *>();
	CMap<WCHAR *, WCHAR *, CString, CString>* dataMap = new CMap<WCHAR *, WCHAR *, CString, CString>();
	//CMap<CString, CString, CString, CString>* dataMap = new CMap<CString, CString, CString, CString>();

	MeterDataInfo* meter = (MeterDataInfo*)meterReport->meter;

	//加入检测站内容
	dataMap->SetAt(L"calibrationStation", L"北京市计量科学研究院");

	//表信息
	CString meterID = meterReport->GetAddressStr();
	dataMap->SetAt(L"meterID", meterID);
	CString factoryID = meterID.Right(4);
	dataMap->SetAt(L"factoryID", factoryID);
	dataMap->SetAt(L"meterType", meter->GetMeterTypeStr());
	dataMap->SetAt(L"calibrationMode", L" ");
	dataMap->SetAt(L"calibrationResult", meterReport->GetQualifiedStr());

	//加入测试数据内容

	return dataMap;
}
