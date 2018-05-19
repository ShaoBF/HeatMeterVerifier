#pragma once
#include "afxcmn.h"
#include "MeterReport.h"
#include "MyVector.h"
#include "ColumnProperty.h"
#include "MeterReportTable.h"
//#include <afxdb.h>
#include "MeterReportAccess.h"
#include "MeterDataAccess.h"

// CMetersReportDlg �Ի���

class CMetersReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMetersReportDlg)

public:
	CMetersReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMetersReportDlg();
	BOOL OnInitDialog();
	void InitUIs();
	void CreateMeterReportTable();

	//void SetMeterReportList(MyVector<MeterReport*>* reportList);
	void SetMeterInfoList(MyVector<MeterInfo*>* dataInfoList);


// �Ի�������
	enum { IDD = IDD_METERS_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void SaveReports(MyVector<MeterReport*>* reports);
	//void SaveReport(CMeterReportAccess* db, MeterReport* report);
	void SaveReport(MeterReport* report);

	void SaveMeterDataList(MyVector<MeterInfo*>* meterList);
	void SaveMeterData(CMeterDataAccess* dataAccess, MeterDataInfo* meterData);

	void SaveTestData(DataFrame* data, UINT64 testID);
	void SaveTestData(CMeterDataAccess* access, DataFrame* data, UINT64 testID);

	void UpdateMeterReportList();
	//UINT64 GetReportID(CDatabase* database, MeterReport* report);

	CMutex* g_clsMutex;

	//ColumnProperty** columns;
	//MyVector<MeterReport*>* reports;
	MeterReportTable* reportTable;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	// ��Ʊ����б�����ѡ��ĳһ��鿴��ϸ����
	CListCtrl meterReportList;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSave();
	afx_msg void OnHdnItemdblclickReportList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkReportList(NMHDR *pNMHDR, LRESULT *pResult);
};
