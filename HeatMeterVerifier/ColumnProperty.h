#pragma once
class ColumnProperty
{
public:
	ColumnProperty();
	virtual ~ColumnProperty();
	ColumnProperty(CString name, int format, int width);

	CString name;		//��ʾ������
	int width;			//��ʾ���
	int format;			//��ʾ���뷽ʽ
};

