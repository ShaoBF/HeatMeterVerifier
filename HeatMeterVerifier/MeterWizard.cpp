#include "stdafx.h"
#include "MeterWizard.h"
#include "MyVector.cpp"
#include "DBConfig.h"
#include "ComConfig.h"

#define INI_PARAGRAPH_TEMPLATE L"ReportTemplate"
#define INI_KEY_TEMPLATE L"template"


CMeterWizard::CMeterWizard()
{
	//meterInfoList = new MeterInfo*[128];
	templateConfig = new TemplateConfig(GetConfigFilePath());
}


CMeterWizard::~CMeterWizard()
{
	//施放MeterInfo
	/*vector<PMeterInfo>::iterator it;
	for (it = meterInfoList.begin(); it != meterInfoList.end(); it++)
		delete *it;
	meterInfoList.clear();*/
	for (int i = 0; i < meterInfoList.GetSize(); i++){
		delete meterInfoList[i];
	}
	meterInfoList.Clear();
	if (templateConfig){
		delete templateConfig;
	}
}

void CMeterWizard::ChooseMeters(){
	CMeterChooserDlg dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}
}

void CMeterWizard::ReadMeters(){
	CMeterReadDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

}

void CMeterWizard::GenerateReports(){
	CMetersReportDlg dlg;
	dlg.SetMeterInfoList(GetMeterInfoList());

	INT_PTR nResponse = dlg.DoModal();


	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}


}

void CMeterWizard::AddMeterAddressCom(MeterInfo *meterInfo){
	meterInfoList.Add(meterInfo);
	//meterInfoList[meterCount++] = meterInfo;
}

void CMeterWizard::SetSelectMeterIndex(LPINT rgIndex, int count){
	selectedMeterIndex = rgIndex;
	selectedMeterCount = count;
	/*vector<MeterInfo*>::iterator it;
	for (it = meterInfoList.begin(); it != meterInfoList.end(); it++){
		(*it)->selected = false;
	}
	for (int i = 0; i < count; i++){
		meterInfoList[rgIndex[i]]->selected = true;
	}*/
	for (int i = 0; i < meterInfoList.GetSize(); i++){
		meterInfoList[i]->selected = false;
	}
	for (int i = 0; i < count; i++){
		meterInfoList[rgIndex[i]]->selected = true;
	}
}

void CMeterWizard::CloseAllCom(){
	//	vector<MeterInfo*>::iterator it;
	//	for (it = meterInfoList.begin(); it != meterInfoList.end(); it++){
	for (int i = 0; i<meterInfoList.GetSize(); i++){
		if (meterInfoList[i]->active){
			meterInfoList[i]->serial.CloseSerialPort();
		}
	}
}

MeterInfo* CMeterWizard::GetMeterInfo(int index){
	return meterInfoList[index];
}

void CMeterWizard::setRefMeter(int index){
	refMeterIndex = index;
}

MeterInfo* CMeterWizard::GetRefMeterInfo(){
	return GetMeterInfo(refMeterIndex);
}

bool CMeterWizard::IsRefMeter(UCHAR* address){
	UCHAR* refAddress = meterInfoList[refMeterIndex]->address;
	for (int i = 0; i < CJ188_ADDRESS_LENGTH; i++){
		if (address[i] != refAddress[i]){
			return false;
		}
	}
	return true;
}

MeterInfo* CMeterWizard::GetMeterInfo(UCHAR* address){
	int i;
	int j;
	for (j = 0; j < meterInfoList.GetSize(); j++){
		UCHAR* refAddress = meterInfoList[j]->address;
		for (i = 0; i < CJ188_ADDRESS_LENGTH; i++){
			if (address[i] != refAddress[i]){
				break;
			}
		}
		if (i == CJ188_ADDRESS_LENGTH){
			return meterInfoList[j];
		}
	}
	return nullptr;
}

MyVector<MeterReport*>* CMeterWizard::GetMeterReports(){
	MyVector<MeterReport*>* reports = new MyVector<MeterReport*>();
	MeterDataInfo* refMeter = (MeterDataInfo*)meterInfoList[refMeterIndex];
	//refMeter->CalculateReport(refMeter->GetReport());
	for (int i = 0; i < meterInfoList.GetSize(); i++){
		MeterDataInfo* info = (MeterDataInfo*)meterInfoList[i];
		MeterReport* report = info->CalculateReport(refMeter->GetReport());
		reports->Add(report);
	}
	return reports;
}
/*
MyVector<MeterInfo*>* CMeterWizard::GetMeterInfoList(){
	return &meterInfoList;
}
*/
extern CDBConfig dbConfig;
LPCTSTR CMeterWizard::GetConnectStr(){
	//FIXME: would retrive form config file.
	return dbConfig.ToString();
}
double CMeterWizard::GetVerifyRate(){
	return config.GetVerifyRate();
}

void CMeterWizard::setLowByteFirst(bool lowFirst){
	lowByteFirst = lowFirst;
}
void CMeterWizard::setPaddingDigit(UCHAR digit){
	paddingDigit = digit;
}

bool CMeterWizard::isLowByteFirst(){
	return lowByteFirst;
}
UCHAR CMeterWizard::getPaddingDigit(){
	return paddingDigit;
}

CString CMeterWizard::GetConfigFilePath(){
	//获取程序安装路径
	CString installPath = GetInstallPath();
	//添加配置文件相对路径
	CString configFilePath = L"\\conf\\Config.ini";
	//返回完整路径
	//return L"D:\\ShaoBF\\IniTest\\Config.ini";
	return installPath + configFilePath;
}

CString GetWorkDir()
{
	//char pFileName[MAX_PATH];
	LPWSTR pFileName = new WCHAR[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	if (nPos < 0)
		return CString("");
	else
		return csFullPath;
}
CString CMeterWizard::GetInstallPath(){
	return GetWorkDir();
}

CString CMeterWizard::GetTemplatePath(){
	//获取程序安装路径
	CString installPath = GetInstallPath();
	//添加配置文件相对路径
	CString templatePath = L"\\template\\";
	//返回完整路径
	//return L"D:\\ShaoBF\\IniTest\\Config.ini";
	return installPath + templatePath;
}

CString CMeterWizard::GetTemplateFilePath(){
	//获取程序安装路径
	CString path = GetTemplatePath();
	//返回完整路径
	CString filePath;//= path + templateFile;
	filePath.Format(L"%ws%ws", path, templateConfig->fileName);
	return filePath;
}
extern ComConfig comConfig;
extern CDBConfig dbConfig;


BOOL CMeterWizard::LoadConfig(){
	try{
		//ComConfig config;
		comConfig.ReadConfig();
		dbConfig.ReadConfig();
		//ReadReportTemplateConfig(GetConfigFilePath());
		templateConfig->ReadConfig();
	}
	catch (int e){
		return FALSE;
	}
	return TRUE;
}
/*BOOL CMeterWizard::ReadReportTemplateConfig(CString configPath){
	int error;
	int strLength;

	strLength = ::GetPrivateProfileString(L"ReportTemplate", L"template", NULL, templateConfig.fileName.GetBuffer(128), 128, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	return TRUE;
}*/
