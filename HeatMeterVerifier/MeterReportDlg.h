#pragma once
#include "afxwin.h"


// CMeterReportDlg �Ի���

class CMeterReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeterReportDlg)

public:
	CMeterReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeterReportDlg();

// �Ի�������
	enum { IDD = IDD_METER_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
