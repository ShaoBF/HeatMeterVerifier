#include "stdafx.h"
#include "CalibrationReport.h"
#include "ReportGenerator.h"
#include "MeterDataInfo.h"
#include "CJ188.h"
#include "Converter.h"
#include "MeterWizard.h"

extern CMeterWizard wizard;

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
void CalibrationReport::SetRefMeter(MeterDataInfo* refMeterData){
	this->refMeter = refMeterData;
}


CString CalibrationReport::GetContent(){
	if (reportTemplate.IsEmpty()){
		return L"";
	}
	CReportGenerator generator;
	CMap <CString, LPCTSTR, CString, LPCTSTR>* dataMap = GetDataMap();
	CString content = generator.GenerateReport(reportTemplate, dataMap);

	return content;
}
CMap<CString, LPCTSTR, CString, LPCTSTR>* CalibrationReport::GetDataMap(){
	//CMap<int, CString, WCHAR *, WCHAR *>* dataMap = new CMap<int, CString, WCHAR *, WCHAR *>();
	CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap = new CMap<CString, LPCTSTR, CString, LPCTSTR>();
	//CMap<CString, CString, CString, CString>* dataMap = new CMap<CString, CString, CString, CString>();

	MeterDataInfo* meter = (MeterDataInfo*)meterReport->meter;


	//表信息
	CString meterID = meterReport->GetAddressStr();
	dataMap->SetAt(L"meterID", meterID);
	CString value;
	BOOL b = dataMap->Lookup(L"meterID", value);
	CString factoryID = meterID.Right(4);
	dataMap->SetAt(L"factoryID", factoryID);
	dataMap->SetAt(L"meterType", meter->GetMeterTypeStr());
	dataMap->SetAt(L"calibrationMode", L" ");
	dataMap->SetAt(L"calibrationResult", meterReport->GetQualifiedStr());

	//加入测试数据内容
	//<td>项目</td><td>数值</td><td>单位</td><td>参考区间</td><td>标准值</td><td>结论</td>
	CString lowerbound, upperbound;

	dataMap->SetAt(L"heatname", meterReport->heat.GetName());
	dataMap->SetAt(L"heatvalue", meterReport->heat.GetValueStr());
	dataMap->SetAt(L"heatunit", CJ188::GetUnit(meterReport->heat.unit));
	MeterReport* refReport = refMeter->GetReport();
	lowerbound.Format(L"%.2f", refReport->heat.GetValue() * 0.99);
	upperbound.Format(L"%.2f", refReport->heat.GetValue() * 1.01);
	dataMap->SetAt(L"heatlowerbound", lowerbound);
	dataMap->SetAt(L"heatupperbound", upperbound);
	dataMap->SetAt(L"heatrefvalue", refReport->heat.GetValueStr());
	dataMap->SetAt(L"heatresult", meterReport->heat.VerifyWith(&(refReport->heat),wizard.GetVerifyRate()));

	//加入检测站内容
	CString calibrationStation = L"北京市计量科学研究院";
	dataMap->SetAt(L"calibrationStation", calibrationStation);


	return dataMap;
}
