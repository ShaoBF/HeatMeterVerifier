#pragma once
#include "MeterChooserDlg.h"
#include "MeterReadDlg.h"
#include "MetersReportDlg.h"
#include "MeterReportDlg.h"
#include "MeterInfo.h"
#include "MyVector.h"
//#include "MyVector.cpp"
#include "MeterReportDlg.h"
#include "VerifyConfig.h"
#include "TemplateConfig.h"
#include "CalibratorInfo.h"
#include "DBConfig.h"

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
	//MyVector<MeterDataInfo*>* GetMeterInfoList();
	void CloseAllCom();
	MeterInfo* GetMeterInfo(int index);
	MeterInfo* GetMeterInfo(UCHAR* address);
	void setRefMeter(int index);
	bool IsRefMeter(UCHAR* address);
	MeterInfo* GetRefMeterInfo();

	void setLowByteFirst(bool lowFirst);
	void setPaddingDigit(UCHAR digit);

	bool isLowByteFirst();
	UCHAR getPaddingDigit();

	double GetVerifyRate();

	LPCTSTR GetConnectStr();
	//void RemoveReport(int index);
	CString GetConfigFilePath();
	CString GetTemplatePath();
	CString GetTemplateFilePath();
	CString GetInstallPath();
	BOOL LoadConfig();
	LPCTSTR GetDataSrcStr();
	//BOOL CMeterWizard::ReadReportTemplateConfig(CString configPath);

public:
//	MeterInfo** meterInfoList;
//	int meterCount;
	MyVector<MeterInfo*> meterInfoList;
	MeterInfo* refMeter;
	int refMeterIndex;
	//MyVector<MeterReport*> meterReports;
	VerifyConfig config;
	bool lowByteFirst = false;
	UCHAR paddingDigit = 0xf;
	//CString templateFile;
	TemplateConfig* templateConfig;
	CCalibratorInfo* calibratorInfo;
	CDBConfig* dbConfig;

protected:
	LPINT selectedMeterIndex;
	int selectedMeterCount = 0;
};

