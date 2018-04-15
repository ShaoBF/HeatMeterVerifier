#include "stdafx.h"
#include"MyPrintWizard.h"

#define MAX_PAHT 256

//TCHAR m_tchPrinterName[MAX_PAHT];///< Ĭ�ϴ�ӡ��������

TCHAR m_tchPrinterMessage[MAX_PAHT];///< ��ӡ����ķ�����Ϣ
static int s_nPrintImageCount = 0;///< ��ӡͼƬ������


MyPrintWizard::MyPrintWizard(){

}
MyPrintWizard::~MyPrintWizard(){

}



/// ��ȡĬ�ϴ�ӡ��������

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

	/// Ĭ�ϴ�ӡ��������

	_tcscpy(m_tchPrinterName, pPrinterDefault);
	return bResult;
}*/



/// ��ȡ��ӡ���ľ��

/// lpszPrinterName ��ʾ��ӡ������ 

HANDLE MyPrintWizard::GetPrinterHandle(LPTSTR lpszPrinterName)
{
	HANDLE   hPrint;
	/// ��ȡ��ӡ����Handle
	BOOL bOk = ::OpenPrinter(lpszPrinterName, &hPrint, NULL);
	if (!bOk)
	{
		//LPVOID   lpMsgBuf;  
		//FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER   |   FORMAT_MESSAGE_FROM_SYSTEM   |    
		//FORMAT_MESSAGE_IGNORE_INSERTS,NULL,::GetLastError(),  
		//MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),(LPTSTR)   &lpMsgBuf,0,NULL);  
		//::MessageBox(   NULL,   (LPTSTR)(LPCTSTR)lpMsgBuf,   _TEXT("Error"),   MB_OK   |  MB_ICONINFORMATION   );  
		//LocalFree( lpMsgBuf );
		_tcscpy(m_tchPrinterMessage, _TEXT("�򿪴�ӡ��ʧ��"));
		return NULL;
	}
	return  hPrint;
}





/// ��ӡλͼ�Ĺؼ�ʵ�ֺ���
/// nPaperSizeIndex��ʾ��ӡ��ֽ�����ͣ���A4ΪDMPAPER_A4��

/// nPrintModel��ʾ��ӡ��ʽ��������ӡΪDMORIENT_LANDSCAPE

/// lpszPrinterName ��ʾ��ӡ������
BOOL MyPrintWizard::PrintBmp(short nPaperSizeIndex, int nPrintModel, 
	LPTSTR  lpszPrinterName,
	HANDLE m_hBitmap, ///< ��ӡλͼ��Handle
	HWND m_hWnd
	)
{
	BOOL bResult = FALSE;
	HBITMAP hbitmap;
	hbitmap = (HBITMAP)m_hBitmap;
	if (hbitmap == NULL)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("��ӡͼƬ�����ڣ�"));
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

	/// ��ȡ��ӡ����Handle
	pn = this->GetPrinterHandle(lpszPrinterName);
	if (NULL == pn)
	{
		return bResult;
	}

	//m_hPrintHandle = pn;

	/// ���ڴ�ӡ����������Ҫ��һ���ṹ����DEVMODE�ṹ���ṹ�ľ�����MSDN����

	/// �ýṹ�м��������˴�ӡ��������������Ϣ��
	/// ���ش�ӡ�������ã�DEVMODE�ṹ���Ĵ�С  
	int   nSize = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, NULL, 0);
	print = (DEVMODE *)malloc(nSize + 1);
	if (NULL == print)
	{
		return bResult;
	}
	//ZeroMemory(print,nSize + 1);
	memset(print, 0, nSize + 1);

	/// ��ȡ��ӡ��������Ϣ
	LONG   lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, print, NULL, DM_OUT_BUFFER);
	print->dmFields = DM_PAPERSIZE | DM_ORIENTATION;
	// print->dmFields=DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;

	/// ����ӡֽ����ΪA4
	print->dmPaperSize = nPaperSizeIndex;

	// print->dmPaperLength=m_PrintSize.x ; 
	// print->dmPaperWidth= m_PrintSize.y;

	/// ����ӡ������Ϊ�����ӡ
	print->dmOrientation = nPrintModel;

	////���ʹ���Զ���ֽ�Ŵ�С  
	//print->dwFields=DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;  
	//print->dmPaperLength=�û�����ֽ�ŵĳ���  
	//print->dmPaperWidth=�û�����ֽ�ŵĿ��

	/// ���ô�ӡ��������Ϣ
	lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, print, DM_IN_BUFFER | DM_OUT_BUFFER);

	/// ��ȡ��ӡ��HDC
	HDC hDC = ::CreateDC(NULL, lpszPrinterName, NULL, print);//GetPrinterDC(lpszPrinterName, NULL);

	if (NULL == hDC)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("������ӡ�豸ʧ�ܣ�"));
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


	/// ��ӡҳ�����ؿ�--width of a printed page in pixels
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
	/// ��ӡ�ĵ�����
	di.lpszDocName = strDocName;//_TEXT("PrintImage");//szAppName;
	//di.lpszOutput = szPortName;
	//prtDC.m_bPrinting = TRUE;

	//bitmap.Attach(hbitmap);

	//bitmap.GetBitmap(&bm);
	::GetObject(hbitmap, sizeof(BITMAP), &bm);

	/// �����ӡͼƬ�Ĵ�С��ߣ�nStartX nStartY nPicWidth nPicHeight
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

	/// ׼����ӡ
	//if(prtDC.StartDoc(&di) == -1)
	if (::StartDoc(prtDC, &di) == -1)
	{
		_tcscpy(m_tchPrinterMessage, _TEXT("������ӡʧ�ܣ�"));
		//::MessageBox(NULL,_TEXT("Printing error occured. Unable to find printer."),_TEXT("Error"),MB_OK|MB_ICONQUESTION );
		//prtDC.Detach();
		//prtDC.DeleteDC();
		::DeleteDC(prtDC);
		return bResult;
	}

	//prtDC.SetMapMode(MM_TEXT);
	::SetMapMode(prtDC, MM_TEXT);

	/// ��ʼ��ӡҳ
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

	// ��ӡλͼ
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
		_tcscpy(m_tchPrinterMessage, _TEXT("��ӡʧ�ܣ�"));
		return bResult;
	}

	bResult = TRUE;
	return bResult;
}