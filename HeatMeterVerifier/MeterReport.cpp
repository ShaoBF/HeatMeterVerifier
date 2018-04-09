#include "stdafx.h"
#include "MeterReport.h"
#include "Converter.h"


MeterReport::MeterReport()
{
}


MeterReport::~MeterReport()
{
}
MeterReport::MeterReport(DataFrame* startFrame, DataFrame* endFrame){

	Calculate(startFrame, endFrame);
}

void MeterReport::Calculate(DataFrame* startFrame, DataFrame* endFrame){
	DataItem* start;
	DataItem* end;
	UnionValue value;

	start = &(startFrame->currentHeat);
	end = &(endFrame->currentHeat);
	value.dv = end->value.dv - start->value.dv;
	heat.SetData(value, end->decimalPoint, end->hasUnit, end->unit);

	start = &(startFrame->billingDayCapacity);
	end = &(endFrame->billingDayCapacity);
	value.dv = end->value.dv - start->value.dv;
	capacity.SetData(value, end->decimalPoint, end->hasUnit, end->unit);

	//FIXME:数值取值具体方式待定
	start = &(startFrame->heatPower);
	end = &(endFrame->heatPower);
	value.dv = end->value.dv; //- start->value.dv;
	heatPower.SetData(value, end->decimalPoint, end->hasUnit, end->unit);

	//FIXME:数值取值具体方式待定
	start = &(startFrame->flowRate);
	end = &(endFrame->flowRate);
	value.dv = end->value.dv; //- start->value.dv;
	flowRate.SetData(value, end->decimalPoint, end->hasUnit, end->unit);

	start = &(startFrame->currentTime);
	end = &(endFrame->currentTime);
	value.puc = start->value.puc;
	startTime.SetData(value, end->decimalPoint, end->hasUnit, end->unit);
	value.puc = end->value.puc;
	endTime.SetData(value, end->decimalPoint, end->hasUnit, end->unit);

	/*UCHAR meterType = endFrame->frame->meterType;
	UCHAR* vandorID = &(endFrame->frame->address[5]);*/
	//bool highByteFirst = CJ188::LookUpByteOrder(meterType, vandorID);
	bool highByteFirst = true;


	time_t s = Converter::BcdToTimeT(start->value.puc, 7, highByteFirst);
	time_t e = Converter::BcdToTimeT(end->value.puc, 7, highByteFirst);
	duration.value.dwv = e - s;

	memcpy(address, startFrame->frame->address, CJ188_ADDRESS_LENGTH);
}
