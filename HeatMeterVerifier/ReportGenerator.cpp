#include "stdafx.h"
#include "ReportGenerator.h"


CReportGenerator::CReportGenerator()
{
}


CReportGenerator::~CReportGenerator()
{
}

int CopyBuffer(WCHAR *buffer, CString src, int startIndex, int endIndex){
	int count = 0;
	for (int i = startIndex; i < endIndex; i++){
		buffer[count++] = src.GetAt(i);
	}
	return count;
}


CString CReportGenerator::GenerateReport(CString templateContent, CMap<CString, LPCTSTR, CString, LPCTSTR>* dataMap){
	//
	CString reportContent = templateContent;

	WCHAR* key;
	CString keystr, tempstr;
	CString value;
	int length;
	WCHAR* buffer = new WCHAR[templateContent.GetLength() + 500];
	int startPos, endPos;

	int srcIndex = 0;
	int bufIndex = 0;
	TCHAR tchar;
	int srcLength = templateContent.GetLength();

	//确定关键字串key
	//起始串为"<%="，结束串为"%>"
	//获取起始和结束位置下标startPos，endPos
	CString keyStart = L"<%=";
	CString keyEnd = L"%>";

	srcIndex = 0;
	while (srcIndex < srcLength){
		//寻找"<%"位置
		startPos = templateContent.Find(keyStart, srcIndex);
		if (startPos != -1){
			//将srcIndex到startPos之间内容全部写入buffer
			int count = CopyBuffer(&(buffer[bufIndex]), templateContent, srcIndex, startPos);
			srcIndex = startPos;
			bufIndex += count;

			//寻找"%>"位置
			endPos = templateContent.Find(keyEnd, srcIndex) + 2;
			//若没有"%>"继续复制到结束
			if (endPos == -1){
				break;
			}
			else{//将区间内key对应value写入buffer
				int keyStart = startPos + 3;
				int keyEnd = endPos - 2;
				//取出"<%="到"%>"之间内容
				keystr = templateContent.Mid(keyStart, keyEnd - keyStart);
				//去除该内容空格
				keystr.Trim();
				//从dataMap中找到key对应value
				if (dataMap->Lookup(keystr.GetBuffer(), value)){
					//若找到，将value加入buffer
					int count = CopyBuffer(&(buffer[bufIndex]), value, 0, value.GetLength());
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


	//从头开始，将模板逐字符复制进新buffer
	/*int srcIndex = 0;
	int bufIndex = 0;
	TCHAR tchar;
	int srcLength = templateContent.GetLength();
	while ((tchar = templateContent.GetAt(srcIndex)) != '\0'){
		//遇到'<'时，查看下一个字符是否为'%'
		if (tchar == '<'){
			//若为"<%"组合，下标作为待替换内容起始位置
			if (templateContent.GetAt(srcIndex + 1) == '%'){
				startPos = srcIndex;
				//寻找"%>"作为结束位置
				//找到下一个'%'位置
				while (srcIndex < srcLength){
					while (tchar = templateContent.GetAt(srcIndex) != '%'&&srcIndex < srcLength){
						srcIndex++;
					}
					//如果找到
					if (srcIndex < srcLength){
						//确定下一个是否'>'
						if (templateContent.GetAt(srcIndex + 1) == '>'){
							endPos = ++srcIndex;
							break;
						}
						//不为'>'，继续寻找'%'
					}
				}
				if (srcIndex == srcLength){//没找到标识符，则将剩余部分完全复制
					srcIndex = startPos;
					while (srcIndex < srcLength){
						tchar = templateContent.GetAt(srcIndex++);
						buffer[bufIndex++] = tchar;
					}
					break;
				}
				else{//找到标识符

				}

				//确认该段内容中的key
				//寻找dataMap中对应key的value值
				//将该值填入buffer当前位置
				//从"%>"位置继续复制
			}
			else{//否则复制
				buffer[bufIndex++] = tchar;
			}
		}
		else{//否则复制
			buffer[bufIndex++] = tchar;
		}
		srcIndex++;
	}



	//找到起始位置
	startPos = templateContent.Find(keyStart);
	endPos = templateContent.Find(keyEnd) + 1;
	//keystr = templateContent.Mid(startPos, endPos);

	//截取"<%= XXXXXXX%>"
	keystr = templateContent.Mid(startPos, endPos - startPos + 1);
	//tempstr = templateContent.Left(endPos);
	//keystr = tempstr.Right(endPos-startPos);

	//从CMap中找出对应的value
	key = keystr.AllocSysString();
	dataMap->Lookup(key, value);
	keystr.Format(L"<%%=%ws%%>", key);
	length = templateContent.Replace(L"<%=" + keystr + ">", value);*/


	//

	return CString(buffer);
}

/*CString GenerateReport(CString templateContent, MeterReport* meterReport){
	CString reportContent = templateContent;
	CString addressStr = meterReport->GetAddressStr();
	CString factoryIDStr = addressStr.Right(4);

	reportContent.Replace(L"<%= calibrationStation%>", L"北京市计量检测研究院");
	reportContent.Replace(L"<%= calibrationMode%>", L"");
	reportContent.Replace(L"<%= calibrationResult%>", meterReport->GetQualifiedStr());
	reportContent.Replace(L"<%= meterID%>", addressStr);
	reportContent.Replace(L"<%= factoryID%>", factoryIDStr);
	reportContent.Replace(L"<%= meterType%>", L"热水表");
	return reportContent;
}*/
