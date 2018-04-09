// MetersReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MetersReportDlg.h"
#include "MeterWizard.h"
#include "afxdialogex.h"
#include <afxdb.h>
#include "Converter.h"

#include "mysql.h"
#include <winsock.h>
#pragma comment(lib,"libmySQL.lib") 

// CMetersReportDlg 对话框

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
	ON_BN_CLICKED(IDC_SAVE, &CMetersReportDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CMetersReportDlg 消息处理程序

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


void CMetersReportDlg::OnBnClickedSave()
{
	//保存报告数据
	if (reports != nullptr){
		SaveReports(reports);
	}
	//保存读取数据
	//SaveReadData();
}

void CMetersReportDlg::SaveReports(MyVector<MeterReport*>* reports){
	//打开数据库
	CDatabase db;
	BOOL result = TRUE;
	LPCTSTR lpszConnect = wizard.GetConnectStr();// _T("ODBC;DSN=HeatMeterDS32;UID=ShaoBF;PWD=shbofe");
	try {
		result = db.Open(NULL, FALSE, FALSE, lpszConnect);
		for (int i = 0; i < reports->GetSize(); i++){
			SaveReport(&db, (*reports)[i]);
		}

		//关闭数据库
		db.Close();
	} catch (CDBException * e) {
		MessageBox(e->m_strError);
	}
}

void CMetersReportDlg::SaveReport(CDatabase* db, MeterReport* report){
	//保存检测报告数据
	CString str;
	str.Format(L"insert into meter_reports (is_reference, address, heat, heat_unit, heat_power, heat_power_unit, capacity, capacity_unit, flow_rate, flow_rate_unit, temperature_in, temperature_out, start_time, end_time, duration, is_qualified ) \
																values(%d,'%s',%f, %d, %f, %d, %f, %d, %f, %d, %f, %f, '%s', '%s',%d, %d)",
																0, Converter::HexToString(report->address, 7,0),
																report->heat.value.dv, report->heat.unit,
																report->heatPower.value.dv, report->heatPower.unit,
																report->capacity.value.dv, report->capacity.unit,
																report->flowRate.value.dv, report->flowRate.unit,
																report->temperatureIn.value.dv,
																report->temperatureOut.value.dv,
																Converter::BcdToDateTimeStr(report->startTime.value.puc, 7, true),
																Converter::BcdToDateTimeStr(report->endTime.value.puc, 7, true),
																report->duration.value.dwv,
																1);
	db->ExecuteSQL(str);
}
void CMetersReportDlg::SaveReport(MeterReport* report){
	//打开数据库
	CDatabase db;
	BOOL result = TRUE;
	LPCTSTR lpszConnect = wizard.GetConnectStr();// _T("ODBC;DSN=HeatMeterDS;UID=ShaoBF;PWD=shbofe");
	try {
		result = db.Open(NULL, FALSE, FALSE, lpszConnect);
	}
	catch (CDBException * e) {
		MessageBox(e->m_strError);
	}
	//保存检测报告数据
	SaveReport(&db, report);

	//关闭数据库
	db.Close();

}

void CMetersReportDlg::SetReports(MyVector<MeterReport*>* reports){
	this->reports = reports;
}