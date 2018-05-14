// MetersReportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "TestHtmlDlg.h"
#include "MetersReportDlg.h"
#include "MeterWizard.h"
#include "MeterReportDlg.h"
#include "afxdialogex.h"
#include <afxdb.h>
#include "Converter.h"
#include "MyVector.cpp"

#include "mysql.h"
#include <winsock.h>
#pragma comment(lib,"libmySQL.lib") 

// CMetersReportDlg �Ի���

IMPLEMENT_DYNAMIC(CMetersReportDlg, CDialogEx)

extern CMeterWizard wizard; 


CMetersReportDlg::CMetersReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMetersReportDlg::IDD, pParent)
{
	reportTable = new MeterReportTable();
	g_clsMutex = new CMutex(FALSE, L"DataBufferMutex", NULL);
}

CMetersReportDlg::~CMetersReportDlg()
{
	reportTable->ClearAllData();
	delete reportTable;
	if (g_clsMutex){
		delete g_clsMutex;
	}
	wizard.CloseAllCom();
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
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CMetersReportDlg::OnHdnItemdblclickReportList)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT_LIST, &CMetersReportDlg::OnNMDblclkReportList)
END_MESSAGE_MAP()


BOOL CMetersReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ������UI�ؼ�
	InitUIs();

	//�������ӵı��ַ��com�ڲ���ʾ
	UpdateMeterReportList();

	//��ϸ��Ϣ�б���ʽ��ʾ����

	return true;
}

void CMetersReportDlg::UpdateMeterReportList(){
	//��������
	g_clsMutex->Lock();
	meterReportList.DeleteAllItems();
	wizard.GetMeterReports();
	for (int i = 0; i < reportTable->GetRowCount(); i++){
		MeterReport* report = reportTable->GetReport(i);
		UCHAR* address = report->address;
		//�ж��Ƿ�ο���
		if (wizard.IsRefMeter(address)){//��Ϊ�ο�����һ�м�����
			meterReportList.InsertItem(i, L"*");//������
			//meterReadList.InsertItem(i, REF_METER_MARK);//������

		}
		else{//�����һ��Ϊ�հ�
			meterReportList.InsertItem(i, L" ");//������
		}
		int currentColumn = 1;
		meterReportList.SetItemText(i, currentColumn++, report->GetAddressStr());//������

		meterReportList.SetItemText(i, currentColumn++, report->GetHeatStr());//���ø��еĲ�ͬ�е���ʾ�ַ�
		meterReportList.SetItemText(i, currentColumn++, report->GetHeatPowerStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetCapacityStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetFlowRateStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetTemperatureInStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetTemperatureOutStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetStartTimeStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetEndTimeStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetDurationStr());
		meterReportList.SetItemText(i, currentColumn++, report->GetQualifiedStr());
	}
	g_clsMutex->Unlock();

}

void CMetersReportDlg::InitUIs(){
	//���ɱ�ͷ
	CreateMeterReportTable();

	meterReportList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CMetersReportDlg::CreateMeterReportTable(){
	meterReportList.DeleteAllItems();
	//���ɱ�ͷ
	for (int i = 0; i < reportTable->GetColumnCount(); i++){
		ColumnProperty* columnProperty = reportTable->GetColumnProperty(i);
		meterReportList.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//������
	}

}

// CMetersReportDlg ��Ϣ�������

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
	//���汨������
	/*MyVector<MeterReport*>* reports = reportTable->GetReports();
	if (reports != nullptr){
		SaveReports(reports);
	}*/
	MyVector<MeterInfo*>* meters = wizard.GetMeterInfoList();
	if (meters != nullptr){
		SaveMeterDataList(meters);
	}
}

void CMetersReportDlg::SaveReports(MyVector<MeterReport*>* reports){
	//�����ݿ�
	CDatabase db;
	BOOL result = TRUE;
	LPCTSTR lpszConnect = wizard.GetConnectStr();// _T("ODBC;DSN=HeatMeterDS32;UID=ShaoBF;PWD=shbofe");
	try {
		result = db.Open(NULL, FALSE, FALSE, lpszConnect);
		for (int i = 0; i < reports->GetSize(); i++){
			MeterReport* report = (*reports)[i];
			SaveReport(&db, report);
		}

		//�ر����ݿ�
		db.Close();
		MessageBox(L"����ɹ���");
	} catch (CDBException * e) {
		MessageBox(e->m_strError);
	}
}

void CMetersReportDlg::SaveReport(CDatabase* db, MeterReport* report){
	//�����ⱨ������
	CString str;
	str.Format(L"insert into meter_reports (is_reference, address, heat, heat_unit, heat_power, heat_power_unit, capacity, capacity_unit, flow_rate, flow_rate_unit, temperature_in, temperature_out, start_time, end_time, duration, is_qualified ) \
																values(%d,'%s',%f, %d, %f, %d, %f, %d, %f, %d, %f, %f, '%s', '%s',%d, %d)",
																0, report->GetAddressStr(),
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
	//��ȡID����address��start_timeֵ��ѯ
	report->id = GetReportID(db, report);
	//str.Format(L"select id from meter_reports where address='%s' and start_time='%s'", report->GetAddressStr(), Converter::BcdToDateTimeStr(report->startTime.value.puc, 7, true));
	//db->querySQL(str);
}

void CMetersReportDlg::SaveMeterDataList(MyVector<MeterInfo*>* meterList){
	//�����ݿ�
	CDatabase db;
	BOOL result = TRUE;
	LPCTSTR lpszConnect = wizard.GetConnectStr();// _T("ODBC;DSN=HeatMeterDS32;UID=ShaoBF;PWD=shbofe");
	try {
		result = db.Open(NULL, FALSE, FALSE, lpszConnect);
		for (int i = 0; i < meterList->GetSize(); i++){
			MeterDataInfo* meter = (MeterDataInfo*)(*meterList)[i];
			SaveMeterData(&db, meter);
		}

		//�ر����ݿ�
		db.Close();
		MessageBox(L"����ɹ���");
	}
	catch (CDBException * e) {
		MessageBox(e->m_strError);
	}

}
void CMetersReportDlg::SaveMeterData(CDatabase* db, MeterDataInfo* meterData){
	//�洢����
	SaveReport(db, meterData->GetReport());
	//��meter_reports�ж�ȡ��Ӧ����ID��
	//�����ȡ����
	//��ȡ��Ӧ��ʼ֡����֡
	DataFrame* startFrame = meterData->GetStartFrame();
	DataFrame* endFrame = meterData->GetEndFrame();
	//����
	SaveTestData(db, startFrame, meterData->GetReport()->id);
	SaveTestData(db, endFrame, meterData->GetReport()->id);

}


UINT64 CMetersReportDlg::GetReportID(CDatabase* database, MeterReport* report){
	CString query;
	query.Format(L"select id from meter_reports where address='%s' and start_time='%s'", report->GetAddressStr(), Converter::BcdToDateTimeStr(report->startTime.value.puc, 7, true));
	CRecordset recordset(database);
	CString temp, record;
	UINT64 reportId=0;
	recordset.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
	if (!recordset.IsEOF()){
		recordset.GetFieldValue((short)0, temp);
		temp.GetBuffer();
		LPTSTR lpsz = (LPTSTR)(LPCTSTR)temp;

		reportId = _wtoi64(lpsz);
	}
	return reportId;
	/*
	while (!recordset.IsEOF()){//is null
		record = _T("");
		register int len = recordset.GetODBCFieldCount();
		for (register int i = 0; i < len; i++){
			recordset.GetFieldValue(i, temp);
			record += temp + _T("   |   ");
		}
		//list.AddString(record);
		recordset.MoveNext();
	}
	*/

}

void CMetersReportDlg::SaveReport(MeterReport* report){
	//�����ݿ�
	CDatabase db;
	BOOL result = TRUE;
	LPCTSTR lpszConnect = wizard.GetConnectStr();// _T("ODBC;DSN=HeatMeterDS;UID=ShaoBF;PWD=shbofe");
	try {
		result = db.Open(NULL, FALSE, FALSE, lpszConnect);
	}
	catch (CDBException * e) {
		MessageBox(e->m_strError);
	}
	//�����ⱨ������
	SaveReport(&db, report);

	//�ر����ݿ�
	db.Close();

}

void CMetersReportDlg::SetMeterInfoList(MyVector<MeterInfo*>* dataInfoList){
	this->reportTable->dataInfoList = dataInfoList;
}

void CMetersReportDlg::OnHdnItemdblclickReportList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMetersReportDlg::OnNMDblclkReportList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int col = 0;
	int row = 0;
	//��ȡѡ�е��к�
	row = pNMListView->iItem;
	col = pNMListView->iSubItem;
	//˫��ĳһ�У���������֡�����Ի���
	if (row < 0){
		return;
	}
	//CMeterReportDlg dlg;
	CHtmlReportDlg dlg;
	//��ȡѡ���е�����
	MeterReport *data = reportTable->GetReport(row);
	//��ȡѡ���е�����
	//�������ݵ������Ի���
	dlg.SetMeterReport(data);
	dlg.SetRefMeterData((MeterDataInfo*)wizard.GetRefMeterInfo());

	//��ʾ�Ի���
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	*pResult = 0;
}

void CMetersReportDlg::SaveTestData(DataFrame* data, UINT64 testID){
	//�����ݿ�
	//�ر����ݿ�
	//�����ݿ�
	CDatabase db;
	BOOL result = TRUE;
	LPCTSTR lpszConnect = wizard.GetConnectStr();// _T("ODBC;DSN=HeatMeterDS32;UID=ShaoBF;PWD=shbofe");
	try {
		result = db.Open(NULL, FALSE, FALSE, lpszConnect);
		//�洢����Ӧ��
		SaveTestData(&db, data,testID);

		//�ر����ݿ�
		db.Close();
		MessageBox(L"����ɹ���");
	}
	catch (CDBException * e) {
		MessageBox(e->m_strError);
	}
}

void CMetersReportDlg::SaveTestData(CDatabase* db, DataFrame* data, UINT64 testID){
	//�����ⱨ������
	CString str;
	str.Format(L"insert into meter_test_data (test_id, address, is_reference_meter, billing_day_heat, billing_day_heat_unit, current_heat, current_heat_unit,\
				 heat_power, heat_power_unit, flow_rate, flow_rate_unit, current_capacity, current_capacity_unit,\
				  temperature_in, temperature_out, work_hours, read_time, status_word, raw_data ) \
					values(%llu,'%s',%d, %f, %d, %f, %d, \
					%f, %d, %f, %d, %f, %d, \
					%f, %f, %d, '%s', %d, '%s')",

					testID, data->GetAddressStr(),0, data->billingDayHeat.value.dv, data->billingDayHeat.unit, data->currentHeat.value.dv, data->currentHeat.unit,
					data->heatPower.value.dv, data->heatPower.unit, data->flowRate.value.dv, data->flowRate.unit, data->billingDayCapacity.value.dv, data->billingDayCapacity.unit,
					data->temperatureIn.value.dv, data->temperatureOut.value.dv, data->totalWorkHours.value.dwv, Converter::BcdToDateTimeStr(data->currentTime.value.puc, 7, true), 
					(UINT16)data->statusData.value.dwv, data->GetRawDataStr());
	db->ExecuteSQL(str);

}

