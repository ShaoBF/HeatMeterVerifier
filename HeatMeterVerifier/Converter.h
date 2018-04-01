#pragma once

#include <afxstr.h>




class Converter
{
public:
	Converter();
	virtual ~Converter();

	static char HexToChar(char hex);

	static CString HexToString(UCHAR* buf, DWORD bufferLen, char seperator = ' ');
	static char* StringToHex(CString dataStr);

	//��BCD��ת��Ϊ����
	//data����ת��BCD��
	//length��BCD���ֽڳ�
	//highByteFirst���ֽ�˳�򡪡����ֽ���ǰ���ǵ��ֽ���ǰ
	static DWORD BcdToNumber(UCHAR* data, int length, bool highByteFirst = true );
};

