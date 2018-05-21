// OtherSettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "OtherSettingsDlg.h"
#include "afxdialogex.h"
#include "MeterWizard.h"
#include "CalibratorConfig.h"
#include <afxdb.h>
#include "Converter.h"

// COtherSettingsDlg �Ի���

IMPLEMENT_DYNAMIC(COtherSettingsDlg, CDialogEx)

COtherSettingsDlg::COtherSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COtherSettingsDlg::IDD, pParent)
{

}

COtherSettingsDlg::~COtherSettingsDlg()
{
}

void COtherSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DB_TYPE, dbType);
	DDX_Control(pDX, IDC_DB_DSN, dbDSN);
	DDX_Control(pDX, IDC_DB_UID, dbUID);
	DDX_Control(pDX, IDC_DB_PWD, dbPWD);
	DDX_Control(pDX, IDC_CALI_STATION, calibrationStationEdit);
	DDX_Control(pDX, IDC_CALI_TYPE, calibrationModeEdit);
	DDX_Control(pDX, IDC_CALI_TESTER, testerEdit);
	DDX_Control(pDX, IDC_CALI_REVIEWER, reviewerEdit);
	DDX_Control(pDX, IDC_REPORT_TEMPLATE, templateFileNameEdit);
	DDX_Control(pDX, IDC_REPORT_ENCODE, encodingEdit);
	DDX_Control(pDX, IDC_REPORT_FORMAT, formatEdit);
	DDX_Control(pDX, IDC_CHECK_SHOW_PWD, showPWDCheck);
}


BEGIN_MESSAGE_MAP(COtherSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &COtherSettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &COtherSettingsDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PWD, &COtherSettingsDlg::OnBnClickedCheckShowPwd)
	ON_BN_CLICKED(IDC_TEST_DB_CONNECTION, &COtherSettingsDlg::OnBnClickedTestDbConnection)
END_MESSAGE_MAP()


// COtherSettingsDlg ��Ϣ�������

extern CMeterWizard wizard;

BOOL COtherSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitConfig();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void COtherSettingsDlg::InitConfig(){
	CDBConfig* dbCongfig = wizard.dbConfig;
	CCalibratorInfo* caliInfo = wizard.calibratorInfo;
	TemplateConfig* templateConfig = wizard.templateConfig;

	//���ݿ����ó�ʼ��
	dbType.SetWindowTextW(dbCongfig->type);
	dbDSN.SetWindowTextW(dbCongfig->dsn);
	dbUID.SetWindowTextW(dbCongfig->uid);
	dbPWD.SetWindowTextW(dbCongfig->pwd);

	//���վ���ó�ʼ��
	calibrationStationEdit.SetWindowTextW(caliInfo->name);
	calibrationModeEdit.SetWindowTextW(caliInfo->calibrateMode);
	testerEdit.SetWindowTextW(caliInfo->tester);
	reviewerEdit.SetWindowTextW(caliInfo->reviewer);

	//����ģ�����ó�ʼ��
	templateFileNameEdit.SetWindowTextW(templateConfig->fileName);
	encodingEdit.SetWindowTextW(templateConfig->encoding);
	formatEdit.SetWindowTextW(templateConfig->format);
}


void COtherSettingsDlg::OnBnClickedOk()
{
	//������������
	OnBnClickedButtonApply();
	//��������
	CDialogEx::OnOK();
}


void COtherSettingsDlg::OnBnClickedButtonApply()
{
	//������������
	if (SaveConfigs()){
		AfxMessageBox(L"���ñ���ɹ���");
	}
	else{
		AfxMessageBox(L"���ñ���ʧ�ܣ�");
	}
}

BOOL COtherSettingsDlg::SaveConfigs(){
	CDBConfig* dbCongfig = wizard.dbConfig;
	CCalibratorInfo* caliInfo = wizard.calibratorInfo;
	TemplateConfig* templateConfig = wizard.templateConfig;

	//���ݿ�����
	dbType.GetWindowTextW(dbCongfig->type);
	dbDSN.GetWindowTextW(dbCongfig->dsn);
	dbUID.GetWindowTextW(dbCongfig->uid);
	dbPWD.GetWindowTextW(dbCongfig->pwd);

	//���վ����
	calibrationStationEdit.GetWindowTextW(caliInfo->name);
	calibrationModeEdit.GetWindowTextW(caliInfo->calibrateMode);
	testerEdit.GetWindowTextW(caliInfo->tester);
	reviewerEdit.GetWindowTextW(caliInfo->reviewer);

	//����ģ������
	templateFileNameEdit.GetWindowTextW(templateConfig->fileName);
	encodingEdit.GetWindowTextW(templateConfig->encoding);
	formatEdit.GetWindowTextW(templateConfig->format);

	//��������
	BOOL result = TRUE;
	result=result&&(dbCongfig->SaveConfig());
	result = result&&(caliInfo->SaveConfig());
	result = result&&(templateConfig->SaveConfig());

	return result;
}

void COtherSettingsDlg::OnBnClickedCheckShowPwd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BOOL show = showPWDCheck.GetCheck();
	if (show){
		//dbPWD.pass
	}
}


void COtherSettingsDlg::OnBnClickedTestDbConnection()
{
	char buffer[256];
	CString type,dsn,uid,pwd;

	dbType.GetWindowTextW(type);
	dbDSN.GetWindowTextW(dsn);
	dbUID.GetWindowTextW(uid);
	dbPWD.GetWindowTextW(pwd);

	sprintf(buffer, "%ws;DSN=%ws;UID=%ws;PWD=%ws", type, dsn, uid, pwd);
	/*int num = MultiByteToWideChar(0, 0, buffer, -1, NULL, 0);
	MultiByteToWideChar(0, 0, buffer, -1, wide, num);*/
	LPCTSTR wide = Converter::pcharToWchar(buffer);

	CDatabase db;
	LPCTSTR lpszConnect;

	BOOL result = TRUE;
	lpszConnect = wide;
	result = db.Open(NULL, FALSE, FALSE, lpszConnect);
	if (result){
		AfxMessageBox(L"���Ӳ��Գɹ���");
		db.Close();
	}
	else{
		AfxMessageBox(L"���Ӳ����ã�");
	}

}
