// TestHtmlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "TestHtmlDlg.h"
#include "afxdialogex.h"
#include "MyPrintWizard.h"

#include <stdio.h>

#include <gdiplus.h>
#pragma comment(lib, "winspool.lib")
#include <winspool.h>

using namespace Gdiplus;


// CTestHtmlDlg 对话框

IMPLEMENT_DYNAMIC(CHtmlReportDlg, CReportDlg)

CHtmlReportDlg::CHtmlReportDlg(CWnd* pParent /*=NULL*/)
: CReportDlg(CHtmlReportDlg::IDD, pParent)
{

}

CHtmlReportDlg::~CHtmlReportDlg()
{
}

void CHtmlReportDlg::InitTestHtml2(){
	m_html.SetHideContextMenu(TRUE);
	m_html.CreateFromStatic(IDC_STATIC_FOR_HTML, this);
	m_html.Navigate(L"about:blank");  //这句话一定要写

	//读取HTML文件，存入变量
	LPCTSTR pszFileName = L"D:/ShaoBF/htmltest/ReportTemp1.html";
	CStdioFile myFile;
	CFileException fileException;
	CString htmlStr;

	if (myFile.Open(pszFileName, CFile::typeText | CFile::modeReadWrite, &fileException)){
		myFile.SeekToBegin();


		CString str1;
		while (myFile.ReadString(str1)){

			htmlStr = htmlStr + str1;
		}
	} else {
		TRACE("Can't open file %s,error=%u/n", pszFileName, fileException.m_cause);
	}

	myFile.Close();
	//设置显示内容
	LPWSTR str = (htmlStr.GetBuffer(htmlStr.GetLength()));
	char* pStr = new char[htmlStr.GetLength()];
	sprintf(pStr, "%ws", htmlStr);
	int nLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pStr, -1, NULL, 0);
	//返回需要的unicode长度     
	WCHAR * wszANSI = new WCHAR[nLen + 1];
	memset(wszANSI, 0, nLen * 2 + 2);
	nLen = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, wszANSI, nLen);    //把utf8转成unicode    
	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);        //得到要的ansi长度     
	char *szANSI = new char[nLen + 1];
	memset(szANSI, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL, NULL);          //把unicode转成ansi     

	/*char *pStr = (htmlStr.GetBuffer(htmlStr.GetLength())); //取得str对象的原始字符串
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //取得所需缓存的多少
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize*sizeof(wchar_t));//转码
	MessageBoxW(NULL, pBuffer, L"Text", MB_OK); //显示
	free(pBuffer); //释放缓存
	*/

	m_html.SetHTML(wszANSI);
	//m_html.SetHTML(htmlStr);

	delete wszANSI;
	delete szANSI;

}

void CHtmlReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC_FOR_HTML, staticForHtml);
}


BEGIN_MESSAGE_MAP(CHtmlReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CHtmlReportDlg::OnBnClickedButtonPrint)
END_MESSAGE_MAP()


// CTestHtmlDlg 消息处理程序


BOOL CHtmlReportDlg::OnInitDialog()
{
	CReportDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//InitTestHtml2();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

/*void CHtmlReportDlg::SetMeterReport(MeterReport* report){
	this->report = report;
}
void CHtmlReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}*/
void CHtmlReportDlg::DisplayReport(CalibrationReport* report){
	m_html.SetHideContextMenu(TRUE);
	m_html.CreateFromStatic(IDC_STATIC_FOR_HTML, this);
	m_html.Navigate(L"about:blank");  //这句话一定要写
	
	report->SetRefMeter(refData);
	CString* htmlStr = report->GetContent();

	//设置显示内容
	LPWSTR str = (htmlStr->GetBuffer(htmlStr->GetLength())); 

	m_html.SetHTML(str);
	delete htmlStr;

}

void CHtmlReportDlg::InitUIs(){

}

void CHtmlReportDlg::OnBnClickedButtonPrint()
{
	// TODO:  在此添加控件通知处理程序代码
	//CRect rect;
	CRect   dlgRect;
	CRect  htmlRect;
	//GetClientRect(&rect);
	//rect.right = 900;
	//rect.bottom = 500;
	//staticForHtml.GetWindowRect(&rect);
	//m_html.GetWindowRect(&htmlRect);
	//GetWindowRect(&dlgRect);
	m_html.GetClientRect(&htmlRect);

	int deltaTop = 20;//dlgRect.top;
	int deltaLeft = 40;//dlgRect.left;
	htmlRect.top += deltaTop;
	htmlRect.bottom += deltaTop;
	htmlRect.left += deltaLeft;
	htmlRect.right += deltaLeft;


	//CBitmap* image = ScreenShot(rect, rect.top, rect.left, "D:\\ShaoBF\\ScreenShot.jpg");
	PrintScreen(htmlRect, htmlRect.left, htmlRect.top);

}

void CHtmlReportDlg::PrintScreen(CRect rect, int left, int top){
	MyPrintWizard printW;
	CPrintDialog print(false);
	if (print.DoModal() == IDOK)
	{
		CDC printed;
		printed.Attach(print.GetPrinterDC());
		DOCINFO pdoc;
		pdoc.cbSize = sizeof(pdoc);
		pdoc.lpszDocName = L"pdoc";
		pdoc.lpszDatatype = NULL;
		pdoc.fwType = NULL;
		pdoc.lpszOutput = NULL;
		if (printed.StartDoc(&pdoc) >= 0)
		{
			LOGFONT logfont;
			memset(&logfont, 0, sizeof(LOGFONT));
			logfont.lfHeight = 75;
			CFont font;
			CFont *oldfont = NULL;

			Size paper_size;
			int xLogPPI = 0;
			int yLogPPI = 0;

			double paper_width;
			double paper_height;
			int nStartX, nStartY, nPicWidth, nPicHeight;

			nStartX = 0;
			nStartY = 0;
			nPicWidth = 0;
			nPicHeight = 0;

			HANDLE   pn;
			DEVMODE  *printDevMode;

			/// 关于打印机配置最重要的一个结构就是DEVMODE结构（结构的具体请MSDN），

			/// 该结构中几乎包含了打印机的所有配置信息。
			/// 返回打印机的配置（DEVMODE结构）的大小  
			CString printerNameStr = print.GetDeviceName();
			LPCTSTR printerName = (LPCTSTR)printerNameStr;
			LPTSTR lpszPrinterName = (LPTSTR)printerName;
			pn = printW.GetPrinterHandle(lpszPrinterName);
			int   nSize = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, NULL, 0);
			printDevMode = (DEVMODE *)malloc(nSize + 1);
			if (NULL == printDevMode)
			{
				return;
			}
			//ZeroMemory(print,nSize + 1);
			memset(printDevMode, 0, nSize + 1);

			/// 获取打印机配置信息
			LONG   lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, printDevMode, NULL, DM_OUT_BUFFER);
			printDevMode->dmFields = DM_PAPERSIZE | DM_ORIENTATION;
			// print->dmFields=DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;

			/// 将打印纸设置为A4
			//printDevMode->dmPaperSize = nPaperSizeIndex;

			//// print->dmPaperLength=m_PrintSize.x ; 
			//// print->dmPaperWidth= m_PrintSize.y;

			/// 将打印机设置为横向打印
			//printDevMode->dmOrientation = nPrintModel;

			////如果使用自定义纸张大小  
			//print->dwFields=DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;  
			//print->dmPaperLength=用户输入纸张的长度  
			//print->dmPaperWidth=用户输入纸张的宽度

			/// 设置打印机配置信息
			lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, printDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);

			/// 获取打印机HDC
			HDC hDC = ::CreateDC(NULL, lpszPrinterName, NULL, printDevMode);//GetPrinterDC(lpszPrinterName, NULL);

			HDC prtDC = hDC;
			DOCINFO di;

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


			/// 计算打印图片的大小宽高：nStartX nStartY nPicWidth nPicHeight
			long bmWidth = rect.Width();
			long bmHeight = rect.Height();
			if (bmWidth > bmHeight)
			{
				nPicWidth = paper_width;
				if (paper_width > bmWidth)
				{
					nPicHeight = bmHeight * (paper_width / bmWidth);
				}
				else
				{
					nPicHeight = rect.Height() * (bmWidth / paper_width);
				}
			}
			else
			{
				nPicHeight = paper_height;
				if (paper_height > bmHeight)
				{
					nPicWidth = bmWidth * (paper_height / bmHeight);
				}
				else
				{
					nPicWidth = bmWidth * (bmHeight / paper_height);
				}
			}
			int headX, headY;
			headX = 1500;
			headY = 280;

			nStartX = (paper_width - nPicWidth) / 2;
			nStartY = headY+130;//(paper_height - nPicHeight) / 2;

			nStartX = nStartX < 0 ? 0 : nStartX;
			nStartY = nStartY < 0 ? 0 : nStartY;







			if (font.CreateFontIndirect(&logfont))
				oldfont = (CFont*)printed.SelectObject(&font);

			//for (j = 1; j <= page; j++){
			printed.StartPage();
			int x = 500, y = 400;//A4纸，页面中的位置,横向为x轴，纵向是y轴,A4 maxX=4000 maxY=7000 建议按字符大小为75，每页安排40条纪录，初试纪录开始位置为x=500 y=200
			CString pageHead, pageBottom;
			pageHead.Format(_T("北京工业大学"));
			printed.TextOut(1500, 100, pageHead); //打印页眉
			CString title;//设置标题栏
			title.Format(_T("信息学部                电子信息工程实验班"));
			printed.TextOut(2500, 200, title); //打印页眉
			CString stt;
			stt.Format(_T("______________________________________________________________________________________"));
			printed.TextOut(500, 200 + 80, stt); //打印页眉

			/*for (int i = 1; (i<40) && (row<count); i++)
			{
			CString record(_T(""));
			record += this->resultList.GetItemText(row, 0) + L"     ";
			record += this->resultList.GetItemText(row, 1) + L"     ";
			record += this->resultList.GetItemText(row, 2);
			printed.TextOut(x, y, record);
			y += 80;
			printed.TextOut(x, y, stt);
			y += 80;
			row++;
			}*/
			//pageBottom.Format(_T("共%d页   第%d页"), page, j);
			printed.TextOut(1500, y, pageBottom);

			CDC*   memDC = new CDC();
			CPaintDC   dc(this);
			memDC->CreateCompatibleDC(&dc);
			CBitmap* memBitmap, *oldmemBitmap;                                        // 建立和屏幕兼容的bitmap
			memBitmap = new CBitmap();
			memBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

			oldmemBitmap = memDC->SelectObject(memBitmap);//将memBitmap选入内存DC
			memDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, left, top, SRCCOPY);  // 调解高度宽度
			//printed.BitBlt(nStartX, nStartY, nPicWidth, nPicHeight, memDC, left, top, SRCCOPY);
			printed.StretchBlt(nStartX, nStartY, nPicWidth, nPicHeight, memDC, left, top, bmWidth, bmHeight, SRCCOPY);

			/*Graphics graphics(printed.m_hDC); // Create a GDI+ graphics object
			//Image  image(L"D:\\ShaoBF\\ScreenShot.jpg");          //加载图片
			Image* image = BitmapToImage(bitmap);
			graphics.DrawImage(image, 0, 0, 900, 500);*/


			printed.EndPage();//此页结束 
			//}

			font.DeleteObject();
			if (oldfont != NULL)
				printed.SelectObject(oldfont);
			printed.EndDoc();
			//delete image;

		}
		printed.DeleteDC();

	}
}
