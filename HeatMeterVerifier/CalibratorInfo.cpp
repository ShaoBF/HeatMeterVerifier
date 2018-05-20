#include "stdafx.h"
#include "CalibratorInfo.h"

#define LP_APPNAME L"CalibrationStation"


CCalibratorInfo::CCalibratorInfo(CMainFrame* main, CString configPath) :CalibratorConfig(main, configPath){

}

CCalibratorInfo::CCalibratorInfo(CString configPath) : CalibratorConfig(configPath)
{

}



CCalibratorInfo::~CCalibratorInfo()
{
}
BOOL CCalibratorInfo::SaveConfig(CString configPath){
	BOOL success = true;

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"name", name, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"calibrateMode", calibrateMode, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"tester", tester, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"reviewer", reviewer, configPath);

	return success;
}
BOOL CCalibratorInfo::ReadConfig(CString configPath){
	int error;
	int strLength;

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"name", NULL, name.GetBuffer(200), 200, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"calibrateMode", NULL, calibrateMode.GetBuffer(64), 64, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"tester", NULL, tester.GetBuffer(32), 32, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"reviewer", NULL, reviewer.GetBuffer(32), 32, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}
	return TRUE;
}
BOOL CCalibratorInfo::SaveConfig(){
	return CalibratorConfig::SaveConfig();
}
BOOL CCalibratorInfo::ReadConfig(){
	return CalibratorConfig::ReadConfig();
}
