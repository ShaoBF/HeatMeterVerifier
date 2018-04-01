#pragma once
#include "CJ188.h"
#include "DataFrame.h"


#define MAX_ROWS 100

class ColumnProperty{
public:
	ColumnProperty();
	~ColumnProperty();
	ColumnProperty(CString name, int format, int width);

	CString name;		//显示的列名
	int width;			//显示宽度
	int format;			//显示对齐方式
};

class MeterDataTable
{
public:
	MeterDataTable();
	virtual ~MeterDataTable();
	int GetColumnCount();
	ColumnProperty* GetColumnProperty(int index);
	void AddData(DataFrame *dataFrame);
	int GetRowCount();
	DataFrame* GetDataFrame(int index);

protected:
	ColumnProperty** columnPropertyList;
	int columnCount;
	DataFrame** frames;
	int frameCount;
};

