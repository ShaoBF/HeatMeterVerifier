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

	//将BCD码转换为整数
	//data：待转换BCD码
	//length，BCD码字节长
	//highByteFirst，字节顺序——高字节在前还是低字节在前
	static DWORD BcdToNumber(UCHAR* data, int length, bool highByteFirst = true );
	static tm BcdToTime(UCHAR* data, int length, bool highByteFirst = true);
	static time_t BcdToTimeT(UCHAR* data, int length, bool highByteFirst = true);
	static CString BcdToDateTimeStr(UCHAR* data, int length, bool highByteFirst = true);
};

