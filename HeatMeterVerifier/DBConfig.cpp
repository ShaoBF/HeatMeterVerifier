#include "stdafx.h"

#include "DBConfig.h"
#include "Converter.h"

#define LP_APPNAME L"DataBaseConfig"


CDBConfig::CDBConfig(CMainFrame* main, CString configPath) :CalibratorConfig(main, configPath){

}

CDBConfig::CDBConfig(CString configPath) :CalibratorConfig(configPath)
{

}


CDBConfig::~CDBConfig()
{
}
//CString type;
//CString dsn;
//CString uid;
//CString pwd;

BOOL CDBConfig::SaveConfig(CString configPath){
	BOOL success = true;

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"type", type, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"DSN", dsn, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"UID", uid, configPath);

	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"PWD", pwd, configPath);

	return success;
}
BOOL CDBConfig::ReadConfig(CString configPath){
	int error;
	int strLength;

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"type", NULL, type.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"DSN", NULL, dsn.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"UID", NULL, uid.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}

	strLength = ::GetPrivateProfileString(LP_APPNAME, L"PWD", NULL, pwd.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}
	return TRUE;
}
BOOL CDBConfig::SaveConfig(){
	return CalibratorConfig::SaveConfig();
}
BOOL CDBConfig::ReadConfig(){
	return CalibratorConfig::ReadConfig();
}


LPCTSTR CDBConfig::ToString(){
	//return _T("ODBC;DSN=HeatMeterDS32;UID=ShaoBF;PWD=shbofe");
	/*CString connectStr = type + L";DSN=" + dsn + L";UID=" + uid + L";PWD=" + pwd;
	connectStr.Format(L"%ws;DSN=%ws;UID=%ws;PWD=%ws", type, dsn, uid, pwd);
	connectStr = type;
	connectStr = connectStr + L";DSN=";
	connectStr = connectStr + dsn;
	connectStr = connectStr + L";UID=";
	connectStr = connectStr + dsn;*/
	char buffer[256];
	sprintf(buffer, "%ws;DSN=%ws;UID=%ws;PWD=%ws", type, dsn, uid, pwd);
	/*int num = MultiByteToWideChar(0, 0, buffer, -1, NULL, 0);
	MultiByteToWideChar(0, 0, buffer, -1, wide, num);*/
	LPCTSTR wide = Converter::pcharToWchar(buffer);
	return wide;
}

