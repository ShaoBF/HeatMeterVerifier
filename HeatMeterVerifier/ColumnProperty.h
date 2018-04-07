#pragma once
class ColumnProperty
{
public:
	ColumnProperty();
	virtual ~ColumnProperty();
	ColumnProperty(CString name, int format, int width);

	CString name;		//显示的列名
	int width;			//显示宽度
	int format;			//显示对齐方式
};

