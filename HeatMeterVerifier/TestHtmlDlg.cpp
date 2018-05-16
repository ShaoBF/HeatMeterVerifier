// TestHtmlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "TestHtmlDlg.h"
#include "afxdialogex.h"

#include <stdio.h>


// CTestHtmlDlg �Ի���

IMPLEMENT_DYNAMIC(CHtmlReportDlg, CReportDlg)

CHtmlReportDlg::CHtmlReportDlg(CWnd* pParent /*=NULL*/)
: CReportDlg(CHtmlReportDlg::IDD, pParent)
{

}

CHtmlReportDlg::~CHtmlReportDlg()
{
}

void CHtmlReportDlg::InitTestHtml2(){
	m_html.SetHideContextMenu(TRUE);
	m_html.CreateFromStatic(IDC_STATIC_FOR_HTML, this);
	m_html.Navigate(L"about:blank");  //��仰һ��Ҫд

	//��ȡHTML�ļ����������
	LPCTSTR pszFileName = L"D:/ShaoBF/htmltest/ReportTemp1.html";
	CStdioFile myFile;
	CFileException fileException;
	CString htmlStr;

	if (myFile.Open(pszFileName, CFile::typeText | CFile::modeReadWrite, &fileException)){
		myFile.SeekToBegin();


		CString str1;
		while (myFile.ReadString(str1)){

			htmlStr = htmlStr + str1;
		}
	} else {
		TRACE("Can't open file %s,error=%u/n", pszFileName, fileException.m_cause);
	}

	myFile.Close();
	//������ʾ����
	LPWSTR str = (htmlStr.GetBuffer(htmlStr.GetLength()));
	char* pStr = new char[htmlStr.GetLength()];
	sprintf(pStr, "%ws", htmlStr);
	int nLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pStr, -1, NULL, 0);
	//������Ҫ��unicode����     
	WCHAR * wszANSI = new WCHAR[nLen + 1];
	memset(wszANSI, 0, nLen * 2 + 2);
	nLen = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, wszANSI, nLen);    //��utf8ת��unicode    
	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);        //�õ�Ҫ��ansi����     
	char *szANSI = new char[nLen + 1];
	memset(szANSI, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL, NULL);          //��unicodeת��ansi     

	/*char *pStr = (htmlStr.GetBuffer(htmlStr.GetLength())); //ȡ��str�����ԭʼ�ַ���
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //ȡ�����軺��Ķ���
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//���뻺��ռ�
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize*sizeof(wchar_t));//ת��
	MessageBoxW(NULL, pBuffer, L"Text", MB_OK); //��ʾ
	free(pBuffer); //�ͷŻ���
	*/

	m_html.SetHTML(wszANSI);
	//m_html.SetHTML(htmlStr);

	delete wszANSI;
	delete szANSI;

}

void CHtmlReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_FOR_HTML, staticForHtml);
}


BEGIN_MESSAGE_MAP(CHtmlReportDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestHtmlDlg ��Ϣ�������


BOOL CHtmlReportDlg::OnInitDialog()
{
	CReportDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//InitTestHtml2();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

/*void CHtmlReportDlg::SetMeterReport(MeterReport* report){
	this->report = report;
}
void CHtmlReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}*/
void CHtmlReportDlg::DisplayReport(CalibrationReport* report){
	m_html.SetHideContextMenu(TRUE);
	m_html.CreateFromStatic(IDC_STATIC_FOR_HTML, this);
	m_html.Navigate(L"about:blank");  //��仰һ��Ҫд
	
	report->SetRefMeter(refData);
	CString* htmlStr = report->GetContent();

	//������ʾ����
	LPWSTR str = (htmlStr->GetBuffer(htmlStr->GetLength())); 

	m_html.SetHTML(str);
	delete htmlStr;

}

void CHtmlReportDlg::InitUIs(){

}