#include "stdafx.h"
#include "CalibratorConfig.h"

CalibratorConfig::CalibratorConfig(CMainFrame* main, CString configPath)
{
	this->configPath = configPath;
	/*try{
		ReadConfig(configPath);
	}
	catch (int e){//��δ��ȡ�ɹ���ֱ�ӱ����˳�����
		if (e == 0x02){
			AfxMessageBox(L"�����ļ�ComConfig.iniȱʧ���𻵣��޷�����������");
			main->CloseWindow();
		}
		if (e == -1){
			AfxMessageBox(L"�����ļ�ComConfig.ini�𻵣��޷�����������");
		}
		//д����־�ļ�
	}*/
}

CalibratorConfig::CalibratorConfig(CString configPath)
{
	this->configPath = configPath;
	/*try{
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

