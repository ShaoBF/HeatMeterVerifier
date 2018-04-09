// MeterReportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterReportDlg.h"
#include "afxdialogex.h"


// CMeterReportDlg �Ի���

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
	DDX_Control(pDX, IDC_TEST_UNIT, testUnit);
	DDX_Control(pDX, IDC_TEST_MODE, testMode);
	DDX_Control(pDX, IDC_METERID, meterID);
	DDX_Control(pDX, IDC_MANUFATORY, factoryID);
	DDX_Control(pDX, IDC_METER_TYPE, meterType);
	DDX_Control(pDX, IDC_TEST_START_TIME, startTime);
	DDX_Control(pDX, IDC_TEST_END_TIME, endTime);
	DDX_Control(pDX, IDC_TESTER, tester);
	DDX_Control(pDX, IDC_REVIEWER, reviewer);
}


BEGIN_MESSAGE_MAP(CMeterReportDlg, CDialogEx)
	ON_EN_CHANGE(IDC_METER_TYPE, &CMeterReportDlg::OnEnChangeMeterType)
END_MESSAGE_MAP()


// CMeterReportDlg ��Ϣ�������


void CMeterReportDlg::OnEnChangeMeterType()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
