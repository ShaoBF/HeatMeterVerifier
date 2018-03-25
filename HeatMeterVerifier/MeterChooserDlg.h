#pragma once
#include "afxwin.h"
#include "Resource.h"
#include <vector>
#include "MeterInfo.h"
#include "ComDataReciever.h"
#include "CJ188.h"

using namespace std;


// CMeterChooserDlg �Ի���

class CMeterChooserDlg : public CDialogEx, public CJ188DataReciever
{
	DECLARE_DYNAMIC(CMeterChooserDlg)

public:
	CMeterChooserDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeterChooserDlg();
	bool selectAll;

// �Ի�������
	enum { IDD = IDD_METER_CHOOSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
	void InitMeterSelectList();
	void GetComList_256(CListBox * CCombox);
	int GetSelectedMeter(LPINT rgIndex);
	void ReadMetersID(vector<MeterInfo*>* meterInfoList);
	void AddToMeterList(MeterInfo* meterInfo);

	DECLARE_MESSAGE_MAP()
public:
	// ���ѡ��������סCtrl�ɶ�ѡ
	CListBox meterChooserList;
	// ȫѡ/ȫ��ѡ
	CButton SelectAllCheck;
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnBnClickedOk();
	void OnFrameDataRecieved(char* data, DWORD bufferLen, CJ188Frame *frame, CJ188* cj188);
};
