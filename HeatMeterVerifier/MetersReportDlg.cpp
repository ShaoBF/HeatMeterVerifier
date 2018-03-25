// MetersReportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MetersReportDlg.h"
#include "MeterWizard.h"
#include "afxdialogex.h"


// CMetersReportDlg �Ի���

IMPLEMENT_DYNAMIC(CMetersReportDlg, CDialogEx)

CMetersReportDlg::CMetersReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMetersReportDlg::IDD, pParent)
{

}

CMetersReportDlg::~CMetersReportDlg()
{
}

void CMetersReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_LIST, meterReportList);
}


BEGIN_MESSAGE_MAP(CMetersReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMetersReportDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDCANCEL, &CMetersReportDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMetersReportDlg ��Ϣ�������

extern CMeterWizard wizard;
void CMetersReportDlg::OnBnClickedPrevious()
{
	this->EndDialog(1);
	wizard.ReadMeters();
}


void CMetersReportDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	//wizard.ReadMeters();
}
