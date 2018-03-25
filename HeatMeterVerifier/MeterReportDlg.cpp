// MeterReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterReportDlg.h"
#include "afxdialogex.h"


// CMeterReportDlg 对话框

IMPLEMENT_DYNAMIC(CMeterReportDlg, CDialogEx)

CMeterReportDlg::CMeterReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterReportDlg::IDD, pParent)
{

}

CMeterReportDlg::~CMeterReportDlg()
{
}

void CMeterReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMeterReportDlg, CDialogEx)
END_MESSAGE_MAP()


// CMeterReportDlg 消息处理程序
