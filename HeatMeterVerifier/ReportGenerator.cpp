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

	//ȷ���ؼ��ִ�key
	//��ʼ��Ϊ"<%="��������Ϊ"%>"
	//��ȡ��ʼ�ͽ���λ���±�startPos��endPos
	CString keyStart = L"<%=";
	CString keyEnd = L"%>";

	srcIndex = 0;
	while (srcIndex < srcLength){
		//Ѱ��"<%"λ��
		startPos = templateContent.Find(keyStart, srcIndex);
		if (startPos != -1){
			//��srcIndex��startPos֮������ȫ��д��buffer
			int count = CopyBuffer(&(buffer[bufIndex]), templateContent, srcIndex, startPos);
			srcIndex = startPos;
			bufIndex += count;

			//Ѱ��"%>"λ��
			endPos = templateContent.Find(keyEnd, srcIndex) + 2;
			//��û��"%>"�������Ƶ�����
			if (endPos == -1){
				break;
			}
			else{//��������key��Ӧvalueд��buffer
				int keyStart = startPos + 3;
				int keyEnd = endPos - 2;
				//ȡ��"<%="��"%>"֮������
				keystr = templateContent.Mid(keyStart, keyEnd - keyStart);
				//ȥ�������ݿո�
				keystr.Trim();
				//��dataMap���ҵ�key��Ӧvalue
				if (dataMap->Lookup(keystr.GetBuffer(), value)){
					//���ҵ�����value����buffer
					int count = CopyBuffer(&(buffer[bufIndex]), value, 0, value.GetLength());
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


	//��ͷ��ʼ����ģ�����ַ����ƽ���buffer
	/*int srcIndex = 0;
	int bufIndex = 0;
	TCHAR tchar;
	int srcLength = templateContent.GetLength();
	while ((tchar = templateContent.GetAt(srcIndex)) != '\0'){
		//����'<'ʱ���鿴��һ���ַ��Ƿ�Ϊ'%'
		if (tchar == '<'){
			//��Ϊ"<%"��ϣ��±���Ϊ���滻������ʼλ��
			if (templateContent.GetAt(srcIndex + 1) == '%'){
				startPos = srcIndex;
				//Ѱ��"%>"��Ϊ����λ��
				//�ҵ���һ��'%'λ��
				while (srcIndex < srcLength){
					while (tchar = templateContent.GetAt(srcIndex) != '%'&&srcIndex < srcLength){
						srcIndex++;
					}
					//����ҵ�
					if (srcIndex < srcLength){
						//ȷ����һ���Ƿ�'>'
						if (templateContent.GetAt(srcIndex + 1) == '>'){
							endPos = ++srcIndex;
							break;
						}
						//��Ϊ'>'������Ѱ��'%'
					}
				}
				if (srcIndex == srcLength){//û�ҵ���ʶ������ʣ�ಿ����ȫ����
					srcIndex = startPos;
					while (srcIndex < srcLength){
						tchar = templateContent.GetAt(srcIndex++);
						buffer[bufIndex++] = tchar;
					}
					break;
				}
				else{//�ҵ���ʶ��

				}

				//ȷ�ϸö������е�key
				//Ѱ��dataMap�ж�Ӧkey��valueֵ
				//����ֵ����buffer��ǰλ��
				//��"%>"λ�ü�������
			}
			else{//������
				buffer[bufIndex++] = tchar;
			}
		}
		else{//������
			buffer[bufIndex++] = tchar;
		}
		srcIndex++;
	}



	//�ҵ���ʼλ��
	startPos = templateContent.Find(keyStart);
	endPos = templateContent.Find(keyEnd) + 1;
	//keystr = templateContent.Mid(startPos, endPos);

	//��ȡ"<%= XXXXXXX%>"
	keystr = templateContent.Mid(startPos, endPos - startPos + 1);
	//tempstr = templateContent.Left(endPos);
	//keystr = tempstr.Right(endPos-startPos);

	//��CMap���ҳ���Ӧ��value
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

	reportContent.Replace(L"<%= calibrationStation%>", L"�����м�������о�Ժ");
	reportContent.Replace(L"<%= calibrationMode%>", L"");
	reportContent.Replace(L"<%= calibrationResult%>", meterReport->GetQualifiedStr());
	reportContent.Replace(L"<%= meterID%>", addressStr);
	reportContent.Replace(L"<%= factoryID%>", factoryIDStr);
	reportContent.Replace(L"<%= meterType%>", L"��ˮ��");
	return reportContent;
}*/
