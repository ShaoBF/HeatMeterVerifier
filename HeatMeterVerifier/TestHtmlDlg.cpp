// TestHtmlDlg.cpp : ʵ���ļ�
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


// CTestHtmlDlg �Ի���

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
	m_html.Navigate(L"about:blank");  //��仰һ��Ҫд

	//��ȡHTML�ļ����������
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
	//������ʾ����
	LPWSTR str = (htmlStr.GetBuffer(htmlStr.GetLength()));
	char* pStr = new char[htmlStr.GetLength()];
	sprintf(pStr, "%ws", htmlStr);
	int nLen = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pStr, -1, NULL, 0);
	//������Ҫ��unicode����     
	WCHAR * wszANSI = new WCHAR[nLen + 1];
	memset(wszANSI, 0, nLen * 2 + 2);
	nLen = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, wszANSI, nLen);    //��utf8ת��unicode    
	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);        //�õ�Ҫ��ansi����     
	char *szANSI = new char[nLen + 1];
	memset(szANSI, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL, NULL);          //��unicodeת��ansi     

	/*char *pStr = (htmlStr.GetBuffer(htmlStr.GetLength())); //ȡ��str�����ԭʼ�ַ���
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //ȡ�����軺��Ķ���
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//���뻺��ռ�
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize*sizeof(wchar_t));//ת��
	MessageBoxW(NULL, pBuffer, L"Text", MB_OK); //��ʾ
	free(pBuffer); //�ͷŻ���
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


// CTestHtmlDlg ��Ϣ�������


BOOL CHtmlReportDlg::OnInitDialog()
{
	CReportDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//InitTestHtml2();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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
	m_html.Navigate(L"about:blank");  //��仰һ��Ҫд
	
	report->SetRefMeter(refData);
	CString* htmlStr = report->GetContent();

	//������ʾ����
	LPWSTR str = (htmlStr->GetBuffer(htmlStr->GetLength())); 

	m_html.SetHTML(str);
	delete htmlStr;

}

void CHtmlReportDlg::InitUIs(){

}

void CHtmlReportDlg::OnBnClickedButtonPrint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

			/// ���ڴ�ӡ����������Ҫ��һ���ṹ����DEVMODE�ṹ���ṹ�ľ�����MSDN����

			/// �ýṹ�м��������˴�ӡ��������������Ϣ��
			/// ���ش�ӡ�������ã�DEVMODE�ṹ���Ĵ�С  
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

			/// ��ȡ��ӡ��������Ϣ
			LONG   lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, printDevMode, NULL, DM_OUT_BUFFER);
			printDevMode->dmFields = DM_PAPERSIZE | DM_ORIENTATION;
			// print->dmFields=DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;

			/// ����ӡֽ����ΪA4
			//printDevMode->dmPaperSize = nPaperSizeIndex;

			//// print->dmPaperLength=m_PrintSize.x ; 
			//// print->dmPaperWidth= m_PrintSize.y;

			/// ����ӡ������Ϊ�����ӡ
			//printDevMode->dmOrientation = nPrintModel;

			////���ʹ���Զ���ֽ�Ŵ�С  
			//print->dwFields=DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH|DM_ORIENTATION;  
			//print->dmPaperLength=�û�����ֽ�ŵĳ���  
			//print->dmPaperWidth=�û�����ֽ�ŵĿ��

			/// ���ô�ӡ��������Ϣ
			lt = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, printDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);

			/// ��ȡ��ӡ��HDC
			HDC hDC = ::CreateDC(NULL, lpszPrinterName, NULL, printDevMode);//GetPrinterDC(lpszPrinterName, NULL);

			HDC prtDC = hDC;
			DOCINFO di;

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


			/// �����ӡͼƬ�Ĵ�С��ߣ�nStartX nStartY nPicWidth nPicHeight
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
			int x = 500, y = 400;//A4ֽ��ҳ���е�λ��,����Ϊx�ᣬ������y��,A4 maxX=4000 maxY=7000 ���鰴�ַ���СΪ75��ÿҳ����40����¼�����Լ�¼��ʼλ��Ϊx=500 y=200
			CString pageHead, pageBottom;
			pageHead.Format(_T("������ҵ��ѧ"));
			printed.TextOut(1500, 100, pageHead); //��ӡҳü
			CString title;//���ñ�����
			title.Format(_T("��Ϣѧ��                ������Ϣ����ʵ���"));
			printed.TextOut(2500, 200, title); //��ӡҳü
			CString stt;
			stt.Format(_T("______________________________________________________________________________________"));
			printed.TextOut(500, 200 + 80, stt); //��ӡҳü

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
			//pageBottom.Format(_T("��%dҳ   ��%dҳ"), page, j);
			printed.TextOut(1500, y, pageBottom);

			CDC*   memDC = new CDC();
			CPaintDC   dc(this);
			memDC->CreateCompatibleDC(&dc);
			CBitmap* memBitmap, *oldmemBitmap;                                        // ��������Ļ���ݵ�bitmap
			memBitmap = new CBitmap();
			memBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

			oldmemBitmap = memDC->SelectObject(memBitmap);//��memBitmapѡ���ڴ�DC
			memDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, left, top, SRCCOPY);  // ����߶ȿ��
			//printed.BitBlt(nStartX, nStartY, nPicWidth, nPicHeight, memDC, left, top, SRCCOPY);
			printed.StretchBlt(nStartX, nStartY, nPicWidth, nPicHeight, memDC, left, top, bmWidth, bmHeight, SRCCOPY);

			/*Graphics graphics(printed.m_hDC); // Create a GDI+ graphics object
			//Image  image(L"D:\\ShaoBF\\ScreenShot.jpg");          //����ͼƬ
			Image* image = BitmapToImage(bitmap);
			graphics.DrawImage(image, 0, 0, 900, 500);*/


			printed.EndPage();//��ҳ���� 
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
