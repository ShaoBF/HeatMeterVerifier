#pragma once
#include "afxcmn.h"


// CMeterReadDlg �Ի���

class CMeterReadDlg : public CDialogEx
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

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	// �������б�
	CListCtrl MeterReadList;
};
