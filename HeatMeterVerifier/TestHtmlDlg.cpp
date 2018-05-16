// TestHtmlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "TestHtmlDlg.h"
#include "afxdialogex.h"

#include <stdio.h>


// CTestHtmlDlg 对话框

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
	m_html.Navigate(L"about:blank");  //这句话一定要写

	//读取HTML文件，存入变量
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
	//设置显示内容
	LPWSTR str = (htmlStr.GetBuffer(htmlStr.GetLength()));
	char* pStr = new char[htmlStr.GetLength()];
	sprintf(pStr, "%ws", htmlStr);
	int nLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pStr, -1, NULL, 0);
	//返回需要的unicode长度     
	WCHAR * wszANSI = new WCHAR[nLen + 1];
	memset(wszANSI, 0, nLen * 2 + 2);
	nLen = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, wszANSI, nLen);    //把utf8转成unicode    
	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);        //得到要的ansi长度     
	char *szANSI = new char[nLen + 1];
	memset(szANSI, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL, NULL);          //把unicode转成ansi     

	/*char *pStr = (htmlStr.GetBuffer(htmlStr.GetLength())); //取得str对象的原始字符串
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //取得所需缓存的多少
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize*sizeof(wchar_t));//转码
	MessageBoxW(NULL, pBuffer, L"Text", MB_OK); //显示
	free(pBuffer); //释放缓存
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


// CTestHtmlDlg 消息处理程序


BOOL CHtmlReportDlg::OnInitDialog()
{
	CReportDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//InitTestHtml2();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
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
	m_html.Navigate(L"about:blank");  //这句话一定要写
	
	report->SetRefMeter(refData);
	CString* htmlStr = report->GetContent();

	//设置显示内容
	LPWSTR str = (htmlStr->GetBuffer(htmlStr->GetLength())); 

	m_html.SetHTML(str);
	delete htmlStr;

}

void CHtmlReportDlg::InitUIs(){

}