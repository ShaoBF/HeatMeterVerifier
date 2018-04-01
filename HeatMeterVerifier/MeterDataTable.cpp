#include "stdafx.h"
#include "MeterDataTable.h"

ColumnProperty::ColumnProperty(){
	
}

ColumnProperty::~ColumnProperty(){

}

ColumnProperty::ColumnProperty(CString name, int format, int width){
	this->name = name;
	this->format = format;
	this->width = width;
}


//////////////////////////////////////////////////////////////////////////////////////
/*表号10字符
厂家号4字符
累计热量999999.99格式数值
累计热量单位1字节整数
热功率999999.99格式数值
热功率单位1字节整数
流速9999.9999格式数值
流速单位1字节整数
累计流量999999.99格式数值
累计流量单位1字节整数
入口温度9999.99型数值（默认单位℃）
出口温度9999.99型数值（默认单位℃）
运行时间999999型数值（小时）
实际时间按日期格式存储（显示YYYY - MM - DD - hh - mm - ss）
状态字ST，2字节整数
*/
MeterDataTable::MeterDataTable()
{
	columnCount = 12;
	columnPropertyList = new ColumnProperty*[columnCount];
	int current=0;
	ColumnProperty* column;
	column = new ColumnProperty(L"表号地址", LVCFMT_LEFT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"结算日热量", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"累计热量", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"热功率", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"流速", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"累计流量", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"入口温度(℃)", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"出口温度(℃)", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"运行时间（h）", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"实际时间", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"状态字", LVCFMT_RIGHT, 100);
	columnPropertyList[current++] = column;
	column = new ColumnProperty(L"原数据帧", LVCFMT_LEFT, 100);
	columnPropertyList[current++] = column;
	frameCount = 0;
	frames = new DataFrame*[MAX_ROWS];
}


MeterDataTable::~MeterDataTable()
{
	for (int i = 0; i < columnCount; i++){
		delete columnPropertyList[i];
	}
	delete columnPropertyList;
}

int MeterDataTable::GetColumnCount(){
	return columnCount;
}

ColumnProperty* MeterDataTable::GetColumnProperty(int index){
	return columnPropertyList[index];
}
void MeterDataTable::AddData(DataFrame *dataFrame){
	this->frames[frameCount++] = dataFrame;
}

int MeterDataTable::GetRowCount(){
	return frameCount;
}

DataFrame* MeterDataTable::GetDataFrame(int index){
	if (index >= 0 && index < frameCount){
		return frames[index];
	}
	//throw index;
	return nullptr;
}