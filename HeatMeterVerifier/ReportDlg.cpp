#pragma once
#include "stdafx.h"
#include "ReportDlg.h"
#include <afxtempl.h>
#include "MeterWizard.h"
#include "Converter.h"

extern CMeterWizard wizard;
CReportDlg::CReportDlg(CWnd* pParent)
{
}
CReportDlg::CReportDlg(UINT nIDTemplate, CWnd *pParent)
	: CDialogEx(nIDTemplate, pParent)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::SetMeterReport(MeterReport* report){
	this->meterReport = report;
}
void CReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}
BOOL CReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化所有UI控件
	InitUIs();

	//根据需要生成报告
	CalibrationReport* report=GenerateReport();

	//显示检测报告数据项列表
	DisplayReport(report);

	//详细信息列表形式显示读数

	return true;
}
CalibrationReport* CReportDlg::GenerateReport(){
	//获取模板
	CString reportTemplate = GetReportTemplate(); 
	//获取数据
	//MeterReport* meterReport = GetMeterReport();
	//将数据填入模板对应位置
	CalibrationReport* report = new CalibrationReport(reportTemplate);
	report->SetCalibratorInfo(GetCalibratorInfo());
	report->SetMeterReport(meterReport);
	//CalibrationReport* reportContent = generator.GenerateReport(reportTemplate, dataMap);
	return report;
}

CString CReportDlg::GetReportTemplate(){
	//获取模板路径
	CString templatePath = wizard.GetTemplateFilePath();
	//获取模板内容

	//读取HTML文件，存入变量
	LPCTSTR pszFileName = templatePath.GetBuffer(templatePath.GetLength());//L"D:/ShaoBF/htmltest/ReportTemp1.html";
	CStdioFile myFile;
	CFileException fileException;
	CString htmlStr;

	if (myFile.Open(pszFileName, CFile::typeText | CFile::modeReadWrite, &fileException)){
		myFile.SeekToBegin();


		CString str1;
		while (myFile.ReadString(str1)){

			htmlStr = htmlStr + str1;
		}
	}
	else {
		TRACE("Can't open file %s,error=%u/n", pszFileName, fileException.m_cause);
	}

	myFile.Close();
	//设置显示内容
	/*LPWSTR str = (htmlStr.GetBuffer(htmlStr.GetLength()));
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

	delete wszANSI;
	delete szANSI;*/
	WCHAR* templateR=new WCHAR[16000];
	if (!wizard.templateConfig->encoding.CompareNoCase(L"utf-8")){
		templateR = Converter::Utf8ToAnsi(htmlStr);
	}
	htmlStr = CString(templateR);
	delete templateR;

	return htmlStr;
}

CCalibratorInfo* CReportDlg::GetCalibratorInfo(){
	return nullptr;
}
