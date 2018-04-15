#include "stdafx.h"
#include"MyPrintWizard.h"

#define MAX_PAHT 256

//TCHAR m_tchPrinterName[MAX_PAHT];///< 默认打印机的名称

TCHAR m_tchPrinterMessage[MAX_PAHT];///< 打印结果的返回信息
static int s_nPrintImageCount = 0;///< 打印图片的数量


MyPrintWizard::MyPrintWizard(){

}
MyPrintWizard::~MyPrintWizard(){

}



/// 获取默认打印机的名称

/*BOOL GetDefaultPrinterInfo()
{
	BOOL bResult = FALSE;

	DWORD cbNeeded = 0, cReturned = 0;

	if (!GetDefaultPRN(NULL, &cbNeeded))
	{
		return bResult;
	}
	TCHAR pPrinterDefault[MAX_PATH] = { 0 };

	if (!GetDefaultPRN(pPrinterDefault, &cbNeeded))
	{
		//::MessageBox(NULL,_TEXT("No default printer found!"),_TEXT("Error"),MB_OK|MB_ICONQUESTION);
		return bResult;
	}

	/// 默认打印机的名称

	_tcscpy(m_tchPrinterName, pPrinterDefault);
	return bResult;
}*/



/// 获取打印机的句柄

/// lpszPrinterName 表示打印机名称 

HANDLE MyPrintWizard::GetPrinterHandle(LPTSTR lpszPrinterName)
{
	HANDLE   hPrint;
	/// 获取打印机的Handle
	BOOL bOk = ::OpenPrinter(lpszPrinterName, &hPrint, NULL);
	if (!bOk)
	{
		//LPVOID   lpMsgBuf;  
		//FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER   |   FORMAT_MESSAGE_FROM_SYSTEM   |    
		//FORMAT_MESSAGE_IGNORE_INSERTS,NULL,::GetLastError(),  
		//MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),(LPTSTR)   &lpMsgBuf,0,NULL);  
		//::MessageBox(   NULL,   (LPTSTR)(LPCTSTR)lpMsgBuf,   _TEXT("Error"),   MB_OK   |  MB_ICONINFORMATION   );  
		//LocalFree( lpMsgBuf );
		_tcscpy(m_tchPrinterMessage, _TEXT("打开打印机失败"));
		return NULL;
	}
	return  hPrint;
}





/// 打印位图的关键实现函数
/// nPaperSizeIndex表示打印的纸张类型，如A4为DMPAPER_A4，

/// nPrintModel表示打印方式，如横向打印为DMORIENT_LANDSCAPE

/// lpszPrinterName 表示打印机名称
BOOL MyPrintWizard::PrintBmp(short nPaperSizeIndex, int nPrintModel, 
	LPTSTR  lpszPrinterName,
	HANDLE m_hBitmap, ///< 打印位图的Handle
	HWND m_hWnd
	)
{
	BOOL bResult = FALSE;
	HBITMAP hbitmap;
	hbitmap = (HBITMAP)m_hBitmap;
	if (hbitmap == NULL)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("打印图片不存在！"));
		return bResult;
	}
	//CBitmap bitmap;
	Bitmap bitmap(hbitmap, NULL);
	BITMAP bm;
	memset(&bm, 0, sizeof(BITMAP));
	//CDC memDC;
	HDC memDC = ::CreateCompatibleDC(NULL);
	//CBitmap * pOldBitmap = NULL;

	//CDC prtDC;
	HDC prtDC = NULL;
	DOCINFO di;

	HDC hDc = ::CreateCompatibleDC(NULL);
	//memDC.CreateCompatibleDC(NULL);
	//CSize paper_size; //printer paper size in mm
	Size paper_size;
	int xLogPPI = 0;
	int yLogPPI = 0;

	//pop up printer dialog

	int paper_width;
	int paper_height;
	int nStartX, nStartY, nPicWidth, nPicHeight;

	nStartX = 0;
	nStartY = 0;
	nPicWidth = 0;
	nPicHeight = 0;



	HANDLE   pn;
	DEVMODE  *print;

	/// 获取打印机的Handle
	pn = this->GetPrinterHandle(lpszPrinterName);
	if (NULL == pn)
	{
		return bResult;
	}

	//m_hPrintHandle = pn;

	/// 关于打印机配置最重要的一个结构就是DEVMODE结构（结构的具体请MSDN），

	/// 该结构中几乎包含了打印机的所有配置信息。
	/// 返回打印机的配置（DEVMODE结构）的大小  
	int   nSize = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, NULL, 0);
	print = (DEVMODE *)malloc(nSize + 1);
	if (NULL == print)
	{
		return bResult;
	}
	//ZeroMemory(print,nSize + 1);
	memset(print, 0, nSize + 1);

	/// 获取打印机配置信息
	LONG   lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, print, NULL, DM_OUT_BUFFER);
	print->dmFields = DM_PAPERSIZE | DM_ORIENTATION;
	// print->dmFields=DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;

	/// 将打印纸设置为A4
	print->dmPaperSize = nPaperSizeIndex;

	// print->dmPaperLength=m_PrintSize.x ; 
	// print->dmPaperWidth= m_PrintSize.y;

	/// 将打印机设置为横向打印
	print->dmOrientation = nPrintModel;

	////如果使用自定义纸张大小  
	//print->dwFields=DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;  
	//print->dmPaperLength=用户输入纸张的长度  
	//print->dmPaperWidth=用户输入纸张的宽度

	/// 设置打印机配置信息
	lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, print, DM_IN_BUFFER | DM_OUT_BUFFER);

	/// 获取打印机HDC
	HDC hDC = ::CreateDC(NULL, lpszPrinterName, NULL, print);//GetPrinterDC(lpszPrinterName, NULL);

	if (NULL == hDC)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("创建打印设备失败！"));
		return bResult;
	}

	//prtDC.Attach(hDC);
	prtDC = hDC;
	//paper_size.cx = GetDeviceCaps(hDC,HORZSIZE);
	//paper_size.cy = GetDeviceCaps(hDC,VERTSIZE);
	//xLogPPI = prtDC.GetDeviceCaps(LOGPIXELSX);
	//yLogPPI = prtDC.GetDeviceCaps(LOGPIXELSY);
	paper_size.Width = ::GetDeviceCaps(hDC, HORZSIZE);
	paper_size.Height = ::GetDeviceCaps(hDC, VERTSIZE);
	xLogPPI = ::GetDeviceCaps(prtDC, LOGPIXELSX);
	yLogPPI = ::GetDeviceCaps(prtDC, LOGPIXELSY);


	/// 打印页的象素宽--width of a printed page in pixels
	paper_width = (int)((double)paper_size.Width * (double)xLogPPI / 25.4);
	paper_height = (int)((double)paper_size.Height * (double)yLogPPI / 25.4);

	int page_info_left = (int)((double)paper_width * 0.9);
	int page_info_right = paper_width;
	int page_info_top = (int)((double)paper_height * 0.99);
	int page_info_bottom = paper_height;
	//CRect page_info_rect = CRect(page_info_left, page_info_top,
	// page_info_right,page_info_bottom );
	Rect page_info_rect = Rect(page_info_left, page_info_top,
		page_info_right, page_info_bottom);

	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	++s_nPrintImageCount;
	TCHAR strDocName[MAX_PATH] = { 0 };
	wsprintf(strDocName, _TEXT("%s%d"), _TEXT("PrintImage"), s_nPrintImageCount);
	/// 打印文档名称
	di.lpszDocName = strDocName;//_TEXT("PrintImage");//szAppName;
	//di.lpszOutput = szPortName;
	//prtDC.m_bPrinting = TRUE;

	//bitmap.Attach(hbitmap);

	//bitmap.GetBitmap(&bm);
	::GetObject(hbitmap, sizeof(BITMAP), &bm);

	/// 计算打印图片的大小宽高：nStartX nStartY nPicWidth nPicHeight
	if (bm.bmWidth > bm.bmHeight)
	{
		nPicWidth = paper_width;
		if (paper_width > bm.bmWidth)
		{
			nPicHeight = bm.bmHeight * (paper_width / bm.bmWidth);
		}
		else
		{
			nPicHeight = bm.bmHeight * (bm.bmWidth / paper_width);
		}
	}
	else
	{
		nPicHeight = paper_height;
		if (paper_height > bm.bmHeight)
		{
			nPicWidth = bm.bmWidth * (paper_height / bm.bmHeight);
		}
		else
		{
			nPicWidth = bm.bmWidth * (bm.bmHeight / paper_height);
		}
	}
	nStartX = (paper_width - nPicWidth) / 2;
	nStartY = (paper_height - nPicHeight) / 2;

	nStartX = nStartX < 0 ? 0 : nStartX;
	nStartY = nStartY < 0 ? 0 : nStartY;

	/// 准备打印
	//if(prtDC.StartDoc(&di) == -1)
	if (::StartDoc(prtDC, &di) == -1)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("启动打印失败！"));
		//::MessageBox(NULL,_TEXT("Printing error occured. Unable to find printer."),_TEXT("Error"),MB_OK|MB_ICONQUESTION );
		//prtDC.Detach();
		//prtDC.DeleteDC();
		::DeleteDC(prtDC);
		return bResult;
	}

	//prtDC.SetMapMode(MM_TEXT);
	::SetMapMode(prtDC, MM_TEXT);

	/// 开始打印页
	//prtDC.StartPage();
	::StartPage(prtDC);

	int nDrawNum = 0;
	//pOldBitmap = memDC.SelectObject(&bitmap);
	::SetMapMode(memDC, ::GetMapMode(memDC));
	::SetStretchBltMode(memDC, HALFTONE);
	//memDC.SetMapMode(memDC.GetMapMode());
	//memDC.SetStretchBltMode(HALFTONE);

	//if (pOldBitmap == NULL) // if bitmap is very big, better check this !
	//{
	// memDC.DeleteDC();
	// // AfxMessageBox("Not enough resource for the bitmap. Either reduce the bitmap dimension or \n switch to lower screen setting (e.g. 256-color mode), and try again. " ;
	// return bResult;
	//}


	HPALETTE hPal = NULL;

	//BITMAP                     bm;

	BITMAPINFOHEADER           bi;

	LPBITMAPINFOHEADER         lpbi;

	DWORD                      dwLen;

	HANDLE                     hDIB;

	HANDLE                     handle;

	HDC                        hDC1;

	if (GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)

	{

		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);

		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;

		pLP->palNumEntries = GetSystemPaletteEntries(hDC, 0, 255, pLP->palPalEntry);

		hPal = CreatePalette(pLP);

		delete[] pLP;

	}

	if (NULL == hPal)
	{
		hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	}

	::GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);

	bi.biSize = sizeof(BITMAPINFOHEADER);

	bi.biWidth = bm.bmWidth;

	bi.biHeight = bm.bmHeight;

	bi.biPlanes = 1;

	bi.biBitCount = bm.bmPlanes * bm.bmBitsPixel;

	bi.biCompression = BI_RGB;

	bi.biSizeImage = 0;

	bi.biXPelsPerMeter = 0;

	bi.biYPelsPerMeter = 0;

	bi.biClrUsed = 0;

	bi.biClrImportant = 0;

	int nColors = (1 << bi.biBitCount);

	if (nColors > 256)
	{
		nColors = 0;
	}

	dwLen = bi.biSize + nColors * sizeof(RGBQUAD);

	hDC1 = ::GetDC(NULL);

	hPal = SelectPalette(hDC1, hPal, FALSE);

	RealizePalette(hDC1);

	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);

	if (!hDIB)
	{
		//memDC.DeleteDC();
		::DeleteDC(memDC);

		SelectPalette(hDC1, hPal, FALSE);

		::ReleaseDC(NULL, hDC1);

		DeleteObject(hPal);

		return bResult;

	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	::GetDIBits(hDC1, hbitmap, 0L, (DWORD)bi.biHeight, \
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)* bi.biHeight;
	}

	dwLen += bi.biSizeImage;

	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
	{
		hDIB = handle;
	}
	else
	{
		//memDC.DeleteDC();
		::DeleteDC(memDC);

		GlobalFree(hDIB);

		SelectPalette(hDC1, hPal, FALSE);

		::ReleaseDC(NULL, hDC1);

		DeleteObject(hPal);

		return bResult;

	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	BOOL bGotBits = GetDIBits(hDC1, hbitmap, 0L, (DWORD)bi.biHeight, (LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD)), \

		(LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	if (!bGotBits)

	{
		//memDC.DeleteDC();
		::DeleteDC(memDC);

		GlobalFree(hDIB);

		SelectPalette(hDC1, hPal, FALSE);

		::ReleaseDC(NULL, hDC1);

		DeleteObject(hPal);

		return bResult;

	}

	// 打印位图
	int iRet = StretchDIBits(hDC, nStartX, nStartY, nPicWidth, nPicHeight, 0, 0, lpbi->biWidth, lpbi->biHeight, \
		(LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD)), (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);

	SelectPalette(hDC1, hPal, FALSE);

	::ReleaseDC(NULL, hDC1);

	DeleteObject(hDIB);

	DeleteObject(hPal);


	//prtDC.StretchBlt(nStartX, nStartY, nPicWidth, nPicHeight, &memDC,
	// 0, 0, bm.bmWidth , bm.bmHeight, SRCCOPY);

	//memDC.SelectObject(pOldBitmap);
	//bitmap.Detach();
	//prtDC.EndPage();
	::EndPage(prtDC);

	//memDC.DeleteDC();
	::DeleteDC(memDC);

	//prtDC.EndDoc();
	//prtDC.Detach();
	//prtDC.DeleteDC();

	::EndDoc(prtDC);
	::DeleteDC(prtDC);

	//BOOL bCloseOk = ::ClosePrinter(pn);
	//if(!bCloseOk)  
	//{  
	// free(print);  
	// return bResult;  
	//}  
	free(print);

	if (iRet == GDI_ERROR)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("打印失败！"));
		return bResult;
	}

	bResult = TRUE;
	return bResult;
}