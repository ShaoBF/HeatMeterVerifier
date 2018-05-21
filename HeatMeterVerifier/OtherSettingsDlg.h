#pragma once
#include "afxwin.h"


// COtherSettingsDlg 对话框

class COtherSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COtherSettingsDlg)

public:
	COtherSettingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COtherSettingsDlg();

// 对话框数据
	enum { IDD = IDD_OTHER_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void InitConfig();
	BOOL SaveConfigs();

	// 数据库类型配置
	CEdit dbType;
	// 数据库DSN
	CEdit dbDSN;
	// 数据库用户名
	CEdit dbUID;
	// 数据库密码
	CEdit dbPWD;
	// 监测站名称
	CEdit calibrationStationEdit;
	// 检测模式
	CEdit calibrationModeEdit;
	// 检测者名称
	CEdit testerEdit;
	// 审核者名称
	CEdit reviewerEdit;
	// 模板文件名
	CEdit templateFileNameEdit;
	// 模板编码方式
	CEdit encodingEdit;
	// 模板格式
	CEdit formatEdit;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCheckShowPwd();
	CButton showPWDCheck;
	afx_msg void OnBnClickedTestDbConnection();
};
