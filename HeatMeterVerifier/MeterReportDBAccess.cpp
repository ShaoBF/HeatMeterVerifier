#include "stdafx.h"
#include "MeterReportDBAccess.h"
#include "MeterReport.h"
#include "Converter.h"

CMeterReportDBAccess::CMeterReportDBAccess()
{
	db = new CDatabase();
}


CMeterReportDBAccess::~CMeterReportDBAccess()
{
	if (db){
		delete db;
	}
	
}


BOOL CMeterReportDBAccess::Save(void* data){
	//获取要存储的内容
	MeterReport* report = (MeterReport*)data;
	//写成指令串
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
	//读取ID，按address和start_time值查询
	report->id = GetReportID(report);

	return TRUE;

}
BOOL CMeterReportDBAccess::Load(void* data){
	return TRUE;
}
BOOL CMeterReportDBAccess::Open(LPCTSTR src){
	BOOL result = TRUE;
	lpszConnect = src;
	result = db->Open(NULL, FALSE, FALSE, lpszConnect);
	return result;
}
BOOL CMeterReportDBAccess::Close(){
	db->Close();
	return TRUE;
}
UINT64 CMeterReportDBAccess::GetReportID(MeterReport* report){
	CString query;
	query.Format(L"select id from meter_reports where address='%s' and start_time='%s'", report->GetAddressStr(), Converter::BcdToDateTimeStr(report->startTime.value.puc, 7, true));
	CRecordset recordset(db);
	CString temp, record;
	UINT64 reportId = 0;
	recordset.Open(CRecordset::forwardOnly, query, CRecordset::readOnly);
	if (!recordset.IsEOF()){
		recordset.GetFieldValue((short)0, temp);
		temp.GetBuffer();
		LPTSTR lpsz = (LPTSTR)(LPCTSTR)temp;

		reportId = _wtoi64(lpsz);
	}
	return reportId;

}
