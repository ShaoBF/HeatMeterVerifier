#include "stdafx.h"
#include "MeterDataDBAccess.h"
#include "DataFrame.h"
#include "Converter.h"

CMeterDataDBAccess::CMeterDataDBAccess() :CDataBaseAccess()
{
	
}


CMeterDataDBAccess::~CMeterDataDBAccess()
{
}
BOOL CMeterDataDBAccess::Save(void* meterData,UINT64 testID){
		//保存检测报告数据
	CString str;
	DataFrame* data = (DataFrame*)meterData;
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

	return TRUE;
}
BOOL CMeterDataDBAccess::Save(void* meterData){
	return TRUE;
}


BOOL CMeterDataDBAccess::Load(void* data){
	return TRUE;
}
BOOL CMeterDataDBAccess::Open(LPCTSTR src){
	return CDataBaseAccess::Open(src);
}
BOOL CMeterDataDBAccess::Close(){
	return CDataBaseAccess::Close();
}
