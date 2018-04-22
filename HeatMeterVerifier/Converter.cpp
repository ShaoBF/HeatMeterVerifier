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
char* Converter::StringToHex(CString dataStr, char seperator){
	size_t len = dataStr.GetLength();
	int dataLen = ((len + 1) / 3) + 1;
	char* data = new char[dataLen];
	//将发送区内容转换为十六进制指令/数据
	int j = 0;
	data[0] = 0;
	for (int i = 0; i < len; i++){
		char c = dataStr.GetAt(i);
		if (c == seperator){
			data[++j] = 0;
		}
		else{
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
	}
	data[++j] = 0;
	return data;
}

DWORD Converter::BcdToNumber(UCHAR* data, int length
	, bool highByteFirst//字节顺序——高字节在前还是低字节在前
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

tm Converter::BcdToTime(UCHAR* data, int length, bool highByteFirst){
	tm t;
	if (highByteFirst){
		int current = 0;
		t.tm_year = BcdToNumber(&(data[current]), 2, highByteFirst)-1900;
		current += 2;
		t.tm_mon = BcdToNumber(&(data[current++]), 1, highByteFirst)-1;
		t.tm_mday = BcdToNumber(&(data[current++]), 1, highByteFirst);
		t.tm_hour = BcdToNumber(&(data[current++]), 1, highByteFirst);
		t.tm_min = BcdToNumber(&(data[current++]), 1, highByteFirst);
		t.tm_sec = BcdToNumber(&(data[current++]), 1, highByteFirst);
		t.tm_isdst = 0;
	}
	else{
		int current = 6;
		t.tm_year = BcdToNumber(&(data[current]), 2, highByteFirst) - 1900;
		current -= 2;
		t.tm_mon = BcdToNumber(&(data[current--]), 1, highByteFirst) - 1;
		t.tm_mday = BcdToNumber(&(data[current--]), 1, highByteFirst);
		t.tm_hour = BcdToNumber(&(data[current--]), 1, highByteFirst);
		t.tm_min = BcdToNumber(&(data[current--]), 1, highByteFirst);
		t.tm_sec = BcdToNumber(&(data[current--]), 1, highByteFirst);
		t.tm_isdst = 0;
	}
	return t;
}

time_t Converter::BcdToTimeT(UCHAR* data, int length, bool highByteFirst){
	tm t = BcdToTime(data, length, highByteFirst);
	time_t t_of_day = mktime(&t);
	return t_of_day;
}
CString Converter::BcdToDateTimeStr(UCHAR* data, int length, bool highByteFirst){
	tm t = BcdToTime(data, length, highByteFirst);
	CString str;
	str.Format(L"%04d-%02d-%02d %02d:%02d:%02d", t.tm_year+1900, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	return str;
}

int Converter::StringToNumber(CString dataStr){
	size_t len = dataStr.GetLength();
	bool isNegative = false;
	int number = 0;
	//将发送区内容转换为十六进制指令/数据
	int i = 0;
	int j = 0;
	char c = dataStr.GetAt(i++);
	while (c == '-'){
		isNegative = !isNegative;
		c = dataStr.GetAt(i++);
	}
	for (; i < len; i++){
		char c = dataStr.GetAt(i);
		number = number * 10 + c - '0';
	}
	return number;
}

UCHAR* Converter::ChangeByteOrder(UCHAR* data, int length){
	UCHAR* result = new UCHAR[length];
	for (int i = 0; i < length; i++){
		result[i] = data[length-1-i];
	}
	return result;
}