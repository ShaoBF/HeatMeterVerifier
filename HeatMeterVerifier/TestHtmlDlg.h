#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "HtmlCtrl.h"
#include "ReportDlg.h"

// CTestHtmlDlg 对话框

class CHtmlReportDlg : public CReportDlg
{
	DECLARE_DYNAMIC(CHtmlReportDlg)

public:
	CHtmlReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHtmlReportDlg();
	void InitTestHtml2();
	void DisplayReport(CalibrationReport* report);
	void InitUIs();


// 对话框数据
	enum { IDD = IDD_TEST_HTML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CStatic staticForHtml;
	CHtmlCtrl m_html;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPrint();
	void PrintScreen(CRect rect, int left, int top);
};
