#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DataFrame.h"
#include "ColumnProperty.h"
#include "MyVector.h"

// CDataFrameDisplayDlg �Ի���

class CDataFrameDisplayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataFrameDisplayDlg)

public:
	CDataFrameDisplayDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataFrameDisplayDlg();
	BOOL OnInitDialog();

	void SetDataFrame(DataFrame* frame);

// �Ի�������
	enum { IDD = IDD_DATA_FRAME_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//��ʼ������UI�ؼ�
	void InitUIs();

	//����������չʾ���
	void DisplayDataList();
	void UpdateDataList();
	void InsertData(DataItem* item, int row);

public:
	// ԭ����֡չʾ��
	CEdit rawFrameDisplay;
	// ������������չʾ��
	CListCtrl dataListCtrl;
	DataFrame* dataFrame;
	MyVector<ColumnProperty*> columnPropertyList;
};
