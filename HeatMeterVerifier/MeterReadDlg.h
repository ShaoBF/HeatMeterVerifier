#pragma once
#include "afxcmn.h"
#include "ComDataReciever.h"
#include "MeterDataTable.h"
#include "MeterDataInfo.h"
#include "afxwin.h"

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

	// 读表结果列表
	CListCtrl meterReadList;
	MeterDataTable* meterDataTable;
	afx_msg void OnBnClickedReread();
	afx_msg void OnBnClickedReadAgain();
	afx_msg void OnBnClickedSelectAll();
	// 选择要读的表
	CListBox meterChooserList;
	// 标准表选择框
	CComboBox refMeterChooserBox;
	// 全选表
	CButton selectAllMeterCheck;
	// “检测完成”按键
	CButton testFinishedButton;
	// “检测开始”与“重新检测”按钮
	CButton testStartButton;
	afx_msg void OnLbnSelchangeMeterChooserList();
	afx_msg void OnCbnSelchangeRefMeterChooserList();
	afx_msg void OnHdnItemdblclickMetersList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkMetersList(NMHDR *pNMHDR, LRESULT *pResult);
	// 低字节在前选择框
	CButton lowByteFirstCheck;
	// 地址补充码（0或F）
	CComboBox paddingDigitBox;
};
