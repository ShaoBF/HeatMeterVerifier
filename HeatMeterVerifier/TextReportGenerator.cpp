#include "stdafx.h"
#include "TextReportGenerator.h"


CTextReportGenerator::CTextReportGenerator()
{
}


CTextReportGenerator::~CTextReportGenerator()
{
}

int CopyBuffer(WCHAR *buffer, CString *src, int startIndex, int endIndex){
	int count = 0;
	for (int i = startIndex; i < endIndex; i++){
		buffer[count++] = src->GetAt(i);
	}
	return count;
}


void* CTextReportGenerator::GenerateReport(void* tplContent, CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap){
	//
	CString* templateContent = (CString*)tplContent;

	WCHAR* key;
	CString keystr;
	//CString* value;
	int length;
	WCHAR* buffer = new WCHAR[templateContent->GetLength() + 500];
	int startPos, endPos;

	int srcIndex = 0;
	int bufIndex = 0;
	TCHAR tchar;
	int srcLength = templateContent->GetLength();

	//确定关键字串key
	//起始串为"<%="，结束串为"%>"
	//获取起始和结束位置下标startPos，endPos
	CString keyStart = L"<%=";
	CString keyEnd = L"%>";


	srcIndex = 0;
	while (srcIndex < srcLength){
		//寻找"<%"位置
		startPos = templateContent->Find(keyStart, srcIndex);
		if (startPos != -1){
			//将srcIndex到startPos之间内容全部写入buffer
			int count = CopyBuffer(&(buffer[bufIndex]), templateContent, srcIndex, startPos);
			srcIndex = startPos;
			bufIndex += count;

			//寻找"%>"位置
			endPos = templateContent->Find(keyEnd, srcIndex) + 2;
			//若没有"%>"继续复制到结束
			if (endPos == -1){
				break;
			}
			else{//将区间内key对应value写入buffer
				int keyStart = startPos + 3;
				int keyEnd = endPos - 2;
				//取出"<%="到"%>"之间内容
				keystr = templateContent->Mid(keyStart, keyEnd - keyStart);
				//去除该内容空格
				keystr.Trim();
				//从dataMap中找到key对应value
				//void* value = new CString();
				//void* vv = new CString();
				BOOL b;
				//CString keystr(L"calibrationStation");
				CString value;
				b = dataMap->Lookup(keystr.GetBuffer(), value);
				//CString* value = (CString*)vv;

				if (b){
					//若找到，将value加入buffer

					int count = CopyBuffer(&(buffer[bufIndex]), &value, 0, value.GetLength());
					bufIndex += count;
				}
				else{
					//否则，整段复制
					int count = CopyBuffer(&(buffer[bufIndex]), templateContent, startPos, endPos);
					bufIndex += count;
				}
				srcIndex = endPos;
			}
		}
		else{
			break;
		}
	}
	// 复制剩余内容
	int count = CopyBuffer(&(buffer[bufIndex]), templateContent, srcIndex, srcLength);
	bufIndex += count;
	buffer[bufIndex++] = 0;
	CString* content = new CString(buffer);
	delete buffer;
	//

	return content;
}