#pragma once
#include "afxwin.h"


// CMeterReportDlg 对话框

class CMeterReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeterReportDlg)

public:
	CMeterReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterReportDlg();

// 对话框数据
	enum { IDD = IDD_METER_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit testUnit;
	CEdit testMode;
	CEdit meterID;
	CEdit factoryID;
	afx_msg void OnEnChangeMeterType();
	CEdit meterType;
	CEdit startTime;
	CEdit endTime;
	CEdit tester;
	CEdit reviewer;
};
