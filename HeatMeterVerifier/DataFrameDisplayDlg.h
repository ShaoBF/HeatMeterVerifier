#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DataFrame.h"
#include "ColumnProperty.h"
#include "MyVector.h"

// CDataFrameDisplayDlg 对话框

class CDataFrameDisplayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataFrameDisplayDlg)

public:
	CDataFrameDisplayDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataFrameDisplayDlg();
	BOOL OnInitDialog();

	void SetDataFrame(DataFrame* frame);

// 对话框数据
	enum { IDD = IDD_DATA_FRAME_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//初始化所有UI控件
	void InitUIs();

	//绘制数据项展示表格
	void DisplayDataList();
	void UpdateDataList();
	void InsertData(DataItem* item, int row);

public:
	// 原数据帧展示区
	CEdit rawFrameDisplay;
	// 解析后数据项展示区
	CListCtrl dataListCtrl;
	DataFrame* dataFrame;
	MyVector<ColumnProperty*> columnPropertyList;
};
