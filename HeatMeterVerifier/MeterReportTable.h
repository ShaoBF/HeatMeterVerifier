#pragma once
#include "ColumnProperty.h"
#include "MyVector.h"
#include "MeterDataInfo.h"

class MeterReportTable
{
public:
	MeterReportTable();
	virtual ~MeterReportTable();

	int GetColumnCount();
	ColumnProperty* GetColumnProperty(int index);
	//void AddData(MeterDataInfo *data);
	int GetRowCount();
	MeterReport* GetReport(int index);
	MeterDataInfo* GetDataInfo(int index);
	MyVector<MeterReport*>* GetReports();
	MeterDataInfo* GetMeterDataInfo(UCHAR* meterID, int startIndex);
	MeterDataInfo* RemoveMeterDataInfo(int index);
	bool ClearAllData();

protected:

public:
	ColumnProperty** columnPropertyList;
	int columnCount;
	MyVector<MeterInfo*>* dataInfoList;
	//MyVector<MeterReport*>* dataInfoList;

};

