#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MeterDataInfo.h"
#include "MyVector.h"
#include "ColumnProperty.h"


// CMeterReportDlg �Ի���

class CMeterReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeterReportDlg)

public:
	CMeterReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeterReportDlg();

	void SetMeterReport(MeterReport* report);
	void SetRefMeterData(MeterDataInfo* data);
	BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_METER_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	//��ʼ������UI�ؼ�
	void InitUIs();
	//����������չʾ���
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
	// ��Ƽ���������б�
	CListCtrl resultList;
	afx_msg void OnEnChangeMeterType();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnPaint();

	void testPrint();
};

