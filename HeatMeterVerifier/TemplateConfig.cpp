#include "stdafx.h"
#include "TemplateConfig.h"

#define LP_APPNAME L"ReportTemplate"

TemplateConfig::TemplateConfig(CMainFrame* main, CString configPath) :CalibratorConfig(main, configPath){

}

TemplateConfig::TemplateConfig(CString configPath) : CalibratorConfig(configPath)
{

}


TemplateConfig::~TemplateConfig()
{
}

BOOL TemplateConfig::SaveConfig(CString configPath){
	BOOL success = true;

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"template", fileName, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"encoding", encoding, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"format", format, configPath);

	return success;
}

BOOL TemplateConfig::ReadConfig(CString configPath){
	int error;
	int strLength;

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"template", NULL, fileName.GetBuffer(128), 128, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"encoding", NULL, encoding.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"format", NULL, format.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	return TRUE;
}
BOOL TemplateConfig::SaveConfig(){
	return CalibratorConfig::SaveConfig();
}
BOOL TemplateConfig::ReadConfig(){
	return CalibratorConfig::ReadConfig();
}
