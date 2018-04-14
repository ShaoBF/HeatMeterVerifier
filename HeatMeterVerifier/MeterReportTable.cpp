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
	column = new ColumnProperty(L"�ο�", LVCFMT_LEFT, 40);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"��ŵ�ַ", LVCFMT_LEFT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"����", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"�ȹ���", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"����", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"����", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"����¶�(��)", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"�����¶�(��)", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"��ʼʱ��", LVCFMT_RIGHT, TIME_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"����ʱ��", LVCFMT_RIGHT, TIME_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"����ʱ��", LVCFMT_RIGHT, DATA_ITEM_WIDTH);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"�ϸ�", LVCFMT_LEFT, 40);
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

//�ǵ�����delete
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