#pragma once
#include "afxwin.h"
#include "Resource.h"
#include <vector>
#include "MeterInfo.h"
#include "ComDataReciever.h"
#include "CJ188.h"

using namespace std;


// CMeterChooserDlg 对话框

class CMeterChooserDlg : public CDialogEx, public CJ188DataReciever
{
	DECLARE_DYNAMIC(CMeterChooserDlg)

public:
	CMeterChooserDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMeterChooserDlg();
	bool selectAll;

// 对话框数据
	enum { IDD = IDD_METER_CHOOSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
	void InitMeterSelectList();
	void GetComList_256(CListBox * CCombox);
	int GetSelectedMeter(LPINT rgIndex);
	void ReadMetersID(vector<MeterInfo*>* meterInfoList);
	void AddToMeterList(MeterInfo* meterInfo);

	DECLARE_MESSAGE_MAP()
public:
	// 表计选择栏，按住Ctrl可多选
	CListBox meterChooserList;
	// 全选/全不选
	CButton SelectAllCheck;
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnBnClickedOk();
	void OnFrameDataRecieved(char* data, DWORD bufferLen, CJ188Frame *frame, CJ188* cj188);
};
