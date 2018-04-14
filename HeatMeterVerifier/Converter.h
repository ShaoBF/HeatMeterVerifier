#pragma once

#include <afxstr.h>
#include <time.h>




class Converter
{
public:
	Converter();
	virtual ~Converter();

	static char HexToChar(char hex);

	static CString HexToString(UCHAR* buf, DWORD bufferLen, char seperator = ' ');
	static char* StringToHex(CString dataStr, char seperator = ' ');

	//��BCD��ת��Ϊ����
	//data����ת��BCD��
	//length��BCD���ֽڳ�
	//highByteFirst���ֽ�˳�򡪡����ֽ���ǰ���ǵ��ֽ���ǰ
	static DWORD BcdToNumber(UCHAR* data, int length, bool highByteFirst = true );
	static tm BcdToTime(UCHAR* data, int length, bool highByteFirst = true);
	static time_t BcdToTimeT(UCHAR* data, int length, bool highByteFirst = true);
	static CString BcdToDateTimeStr(UCHAR* data, int length, bool highByteFirst = true);
};

