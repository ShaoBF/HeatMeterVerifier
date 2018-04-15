#pragma once
#pragma comment(lib, "winspool.lib")
#include <winspool.h>
#include <gdiplus.h>
using namespace Gdiplus;




class MyPrintWizard
{
public:
	MyPrintWizard();
	virtual ~MyPrintWizard();

	HANDLE GetPrinterHandle(LPTSTR lpszPrinterName);
	BOOL PrintBmp(short nPaperSizeIndex, int nPrintModel,
		LPTSTR  lpszPrinterName,
		HANDLE m_hBitmap,
		HWND m_hWnd
		);


};

