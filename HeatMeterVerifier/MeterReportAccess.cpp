#include "stdafx.h"
#include "MeterReportAccess.h"


CMeterReportAccess::CMeterReportAccess()
{
}


CMeterReportAccess::~CMeterReportAccess()
{
}

/*BOOL CMeterReportAccess::Save(void* data){
	//��ȡҪ�洢������
	MeterReport* report = (MeterReport*)data;
	//д��ָ�
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

	return TRUE;
}

BOOL CMeterReportAccess::Load(void* data){
	return TRUE;
}*/
