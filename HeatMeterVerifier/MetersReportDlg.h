#pragma once
#include "afxcmn.h"
#include "MeterReport.h"
#include "MyVector.h"
#include <afxdb.h>

// CMetersReportDlg 对话框

class CMetersReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMetersReportDlg)

public:
	CMetersReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMetersReportDlg();

	void SetReports(MyVector<MeterReport*>* reports);
	

// 对话框数据
	enum { IDD = IDD_METERS_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void SaveReports(MyVector<MeterReport*>* reports);
	void SaveReport(CDatabase* db, MeterReport* report);
	void SaveReport(MeterReport* report);

	MyVector<MeterReport*>* reports;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	// 表计报告列表，可以选择某一项查看详细报告
	CListCtrl meterReportList;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSave();
};
