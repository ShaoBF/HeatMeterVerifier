#pragma once
#include "afxcmn.h"


// CMetersReportDlg �Ի���

class CMetersReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMetersReportDlg)

public:
	CMetersReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMetersReportDlg();

// �Ի�������
	enum { IDD = IDD_METERS_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	// ��Ʊ����б�����ѡ��ĳһ��鿴��ϸ����
	CListCtrl meterReportList;
	afx_msg void OnBnClickedCancel();
};
