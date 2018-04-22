#pragma once
#include "afxcmn.h"
#include "ComDataReciever.h"
#include "MeterDataTable.h"
#include "MeterDataInfo.h"
#include "afxwin.h"

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
	bool selectAll;
	bool testStarted=false;

	UCHAR GetPaddingDigit();

	void InitMeterSelectList();
	void GetComList_256(CListBox * CCombox);
	int GetSelectedMeter(LPINT rgIndex);
	void ReadMetersAddress(MyVector<MeterInfo*>* meterInfoList);
	void AddToMeterList(MeterInfo* meterInfo);

	void InitUIs();

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
	afx_msg void OnBnClickedReread();
	afx_msg void OnBnClickedReadAgain();
	afx_msg void OnBnClickedSelectAll();
	// ѡ��Ҫ���ı�
	CListBox meterChooserList;
	// ��׼��ѡ���
	CComboBox refMeterChooserBox;
	// ȫѡ��
	CButton selectAllMeterCheck;
	// �������ɡ�����
	CButton testFinishedButton;
	// ����⿪ʼ���롰���¼�⡱��ť
	CButton testStartButton;
	afx_msg void OnLbnSelchangeMeterChooserList();
	afx_msg void OnCbnSelchangeRefMeterChooserList();
	afx_msg void OnHdnItemdblclickMetersList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkMetersList(NMHDR *pNMHDR, LRESULT *pResult);
	// ���ֽ���ǰѡ���
	CButton lowByteFirstCheck;
	// ��ַ�����루0��F��
	CComboBox paddingDigitBox;
};
