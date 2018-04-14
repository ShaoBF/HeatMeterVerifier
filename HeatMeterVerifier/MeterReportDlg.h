#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MeterDataInfo.h"
#include "MyVector.h"
#include "ColumnProperty.h"


// CMeterReportDlg 对话框

class CMeterReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeterReportDlg)

public:
	CMeterReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterReportDlg();

	void SetMeterReport(MeterReport* report);
	void SetRefMeterData(MeterDataInfo* data);
	BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_METER_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	//初始化所有UI控件
	void InitUIs();
	//绘制数据项展示表格
	void DisplayReport();
	void CreateReportList();
	void InsertData(int row, DataItem* item, DataItem* refItem, CString unit, double rate);

	MeterReport* report;
	MeterDataInfo* refData;
	MyVector<ColumnProperty*> columnPropertyList;

	DECLARE_MESSAGE_MAP()
public:
	CEdit testUnit;
	CEdit testMode;
	CEdit meterID;
	CEdit factoryID;
	CEdit meterType;
	CEdit startTime;
	CEdit endTime;
	CEdit tester;
	CEdit reviewer;
	// 表计检测结果报告列表
	CListCtrl resultList;
	afx_msg void OnEnChangeMeterType();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnPaint();

	void testPrint();
};

