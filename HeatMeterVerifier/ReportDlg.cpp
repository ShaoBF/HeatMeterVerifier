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

	//��ʼ������UI�ؼ�
	InitUIs();

	//������Ҫ���ɱ���
	CalibrationReport* report=GenerateReport();

	//��ʾ��ⱨ���������б�
	DisplayReport(report);

	//��ϸ��Ϣ�б���ʽ��ʾ����

	return true;
}
CalibrationReport* CReportDlg::GenerateReport(){
	//��ȡģ��
	CString reportTemplate = GetReportTemplate(); 
	//��ȡ����
	//MeterReport* meterReport = GetMeterReport();
	//����������ģ���Ӧλ��
	CalibrationReport* report = new CalibrationReport(reportTemplate);
	report->SetCalibratorInfo(GetCalibratorInfo());
	report->SetMeterReport(meterReport);
	//CalibrationReport* reportContent = generator.GenerateReport(reportTemplate, dataMap);
	return report;
}

CString CReportDlg::GetReportTemplate(){
	//��ȡģ��·��
	CString templatePath = wizard.GetTemplateFilePath();
	//��ȡģ������

	//��ȡHTML�ļ����������
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
	//������ʾ����
	/*LPWSTR str = (htmlStr.GetBuffer(htmlStr.GetLength()));
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
