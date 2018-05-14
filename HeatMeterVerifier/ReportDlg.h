#pragma once
#include "afxdialogex.h"
#include "MeterReport.h"
#include "MeterDataInfo.h"
#include "CalibrationReport.h"


class CReportDlg :
	public CDialogEx
{
public:
	CReportDlg(CWnd* pParent = NULL);
	CReportDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CReportDlg();

	MeterReport* meterReport;
	MeterDataInfo* refData;

	void SetMeterReport(MeterReport* report);
	void SetRefMeterData(MeterDataInfo* data);
	virtual void DisplayReport(CalibrationReport* report) = 0;
	virtual void InitUIs()= 0;
	virtual BOOL OnInitDialog();
	virtual CalibrationReport* GenerateReport();
	virtual CString GetReportTemplate();
	CCalibratorInfo* GetCalibratorInfo();
};

