// MeterReadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterReadDlg.h"
#include "afxdialogex.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"


// CMeterReadDlg 对话框

IMPLEMENT_DYNAMIC(CMeterReadDlg, CDialogEx)

CMeterReadDlg::CMeterReadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterReadDlg::IDD, pParent)
{

}

CMeterReadDlg::~CMeterReadDlg()
{
}

void CMeterReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_METERS_LIST, MeterReadList);
}


BEGIN_MESSAGE_MAP(CMeterReadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMeterReadDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDOK, &CMeterReadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMeterReadDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMeterReadDlg 消息处理程序

extern CMeterWizard wizard;

BOOL CMeterReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	return true;
}


void CMeterReadDlg::OnBnClickedPrevious()
{
	this->EndDialog(1);
	wizard.ChooseMeters();
}


void CMeterReadDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	wizard.GenerateReports();
}


void CMeterReadDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	//wizard.ChooseMeters();
}
