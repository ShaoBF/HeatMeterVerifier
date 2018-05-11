#include "stdafx.h"
#include "ComConfig.h"
#include "Converter.h"

ComConfig::ComConfig(CMainFrame* main, CString configPath)
{
	//TODO����ini�ļ���ȡ������Ϣ
	try{
		ReadConfig(configPath);
	}
	catch(int e){//��δ��ȡ�ɹ���ֱ�ӱ����˳�����
		if (e == 0x02){
			AfxMessageBox(L"�����ļ�ComConfig.iniȱʧ���𻵣��޷�����������");
			main->CloseWindow();
		}
		if (e == -1){
			AfxMessageBox(L"�����ļ�ComConfig.ini�𻵣��޷�����������");
		}
		//д����־�ļ�
	}
}

ComConfig::ComConfig(CString configPath){
	//TODO����ini�ļ���ȡ������Ϣ
	try{
		ReadConfig(configPath);
	}
	catch (int e){//��δ��ȡ�ɹ���ֱ�ӱ����˳�����
		if (e == 0x02){
			AfxMessageBox(L"�����ļ�ComConfig.iniȱʧ���𻵣��޷�����������");
		}
		if (e == -1){
			AfxMessageBox(L"�����ļ�ComConfig.ini�𻵣��޷�����������");
		}
		//д����־�ļ�
	}
}
ComConfig::~ComConfig()
{
}

int ComConfig::GetBaudRateIndex(){
	int index = 0;
	int br = baudRate / BAUD_RATE_BASE;
	while (br != 1){
		index++;
		br /= 2;
	}
	return index;
}

int ComConfig::GetDataBitsIndex(){
	return 8 - dataBits;
}

int ComConfig::GetStopBitsIndex(){
	return stopBits - 1;
}

int ComConfig::GetParityIndex(){
	return parityIndex;
}

BOOL ComConfig::SaveConfig(CString configPath){
	BOOL success = true;

	CString parityIndexStr;
	parityIndexStr.Format(L"%d", parityIndex);
	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"parityIndex", parityIndexStr, configPath);

	CString dataBitsStr;
	dataBitsStr.Format(L"%d", dataBits);
	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"dataBits", dataBitsStr, configPath);

	CString stopBitsStr;
	stopBitsStr.Format(L"%d", stopBits);
	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"stopBits", stopBitsStr, configPath);

	CString baudRateStr;
	baudRateStr.Format(L"%d", baudRate);
	success = success&& ::WritePrivateProfileString(LP_APPNAME, L"baudRate", baudRateStr, configPath);

	return success;
}
BOOL ComConfig::ReadConfig(CString configPath){
	int error;
	int strLength;

	CString parityIndexStr;
	strLength = ::GetPrivateProfileString(LP_APPNAME, L"parityIndex", NULL, parityIndexStr.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}
	parityIndex = _ttoi(parityIndexStr);

	CString dataBitsStr;
	strLength = ::GetPrivateProfileString(LP_APPNAME, L"dataBits", NULL, dataBitsStr.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}
	dataBits = _ttoi(dataBitsStr);

	CString stopBitsStr;
	strLength = ::GetPrivateProfileString(LP_APPNAME, L"stopBits", NULL, stopBitsStr.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}
	stopBits = _ttoi(stopBitsStr);

	CString baudRateStr;
	strLength = ::GetPrivateProfileString(LP_APPNAME, L"baudRate", NULL, baudRateStr.GetBuffer(16), 16, configPath);
	error = ::GetLastError();
	if (error == 0x02){
		throw error;
	}
	if (strLength == 0){
		throw (-1);
	}
	int bBaudRate = _ttoi(baudRateStr);
	if (bBaudRate == 0){
		baudRate = 9600;
	}
	return TRUE;
}
