#include "stdafx.h"
#include "MeterReportTable.h"
#include "MyVector.cpp"
#include "TableDataDefines.h"
#include "MeterWizard.h"

extern CMeterWizard wizard;
MeterReportTable::MeterReportTable()
{
	columnCount = 12;
	columnPropertyList = new ColumnProperty*[columnCount];
	int current = 0;
	ColumnProperty* column;
	column = new ColumnProperty(L"参考", LVCFMT_LEFT, 40);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"表号地址", LVCFMT_LEFT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"热量", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"热功率", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"流量", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"流速", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"入口温度(℃)", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"出口温度(℃)", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"开始时间", LVCFMT_RIGHT, TIME_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"结束时间", LVCFMT_RIGHT, TIME_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"持续时间", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"合格", LVCFMT_LEFT, 40);
	columnPropertyList[current++] = column;
	dataInfoList = wizard.GetMeterInfoList();//new MyVector<MeterDataInfo*>();

}


MeterReportTable::~MeterReportTable()
{
	for (int i = 0; i < columnCount; i++){
		delete columnPropertyList[i];
	}
	delete columnPropertyList;
	ClearAllData();
	//delete dataInfoList;

}

int MeterReportTable::GetColumnCount(){
	return columnCount;
}

ColumnProperty* MeterReportTable::GetColumnProperty(int index){
	return columnPropertyList[index];
}
/*
void MeterReportTable::AddData(MeterDataInfo *data){
	dataInfoList->Add(data);
}
*/
int MeterReportTable::GetRowCount(){
	return dataInfoList->GetSize();
}

MeterReport* MeterReportTable::GetReport(int index){
	MeterDataInfo* data = (MeterDataInfo*)(*dataInfoList)[index];
	return data->GetReport();
}

MeterDataInfo* MeterReportTable::GetMeterDataInfo(UCHAR* meterID, int startIndex){
	int i, j;
	for (i = startIndex; i < dataInfoList->GetSize(); i++){
		for (j = 0; j < CJ188_ADDRESS_LENGTH; j++){
			if ((*dataInfoList)[i]->address[j] != meterID[j]){
				break;
			}
		}
		if (j = CJ188_ADDRESS_LENGTH){
			return (MeterDataInfo*)(*dataInfoList)[i];
		}
	}
	return nullptr;
}


bool MeterReportTable::ClearAllData(){
	if (dataInfoList->GetSize() >= 0){
		for (int i = 0; i < dataInfoList->GetSize(); i++){
			MeterDataInfo* report = RemoveMeterDataInfo(i);
			delete report;
		}
		dataInfoList->Clear();
		return true;
	}
	else{
		return false;
	}
}

//记得用完delete
MeterDataInfo* MeterReportTable::RemoveMeterDataInfo(int index){
	MeterDataInfo* report = (MeterDataInfo*)dataInfoList->Remove(index);
	return report;
}

MyVector<MeterReport*>* MeterReportTable::GetReports(){
	MyVector<MeterReport*>* reportList = new MyVector<MeterReport*>();
	MeterReport* report;
	for (int i = 0; i < dataInfoList->GetSize(); i++){
		MeterDataInfo* data = (MeterDataInfo*)dataInfoList->Get(i);
		report = data->GetReport();
		reportList->Add(report);
	}
	return reportList;
}