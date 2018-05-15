// MeterReportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterReportDlg.h"
#include "afxdialogex.h"
#include "Converter.h"
#include "TableDataDefines.h"
#include "MyVector.cpp"
//#include "VerifyConfig.h"
#include "MeterWizard.h"
#include <gdiplus.h>
#pragma comment(lib, "winspool.lib")
#include <winspool.h>

using namespace Gdiplus;

// CMeterReportDlg �Ի���

IMPLEMENT_DYNAMIC(CMeterReportDlg, CReportDlg)

extern CMeterWizard wizard;

CMeterReportDlg::CMeterReportDlg(CWnd* pParent /*=NULL*/)
	: CReportDlg(CMeterReportDlg::IDD, pParent)
{
	//���ñ�ͷ
	columnPropertyList.Add(new ColumnProperty(L"��Ŀ", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"��ֵ", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"��λ", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"�ο�����", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"��׼ֵ", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"����", LVCFMT_LEFT, DATA_ITEM_WIDTH));
}

CMeterReportDlg::~CMeterReportDlg()
{
}

void CMeterReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEST_UNIT, testUnit);
	DDX_Control(pDX, IDC_TEST_MODE, testMode);
	DDX_Control(pDX, IDC_METERID, meterID);
	DDX_Control(pDX, IDC_MANUFATORY, factoryID);
	DDX_Control(pDX, IDC_METER_TYPE, meterType);
	DDX_Control(pDX, IDC_TEST_START_TIME, startTime);
	DDX_Control(pDX, IDC_TEST_END_TIME, endTime);
	DDX_Control(pDX, IDC_TESTER, tester);
	DDX_Control(pDX, IDC_REVIEWER, reviewer);
	DDX_Control(pDX, IDC_METER_PARAMETER_LIST, resultList);
	DDX_Control(pDX, IDC_RESULT, meterResult);
}


BEGIN_MESSAGE_MAP(CMeterReportDlg, CDialogEx)
	ON_EN_CHANGE(IDC_METER_TYPE, &CMeterReportDlg::OnEnChangeMeterType)
	ON_BN_CLICKED(IDC_PRINT, &CMeterReportDlg::OnBnClickedPrint)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMeterReportDlg ��Ϣ�������

BOOL CMeterReportDlg::OnInitDialog()
{
	CReportDlg::OnInitDialog();

	//��ʼ������UI�ؼ�
	//InitUIs();

	//��ʾ��ⱨ���������б�
	//DisplayReport();

	//��ϸ��Ϣ�б���ʽ��ʾ����

	return true;
}


void CMeterReportDlg::OnEnChangeMeterType()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

/*void CMeterReportDlg::SetMeterReport(MeterReport* report){
	this->report = report;
}
void CMeterReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}*/

void CMeterReportDlg::InitUIs(){
	//�����ı�����Ϣ
	//SetDialogBkColor
	//FIXME:�Ժ�ĳɿɱ������
	testUnit.SetWindowTextW(L"�����м�����ѧ�о�Ժ");
	meterID.SetWindowTextW(meterReport->GetAddressStr());
	factoryID.SetWindowTextW(Converter::HexToString(&(meterReport->address[5]), 2, 0));
	MeterDataInfo* meter = (MeterDataInfo*)meterReport->meter;
	meterType.SetWindowTextW(meter->GetMeterTypeStr());
	startTime.SetWindowTextW(meter->GetStartTimeStr());
	endTime.SetWindowTextW(meter->GetEndTimeStr());
	meterResult.SetWindowTextW(meter->GetReport()->GetQualifiedStr());

	//�����б���ʾ����
	CreateReportList();
}

void CMeterReportDlg::DisplayReport(CalibrationReport* report){
	int currentRow = 0;
	MeterReport* refReport = refData->GetReport();

	double rate = wizard.GetVerifyRate();
	//����
	InsertData(currentRow++, &(meterReport->heat), &(refReport->heat), L"", rate);
	//����
	InsertData(currentRow++, &(meterReport->capacity), &(refReport->capacity), L"", rate);
	//��ˮ�¶�
	InsertData(currentRow++, &(meterReport->temperatureIn), &(refReport->temperatureIn), L"��", rate);
	//��ˮ�¶�
	InsertData(currentRow++, &(meterReport->temperatureOut), &(refReport->temperatureOut), L"��", rate);
	//����ʱ��
	InsertData(currentRow++, &(meterReport->duration), &(refReport->duration), L"sec", rate);
}

void CMeterReportDlg::CreateReportList(){
	resultList.DeleteAllItems();
	//���ɱ�ͷ
	for (int i = 0; i < columnPropertyList.GetSize(); i++){
		ColumnProperty* columnProperty = columnPropertyList.Get(i);
		resultList.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//������
	}
}

void CMeterReportDlg::InsertData(int row, DataItem* item, DataItem* refItem, CString unit, double rate){
	//����
	resultList.InsertItem(row, item->name);//������
	int currentColumn = 1;
	//��ֵ
	resultList.SetItemText(row, currentColumn++, item->GetValueStr());//������
	//��λ
	if (!unit.IsEmpty()){
		resultList.SetItemText(row, currentColumn++, unit);
	}
	else{
		resultList.SetItemText(row, currentColumn++, CJ188::GetUnit(item->unit));
	}
	//�ο�����
	resultList.SetItemText(row, currentColumn++, refItem->GetItemRangeStr(rate));
	//��׼ֵ
	resultList.SetItemText(row, currentColumn++, refItem->GetValueStr());
	//�����ƫ��/ƫ�ͣ�
	CString verifyFlag = item->VerifyWith(refItem, rate);
	/*switch (item->VerifyWith(refItem,rate)){
	case -1:
		verifyFlag = "��";
		break;
	case 0:
		verifyFlag = "-";
		break;
	case 1:
		verifyFlag = "��";
		break;
	}*/
	resultList.SetItemText(row, currentColumn++, verifyFlag);
}

void CMeterReportDlg::OnBnClickedPrint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//testPrint();
	CRect   rect;
	GetClientRect(&rect);
	rect.right = 900;
	rect.bottom = 500;

	//CBitmap* image = ScreenShot(rect, rect.top, rect.left, "D:\\ShaoBF\\ScreenShot.jpg");
	PrintScreen(rect, rect.top, rect.left);
	//testPrint(image);
}

Image* BitmapToImage(CBitmap *pBitmap)
{
	//�õ�λͼ�ľ�� Bitmap��Image������  
	//HBITMAP  hBmp = (HBITMAP)pBitmap->GetSafeHandle();
	HBITMAP  hBmp = (HBITMAP)pBitmap->m_hObject;
	//����һ����λͼ�����Bitmapλͼ  
	Image* img = new Bitmap(hBmp, NULL);
	return img;
}

void CMeterReportDlg::testPrint(CBitmap* bitmap){
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
			if (font.CreateFontIndirect(&logfont))
				oldfont = (CFont*)printed.SelectObject(&font);

			//for (j = 1; j <= page; j++){
			printed.StartPage();
			int x = 500, y = 400;//A4ֽ��ҳ���е�λ��,����Ϊx�ᣬ������y��,A4 maxX=4000 maxY=7000 ���鰴�ַ���СΪ75��ÿҳ����40����¼�����Լ�¼��ʼλ��Ϊx=500 y=200
			CString pageHead, pageBottom;
			pageHead.Format(_T("��־��Ϣ��¼ͳ�Ʊ�"));
			printed.TextOut(1500, 100, pageHead); //��ӡҳü
			CString title;//���ñ�����
			title.Format(_T("���                ʱ��                      ����"));
			printed.TextOut(500, 200, title); //��ӡҳü
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

			Graphics graphics(printed.m_hDC); // Create a GDI+ graphics object
			//Image  image(L"D:\\ShaoBF\\ScreenShot.jpg");          //����ͼƬ
			Image* image = BitmapToImage(bitmap);
			graphics.DrawImage(image, 0, 0, 900, 500);

			//printed.BitBlt(0, 0, 900, 500, bitmap->dc);

			printed.EndPage();//��ҳ���� 
			//}

			font.DeleteObject();
			if (oldfont != NULL)
				printed.SelectObject(oldfont);
			printed.EndDoc();
			delete image;

		}
		printed.DeleteDC();

	}
}
#define MAX_PAHT 256

extern TCHAR m_tchPrinterMessage[MAX_PAHT];///< ��ӡ����ķ�����Ϣ

HANDLE CMeterReportDlg::GetPrinterHandle(LPTSTR lpszPrinterName)
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


void CMeterReportDlg::PrintScreen(CRect rect, int left, int top){
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

			int paper_width;
			int paper_height;
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
			pn = this->GetPrinterHandle(lpszPrinterName);
			int   nSize = ::DocumentProperties(m_hWnd, pn, lpszPrinterName, NULL, NULL, 0);
			printDevMode = (DEVMODE *)malloc(nSize + 1);
			if (NULL == printDevMode)
			{
				return ;
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
			nStartX = (paper_width - nPicWidth) / 2;
			nStartY = (paper_height - nPicHeight) / 2;

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

void CMeterReportDlg::OnPaint(){
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect   rect;
		GetClientRect(&rect);
		dc.FillSolidRect(rect, RGB(255, 255, 255));   //���ñ���
		dc.FillPath();
		CDialogEx::OnPaint();
	}
}

CBitmap* CMeterReportDlg::ScreenShot(CRect rect, int left, int top, char *name){//��ȡ���ڵĴ�С��λ�ã����֣�������Ĭ��·����)
	/*CBitmap*  m_pBitmap;                                                      // �������Ա
	//CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();                     // ��ý�ͼ���ڵ�ָ�룬Ĭ��Ϊ�����ڣ����Ը���Ϊ�����Ĵ��ڡ�

	m_pBitmap = new   CBitmap;
	m_pBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());*/
	CPaintDC   dc(this);

	CDC*   memDC=new CDC();
	memDC->CreateCompatibleDC(&dc);
	CBitmap* memBitmap, *oldmemBitmap;                                        // ��������Ļ���ݵ�bitmap
	memBitmap = new CBitmap();
	memBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	oldmemBitmap = memDC->SelectObject(memBitmap);//��memBitmapѡ���ڴ�DC
	memDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, left, top, SRCCOPY);  // ����߶ȿ��
	BITMAP bmp;
	memBitmap->GetBitmap(&bmp);                                               // ���λͼ��Ϣ 

	FILE *fp = fopen(name, "w+b");

	BITMAPINFOHEADER bih = { 0 };                                              // λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;                                        // ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;                                             // �߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;                       // ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;                                               // ���

	BITMAPFILEHEADER bfh = { 0 };                                              // λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);     // ��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;            // �ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);                           //д��λͼ�ļ�ͷ

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);                           //д��λͼ��Ϣͷ

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];                    //�����ڴ汣��λͼ����

	GetDIBits(memDC->m_hDC, (HBITMAP)memBitmap->m_hObject, 0, rect.Height(), p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);                                    //��ȡλͼ����

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);                       //д��λͼ����
	delete[] p;
	fclose(fp);
	memDC->SelectObject(oldmemBitmap);
	//memDC->DeleteDC();
	return memBitmap;
}

CString CMeterReportDlg::GetReportTemplate(){
	return L"";
}