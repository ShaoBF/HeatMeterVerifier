#include "stdafx.h"
#include "Converter.h"


Converter::Converter()
{
}


Converter::~Converter()
{
}

char Converter::HexToChar(char hex){
	if (hex < 10 && hex >= 0){
		return hex + '0';
	}
	else if (hex >= 10 && hex < 16){
		return hex - 10 + 'A';
	}
}

CString Converter::HexToString(UCHAR* buf, DWORD bufferLen, char seperator){
	int dataLen = bufferLen * 2 + 1;
	if (seperator != 0){
		dataLen += bufferLen;
	}
	char* hexStr = new char[dataLen];
	int j = 0;
	unsigned char a = buf[0];
	for (int i = 0; i < bufferLen; i++){
		a = buf[i];
		int a1 = a / 16;
		int a2 = a % 16;
		hexStr[j++] = HexToChar(a1);
		hexStr[j++] = HexToChar(a2);
		if (seperator != 0)
			hexStr[j++] = seperator;
	}
	hexStr[j] = '\0';
	return CString(hexStr);
}
char* Converter::StringToHex(CString dataStr){
	size_t len = dataStr.GetLength();
	int dataLen = ((len + 1) / 3) + 1;
	char* data = new char[dataLen];
	//������������ת��Ϊʮ������ָ��/����
	int j = 0;
	data[0] = 0;
	for (int i = 0; i < len; i++){
		char c = dataStr.GetAt(i);
		switch (c){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			data[j] = data[j] * 16 + c - '0';
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			data[j] = data[j] * 16 + c - 'a' + 10;
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			data[j] = data[j] * 16 + c - 'A' + 10;
			break;
		case ' ':
			data[++j] = 0;
			break;
		}
	}
	data[++j] = 0;
	return data;
}

DWORD Converter::BcdToNumber(UCHAR* data, int length
	, bool highByteFirst//�ֽ�˳�򡪡����ֽ���ǰ���ǵ��ֽ���ǰ
	){

	DWORD number = 0;
	UINT currentDigit = 0;
	if (highByteFirst){
		for (int i = 0; i < length; i++){
			currentDigit = (data[i] & 0xF0) >> 4;
			number *= 10;
			number += currentDigit;
			currentDigit = data[i] & 0x0F;
			number *= 10;
			number += currentDigit;
		}
	}
	else{
		for (int i = length - 1; i >= 0; i--){
			currentDigit = (data[i] & 0xF0) >> 4;
			number *= 10;
			number += currentDigit;
			currentDigit = data[i] & 0x0F;
			number *= 10;
			number += currentDigit;
		}
	}
	return number;
}

