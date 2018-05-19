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

	//ȷ���ؼ��ִ�key
	//��ʼ��Ϊ"<%="��������Ϊ"%>"
	//��ȡ��ʼ�ͽ���λ���±�startPos��endPos
	CString keyStart = L"<%=";
	CString keyEnd = L"%>";


	srcIndex = 0;
	while (srcIndex < srcLength){
		//Ѱ��"<%"λ��
		startPos = templateContent->Find(keyStart, srcIndex);
		if (startPos != -1){
			//��srcIndex��startPos֮������ȫ��д��buffer
			int count = CopyBuffer(&(buffer[bufIndex]), templateContent, srcIndex, startPos);
			srcIndex = startPos;
			bufIndex += count;

			//Ѱ��"%>"λ��
			endPos = templateContent->Find(keyEnd, srcIndex) + 2;
			//��û��"%>"�������Ƶ�����
			if (endPos == -1){
				break;
			}
			else{//��������key��Ӧvalueд��buffer
				int keyStart = startPos + 3;
				int keyEnd = endPos - 2;
				//ȡ��"<%="��"%>"֮������
				keystr = templateContent->Mid(keyStart, keyEnd - keyStart);
				//ȥ�������ݿո�
				keystr.Trim();
				//��dataMap���ҵ�key��Ӧvalue
				//void* value = new CString();
				//void* vv = new CString();
				BOOL b;
				//CString keystr(L"calibrationStation");
				CString value;
				b = dataMap->Lookup(keystr.GetBuffer(), value);
				//CString* value = (CString*)vv;

				if (b){
					//���ҵ�����value����buffer

					int count = CopyBuffer(&(buffer[bufIndex]), &value, 0, value.GetLength());
					bufIndex += count;
				}
				else{
					//�������θ���
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
	// ����ʣ������
	int count = CopyBuffer(&(buffer[bufIndex]), templateContent, srcIndex, srcLength);
	bufIndex += count;
	buffer[bufIndex++] = 0;
	CString* content = new CString(buffer);
	delete buffer;
	//

	return content;
}