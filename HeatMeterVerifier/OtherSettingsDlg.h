#pragma once
#include "afxwin.h"


// COtherSettingsDlg �Ի���

class COtherSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COtherSettingsDlg)

public:
	COtherSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COtherSettingsDlg();

// �Ի�������
	enum { IDD = IDD_OTHER_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void InitConfig();
	BOOL SaveConfigs();

	// ���ݿ���������
	CEdit dbType;
	// ���ݿ�DSN
	CEdit dbDSN;
	// ���ݿ��û���
	CEdit dbUID;
	// ���ݿ�����
	CEdit dbPWD;
	// ���վ����
	CEdit calibrationStationEdit;
	// ���ģʽ
	CEdit calibrationModeEdit;
	// ���������
	CEdit testerEdit;
	// ���������
	CEdit reviewerEdit;
	// ģ���ļ���
	CEdit templateFileNameEdit;
	// ģ����뷽ʽ
	CEdit encodingEdit;
	// ģ���ʽ
	CEdit formatEdit;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCheckShowPwd();
	CButton showPWDCheck;
	afx_msg void OnBnClickedTestDbConnection();
};
