#pragma once
#include "afxcmn.h"
#include "ComDataReciever.h"


// CMeterReadDlg 对话框

class CMeterReadDlg : public CDialogEx, public CJ188DataReciever
{
	DECLARE_DYNAMIC(CMeterReadDlg)

public:
	CMeterReadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterReadDlg();

// 对话框数据
	enum { IDD = IDD_METER_READ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188);
	void ReadMeters();

	// 读表结果列表
	CListCtrl MeterReadList;
};
