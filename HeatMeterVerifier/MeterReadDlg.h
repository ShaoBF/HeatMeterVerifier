#pragma once
#include "afxcmn.h"
#include "ComDataReciever.h"
#include "MeterDataTable.h"

// CMeterReadDlg �Ի���

class CMeterReadDlg : public CDialogEx, public CJ188DataReciever
{
	DECLARE_DYNAMIC(CMeterReadDlg)

public:
	CMeterReadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeterReadDlg();

// �Ի�������
	enum { IDD = IDD_METER_READ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
	void CreateMeterDataTable();
	void ProcessMeterData(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188);
	void UpdateMeterReadList();
	CMutex* g_clsMutex;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188);
	void ReadMeters();

	// �������б�
	CListCtrl meterReadList;
	MeterDataTable* meterDataTable;
};
