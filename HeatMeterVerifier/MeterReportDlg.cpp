// MeterReportDlg.cpp : 实现文件
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

// CMeterReportDlg 对话框

IMPLEMENT_DYNAMIC(CMeterReportDlg, CReportDlg)

extern CMeterWizard wizard;

CMeterReportDlg::CMeterReportDlg(CWnd* pParent /*=NULL*/)
	: CReportDlg(CMeterReportDlg::IDD, pParent)
{
	//设置表头
	columnPropertyList.Add(new ColumnProperty(L"项目", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"数值", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"单位", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"参考区间", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"标准值", LVCFMT_LEFT, DATA_ITEM_WIDTH));
	columnPropertyList.Add(new ColumnProperty(L"结论", LVCFMT_LEFT, DATA_ITEM_WIDTH));
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


// CMeterReportDlg 消息处理程序

BOOL CMeterReportDlg::OnInitDialog()
{
	CReportDlg::OnInitDialog();

	//初始化所有UI控件
	//InitUIs();

	//显示检测报告数据项列表
	//DisplayReport();

	//详细信息列表形式显示读数

	return true;
}


void CMeterReportDlg::OnEnChangeMeterType()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

/*void CMeterReportDlg::SetMeterReport(MeterReport* report){
	this->report = report;
}
void CMeterReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}*/

void CMeterReportDlg::InitUIs(){
	//设置文本框信息
	//SetDialogBkColor
	//FIXME:以后改成可变的内容
	testUnit.SetWindowTextW(L"北京市计量科学研究院");
	meterID.SetWindowTextW(meterReport->GetAddressStr());
	factoryID.SetWindowTextW(Converter::HexToString(&(meterReport->address[5]), 2, 0));
	MeterDataInfo* meter = (MeterDataInfo*)meterReport->meter;
	meterType.SetWindowTextW(meter->GetMeterTypeStr());
	startTime.SetWindowTextW(meter->GetStartTimeStr());
	endTime.SetWindowTextW(meter->GetEndTimeStr());
	meterResult.SetWindowTextW(meter->GetReport()->GetQualifiedStr());

	//设置列表显示内容
	CreateReportList();
}

void CMeterReportDlg::DisplayReport(CalibrationReport* report){
	int currentRow = 0;
	MeterReport* refReport = refData->GetReport();

	double rate = wizard.GetVerifyRate();
	//热量
	InsertData(currentRow++, &(meterReport->heat), &(refReport->heat), L"", rate);
	//流量
	InsertData(currentRow++, &(meterReport->capacity), &(refReport->capacity), L"", rate);
	//进水温度
	InsertData(currentRow++, &(meterReport->temperatureIn), &(refReport->temperatureIn), L"℃", rate);
	//出水温度
	InsertData(currentRow++, &(meterReport->temperatureOut), &(refReport->temperatureOut), L"℃", rate);
	//持续时间
	InsertData(currentRow++, &(meterReport->duration), &(refReport->duration), L"sec", rate);
}

void CMeterReportDlg::CreateReportList(){
	resultList.DeleteAllItems();
	//生成表头
	for (int i = 0; i < columnPropertyList.GetSize(); i++){
		ColumnProperty* columnProperty = columnPropertyList.Get(i);
		resultList.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//设置列
	}
}

void CMeterReportDlg::InsertData(int row, DataItem* item, DataItem* refItem, CString unit, double rate){
	//名称
	resultList.InsertItem(row, item->name);//插入行
	int currentColumn = 1;
	//数值
	resultList.SetItemText(row, currentColumn++, item->GetValueStr());//设置列
	//单位
	if (!unit.IsEmpty()){
		resultList.SetItemText(row, currentColumn++, unit);
	}
	else{
		resultList.SetItemText(row, currentColumn++, CJ188::GetUnit(item->unit));
	}
	//参考区间
	resultList.SetItemText(row, currentColumn++, refItem->GetItemRangeStr(rate));
	//标准值
	resultList.SetItemText(row, currentColumn++, refItem->GetValueStr());
	//结果（偏高/偏低）
	CString verifyFlag = item->VerifyWith(refItem, rate);
	/*switch (item->VerifyWith(refItem,rate)){
	case -1:
		verifyFlag = "↓";
		break;
	case 0:
		verifyFlag = "-";
		break;
	case 1:
		verifyFlag = "↑";
		break;
	}*/
	resultList.SetItemText(row, currentColumn++, verifyFlag);
}

void CMeterReportDlg::OnBnClickedPrint()
{
	// TODO:  在此添加控件通知处理程序代码
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
	//拿到位图的句柄 Bitmap是Image的子类  
	//HBITMAP  hBmp = (HBITMAP)pBitmap->GetSafeHandle();
	HBITMAP  hBmp = (HBITMAP)pBitmap->m_hObject;
	//创建一个从位图句柄的Bitmap位图  
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
			int x = 500, y = 400;//A4纸，页面中的位置,横向为x轴，纵向是y轴,A4 maxX=4000 maxY=7000 建议按字符大小为75，每页安排40条纪录，初试纪录开始位置为x=500 y=200
			CString pageHead, pageBottom;
			pageHead.Format(_T("日志信息纪录统计表"));
			printed.TextOut(1500, 100, pageHead); //打印页眉
			CString title;//设置标题栏
			title.Format(_T("序号                时间                      操作"));
			printed.TextOut(500, 200, title); //打印页眉
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

			Graphics graphics(printed.m_hDC); // Create a GDI+ graphics object
			//Image  image(L"D:\\ShaoBF\\ScreenShot.jpg");          //加载图片
			Image* image = BitmapToImage(bitmap);
			graphics.DrawImage(image, 0, 0, 900, 500);

			//printed.BitBlt(0, 0, 900, 500, bitmap->dc);

			printed.EndPage();//此页结束 
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

extern TCHAR m_tchPrinterMessage[MAX_PAHT];///< 打印结果的返回信息

HANDLE CMeterReportDlg::GetPrinterHandle(LPTSTR lpszPrinterName)
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

			/// 关于打印机配置最重要的一个结构就是DEVMODE结构（结构的具体请MSDN），

			/// 该结构中几乎包含了打印机的所有配置信息。
			/// 返回打印机的配置（DEVMODE结构）的大小  
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
			nStartX = (paper_width - nPicWidth) / 2;
			nStartY = (paper_height - nPicHeight) / 2;

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

void CMeterReportDlg::OnPaint(){
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
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
		dc.FillSolidRect(rect, RGB(255, 255, 255));   //设置背景
		dc.FillPath();
		CDialogEx::OnPaint();
	}
}

CBitmap* CMeterReportDlg::ScreenShot(CRect rect, int left, int top, char *name){//截取窗口的大小，位置，名字（保存在默认路径下)
	/*CBitmap*  m_pBitmap;                                                      // 加入类成员
	//CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();                     // 获得截图窗口的指针，默认为主窗口，可以更改为其他的窗口。

	m_pBitmap = new   CBitmap;
	m_pBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());*/
	CPaintDC   dc(this);

	CDC*   memDC=new CDC();
	memDC->CreateCompatibleDC(&dc);
	CBitmap* memBitmap, *oldmemBitmap;                                        // 建立和屏幕兼容的bitmap
	memBitmap = new CBitmap();
	memBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	oldmemBitmap = memDC->SelectObject(memBitmap);//将memBitmap选入内存DC
	memDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, left, top, SRCCOPY);  // 调解高度宽度
	BITMAP bmp;
	memBitmap->GetBitmap(&bmp);                                               // 获得位图信息 

	FILE *fp = fopen(name, "w+b");

	BITMAPINFOHEADER bih = { 0 };                                              // 位图信息头
	bih.biBitCount = bmp.bmBitsPixel;                                        // 每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;                                             // 高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;                       // 图像数据大小
	bih.biWidth = bmp.bmWidth;                                               // 宽度

	BITMAPFILEHEADER bfh = { 0 };                                              // 位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);     // 到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;            // 文件总的大小
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);                           //写入位图文件头

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);                           //写入位图信息头

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];                    //申请内存保存位图数据

	GetDIBits(memDC->m_hDC, (HBITMAP)memBitmap->m_hObject, 0, rect.Height(), p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);                                    //获取位图数据

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);                       //写入位图数据
	delete[] p;
	fclose(fp);
	memDC->SelectObject(oldmemBitmap);
	//memDC->DeleteDC();
	return memBitmap;
}

CString CMeterReportDlg::GetReportTemplate(){
	return L"";
}