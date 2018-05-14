#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MeterDataInfo.h"
#include "MyVector.h"
#include "ColumnProperty.h"
#include "ReportDlg.h"


// CMeterReportDlg 对话框

class CMeterReportDlg : public CReportDlg
{
	DECLARE_DYNAMIC(CMeterReportDlg)

public:
	CMeterReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterReportDlg();

	//void SetMeterReport(MeterReport* report);
	//void SetRefMeterData(MeterDataInfo* data);
	BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_METER_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	//初始化所有UI控件
	void InitUIs();
	//绘制数据项展示表格
	void DisplayReport(CalibrationReport* report);
	void CreateReportList();
	void InsertData(int row, DataItem* item, DataItem* refItem, CString unit, double rate);
	CBitmap* ScreenShot(CRect rect, int left, int top, char *name);//截取窗口的大小，位置，名字（保存在默认路径下)
	void PrintScreen(CRect rect, int left, int top);
	HANDLE GetPrinterHandle(LPTSTR lpszPrinterName);

	//MeterReport* report;
	//MeterDataInfo* refData;
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
	// 当前表检测结论（合格/不合格）
	CEdit meterResult;
	// 表计检测结果报告列表
	CListCtrl resultList;
	afx_msg void OnEnChangeMeterType();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnPaint();

	void testPrint(CBitmap* image);
	CString GetReportTemplate();
	//CalibrationReport* GenerateReport();
};

