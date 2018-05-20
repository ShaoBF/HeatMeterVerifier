#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "HtmlCtrl.h"
#include "ReportDlg.h"

// CTestHtmlDlg �Ի���

class CHtmlReportDlg : public CReportDlg
{
	DECLARE_DYNAMIC(CHtmlReportDlg)

public:
	CHtmlReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHtmlReportDlg();
	void InitTestHtml2();
	void DisplayReport(CalibrationReport* report);
	void InitUIs();


// �Ի�������
	enum { IDD = IDD_TEST_HTML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CStatic staticForHtml;
	CHtmlCtrl m_html;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPrint();
	void PrintScreen(CRect rect, int left, int top);
};
