#pragma once
#include "stdafx.h"
#include "CalibrationReport.h"
#include "MeterDataInfo.h"
#include "CJ188.h"
#include "Converter.h"
#include "MeterWizard.h"
#include "TextReportGenerator.h"

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


CString* CalibrationReport::GetContent(){
	if (reportTemplate.IsEmpty()){
		return new CString(L"");
	}
	CReportGenerator* generator=new CTextReportGenerator();
	CMap <CString, LPCTSTR, CString, LPCTSTR>* dataMap = GetDataMap();
	CString* content = (CString*)generator->GenerateReport(&reportTemplate, dataMap);
	delete generator;

	return content;
}
CMap<CString, LPCTSTR, CString, LPCTSTR>* CalibrationReport::GetDataMap(){
	CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap = new CMap<CString, LPCTSTR, CString, LPCTSTR>();
	//CMap<CString, LPCTSTR, void*, void*>* dataMap = new CMap<CString, LPCTSTR, void*, void*>();


	MeterDataInfo* meter = (MeterDataInfo*)meterReport->meter;


	//表信息
	CString meterID = meterReport->GetAddressStr();
	dataMap->SetAt(L"meterID", meterID);

	//b = dataMap->Lookup(L"meterID", vv);

	CString factoryID = meterID.Right(4);
	dataMap->SetAt(L"factoryID", factoryID);
	dataMap->SetAt(L"meterType", meter->GetMeterTypeStr());
	//CString* calibMode = &wizard.calibratorInfo->calibrateMode;
	dataMap->SetAt(L"calibrationMode", wizard.calibratorInfo->calibrateMode);
	dataMap->SetAt(L"calibrationResult", meterReport->GetQualifiedStr());

	//加入测试数据内容
	//<td>项目</td><td>数值</td><td>单位</td><td>参考区间</td><td>标准值</td><td>结论</td>
	CString lowerbound, upperbound;
	MeterReport* refReport = refMeter->GetReport();
	
	CString itemName;
	DataItem* item;
	DataItem* refItem;

	itemName = L"heat";
	item = &meterReport->heat;
	refItem = &refReport->heat;
	dataMap->SetAt(itemName + L"Name", item->GetName());
	dataMap->SetAt(itemName + L"Value", item->GetValueStr());
	dataMap->SetAt(itemName + L"Unit", CJ188::GetUnit(item->unit));
	lowerbound.Format(L"%.2f", refItem->GetValue() * 0.99);
	upperbound.Format(L"%.2f", refItem->GetValue() * 1.01);
	dataMap->SetAt(itemName + L"Lowerbound", lowerbound);
	dataMap->SetAt(itemName + L"Upperbound", upperbound);
	dataMap->SetAt(itemName + L"RefValue", refItem->GetValueStr());
	dataMap->SetAt(itemName + L"Result", item->VerifyWith(refItem, wizard.GetVerifyRate()));

	itemName = L"capacity";
	item = &meterReport->capacity;
	refItem = &refReport->capacity;
	dataMap->SetAt(itemName + L"Name", item->GetName());
	dataMap->SetAt(itemName + L"Value", item->GetValueStr());
	dataMap->SetAt(itemName + L"Unit", CJ188::GetUnit(item->unit));
	lowerbound.Format(L"%.2f", refItem->GetValue() * 0.99);
	upperbound.Format(L"%.2f", refItem->GetValue() * 1.01);
	dataMap->SetAt(itemName + L"Lowerbound", lowerbound);
	dataMap->SetAt(itemName + L"Upperbound", upperbound);
	dataMap->SetAt(itemName + L"RefValue", refItem->GetValueStr());
	dataMap->SetAt(itemName + L"Result", item->VerifyWith(refItem, wizard.GetVerifyRate()));

	itemName = L"temperatureIn";
	item = &meterReport->temperatureIn;
	refItem = &refReport->temperatureIn;
	dataMap->SetAt(itemName + L"Name", item->GetName());
	dataMap->SetAt(itemName + L"Value", item->GetValueStr());
	dataMap->SetAt(itemName + L"Unit", L"℃");// CJ188::GetUnit(item->unit));
	lowerbound.Format(L"%.2f", refItem->GetValue() * 0.99);
	upperbound.Format(L"%.2f", refItem->GetValue() * 1.01);
	dataMap->SetAt(itemName + L"Lowerbound", lowerbound);
	dataMap->SetAt(itemName + L"Upperbound", upperbound);
	dataMap->SetAt(itemName + L"RefValue", refItem->GetValueStr());
	dataMap->SetAt(itemName + L"Result", item->VerifyWith(refItem, wizard.GetVerifyRate()));

	itemName = L"temperatureOut";
	item = &meterReport->temperatureOut;
	refItem = &refReport->temperatureOut;
	dataMap->SetAt(itemName + L"Name", item->GetName());
	dataMap->SetAt(itemName + L"Value", item->GetValueStr());
	dataMap->SetAt(itemName + L"Unit", L"℃"); //CJ188::GetUnit(item->unit));
	lowerbound.Format(L"%.2f", refItem->GetValue() * 0.99);
	upperbound.Format(L"%.2f", refItem->GetValue() * 1.01);
	dataMap->SetAt(itemName + L"Lowerbound", lowerbound);
	dataMap->SetAt(itemName + L"Upperbound", upperbound);
	dataMap->SetAt(itemName + L"RefValue", refItem->GetValueStr());
	dataMap->SetAt(itemName + L"Result", item->VerifyWith(refItem, wizard.GetVerifyRate()));

	itemName = L"duration";
	item = &meterReport->duration;
	refItem = &refReport->duration;
	dataMap->SetAt(itemName + L"Name", item->GetName());
	dataMap->SetAt(itemName + L"Value", item->GetValueStr());
	dataMap->SetAt(itemName + L"Unit", L"s");// CJ188::GetUnit(item->unit));
	lowerbound.Format(L"%.2f", refItem->GetValue() * 0.99);
	upperbound.Format(L"%.2f", refItem->GetValue() * 1.01);
	dataMap->SetAt(itemName + L"Lowerbound", lowerbound);
	dataMap->SetAt(itemName + L"Upperbound", upperbound);
	dataMap->SetAt(itemName + L"RefValue", refItem->GetValueStr());
	dataMap->SetAt(itemName + L"Result", item->VerifyWith(refItem, wizard.GetVerifyRate()));


	//加入检测站内容
	//CString* calibrationStation = &wizard.calibratorInfo->name;
	dataMap->SetAt(L"calibrationStation", wizard.calibratorInfo->name);
	dataMap->SetAt(L"startTime", meterReport->GetStartTimeStr());
	dataMap->SetAt(L"endTime", meterReport->GetEndTimeStr());
	dataMap->SetAt(L"calibrator", wizard.calibratorInfo->tester);
	dataMap->SetAt(L"reviewer", wizard.calibratorInfo->reviewer);
	//void* vv = new CString();
	//BOOL b;
	//CString keystr(L"calibrationStation");
	//b = dataMap->Lookup(keystr.GetBuffer(), vv);
	//CString* value = (CString*)vv;


	return dataMap;
}
