#include "stdafx.h"
#include "CalibratorConfig.h"

CalibratorConfig::CalibratorConfig(CMainFrame* main, CString configPath)
{
	this->configPath = configPath;
	/*try{
		ReadConfig(configPath);
	}
	catch (int e){//若未读取成功，直接报错退出程序
		if (e == 0x02){
			AfxMessageBox(L"配置文件ComConfig.ini缺失或损坏，无法正常工作。");
			main->CloseWindow();
		}
		if (e == -1){
			AfxMessageBox(L"配置文件ComConfig.ini损坏，无法正常工作。");
		}
		//写入日志文件
	}*/
}

CalibratorConfig::CalibratorConfig(CString configPath)
{
	this->configPath = configPath;
	/*try{
		ReadConfig(configPath);
	}
	catch (int e){//若未读取成功，直接报错退出程序
		if (e == 0x02){
			AfxMessageBox(L"配置文件ComConfig.ini缺失或损坏，无法正常工作。");
		}
		if (e == -1){
			AfxMessageBox(L"配置文件ComConfig.ini损坏，无法正常工作。");
		}
		//写入日志文件
	}*/

}


CalibratorConfig::~CalibratorConfig()
{
}

BOOL CalibratorConfig::SaveConfig(){
	return SaveConfig(configPath);
}
BOOL CalibratorConfig::ReadConfig(){
	return ReadConfig(configPath);
}

