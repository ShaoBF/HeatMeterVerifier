#pragma once
#include "MeterChooserDlg.h"
#include "MeterReadDlg.h"
#include "MetersReportDlg.h"
#include "MeterReportDlg.h"
#include "MeterInfo.h"
#include "MyVector.h"
#include "MyVector.cpp"

using namespace std;
class CMeterWizard
{
public:
	CMeterWizard();
	virtual ~CMeterWizard();

	void ChooseMeters();
	void ReadMeters();
	void GenerateReports();

	void AddMeterAddressCom(MeterInfo* meterInfo);
	void SetSelectMeterIndex(LPINT rgIndex,int count);
	MyVector<MeterInfo*>* GetMeterInfoList(){
		return &meterInfoList;
	};
	MyVector<MeterReport*>* GetMeterReports();
	void CloseAllCom();
	MeterInfo* GetMeterInfo(int index);
	MeterInfo* GetMeterInfo(UCHAR* address);
	void setRefMeter(int index);
	bool IsRefMeter(UCHAR* address);

	LPCTSTR GetConnectStr();
	//void RemoveReport(int index);

public:
//	MeterInfo** meterInfoList;
//	int meterCount;
	MyVector<MeterInfo*> meterInfoList;
	MeterInfo* refMeter;
	int refMeterIndex;
	//MyVector<MeterReport*> meterReports;

protected:
	LPINT selectedMeterIndex;
	int selectedMeterCount = 0;
};

