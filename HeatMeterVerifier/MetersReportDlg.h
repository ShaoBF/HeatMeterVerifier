#pragma once
#include "afxcmn.h"
#include "MeterReport.h"
#include "MyVector.h"
#include <afxdb.h>

// CMetersReportDlg �Ի���

class CMetersReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMetersReportDlg)

public:
	CMetersReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMetersReportDlg();

	void SetReports(MyVector<MeterReport*>* reports);
	

// �Ի�������
	enum { IDD = IDD_METERS_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void SaveReports(MyVector<MeterReport*>* reports);
	void SaveReport(CDatabase* db, MeterReport* report);
	void SaveReport(MeterReport* report);

	MyVector<MeterReport*>* reports;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	// ��Ʊ����б�����ѡ��ĳһ��鿴��ϸ����
	CListCtrl meterReportList;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSave();
};
