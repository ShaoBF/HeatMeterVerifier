#pragma once


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
};
