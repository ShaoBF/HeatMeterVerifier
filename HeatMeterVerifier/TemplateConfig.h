#pragma once

#include "CalibratorConfig.h"


class TemplateConfig:public CalibratorConfig
{
public:
	TemplateConfig(CMainFrame* main, CString configPath);
	TemplateConfig(CString configPath);
	virtual ~TemplateConfig();
	
	CString fileName;
	CString encoding;
	CString format;

	BOOL SaveConfig(CString configPath);
	BOOL ReadConfig(CString configPath);
	BOOL SaveConfig();
	BOOL ReadConfig();

};

