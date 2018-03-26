#pragma once
#include "MeterChooserDlg.h"
#include "MeterReadDlg.h"
#include "MetersReportDlg.h"
#include "MeterReportDlg.h"
#include "MeterInfo.h"
#include <vector>

using namespace std;
class CMeterWizard
{
public:
	CMeterWizard();
	virtual ~CMeterWizard();
	void ChooseMeters();
	void ReadMeters();
	void GenerateReports();
	void AddMeterAddressCom(MeterInfo *meterInfo);
	void SetSelectMeterIndex(LPINT rgIndex,int count);
	vector<MeterInfo*>* GetMeterInfoList(){
		return &meterInfoList;
	};

protected:
	vector<MeterInfo*> meterInfoList;
	LPINT selectedMeterIndex;
	int selectedMeterCount = 0;
};

