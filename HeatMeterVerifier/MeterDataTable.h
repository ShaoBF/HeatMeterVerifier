#pragma once
#include "CJ188.h"
#include "DataFrame.h"
#include "MyVector.h"
#include "ColumnProperty.h"

#define MAX_ROWS 100


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
	DataFrame* RemoveDataFrame(int index);
	bool ClearAllData();

protected:
	ColumnProperty** columnPropertyList;
	int columnCount;
	//DataFrame** frames;
	//int frameCount;
	MyVector<DataFrame*>* frames;
};

