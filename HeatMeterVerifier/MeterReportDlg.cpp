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

// CMeterReportDlg 对话框

IMPLEMENT_DYNAMIC(CMeterReportDlg, CDialogEx)

extern CMeterWizard wizard;

CMeterReportDlg::CMeterReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterReportDlg::IDD, pParent)
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
}


BEGIN_MESSAGE_MAP(CMeterReportDlg, CDialogEx)
	ON_EN_CHANGE(IDC_METER_TYPE, &CMeterReportDlg::OnEnChangeMeterType)
	ON_BN_CLICKED(IDC_PRINT, &CMeterReportDlg::OnBnClickedPrint)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMeterReportDlg 消息处理程序

BOOL CMeterReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化所有UI控件
	InitUIs();

	//显示检测报告数据项列表
	DisplayReport();

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

void CMeterReportDlg::SetMeterReport(MeterReport* report){
	this->report = report;
}
void CMeterReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}

void CMeterReportDlg::InitUIs(){
	//设置文本框信息
	//SetDialogBkColor
	//FIXME:以后改成可变的内容
	testUnit.SetWindowTextW(L"北京市计量科学研究院");
	meterID.SetWindowTextW(report->GetAddressStr());
	factoryID.SetWindowTextW(Converter::HexToString(&(report->address[5]), 2, 0));
	MeterDataInfo* meter = (MeterDataInfo*)report->meter;
	meterType.SetWindowTextW(meter->GetMeterTypeStr());
	startTime.SetWindowTextW(meter->GetStartTimeStr());
	endTime.SetWindowTextW(meter->GetEndTimeStr());

	//设置列表显示内容
	CreateReportList();
}

void CMeterReportDlg::DisplayReport(){
	int currentRow = 0;
	MeterReport* refReport = refData->GetReport();

	double rate = wizard.GetVerifyRate();
	//热量
	InsertData(currentRow++, &(report->heat), &(refReport->heat), L"", rate);
	//流量
	InsertData(currentRow++, &(report->capacity), &(refReport->capacity), L"", rate);
	//进水温度
	InsertData(currentRow++, &(report->temperatureIn), &(refReport->temperatureIn), L"℃", rate);
	//出水温度
	InsertData(currentRow++, &(report->temperatureOut), &(refReport->temperatureOut), L"℃", rate);
	//持续时间
	InsertData(currentRow++, &(report->duration), &(refReport->duration), L"sec", rate);
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
	CString verifyFlag;
	switch (item->VerifyWith(refItem,rate)){
	case -1:
		verifyFlag = "↓";
		break;
	case 0:
		verifyFlag = "-";
		break;
	case 1:
		verifyFlag = "↑";
		break;
	}
	resultList.SetItemText(row, currentColumn++, verifyFlag);
}

void CMeterReportDlg::OnBnClickedPrint()
{
	// TODO:  在此添加控件通知处理程序代码
	testPrint();
}

void CMeterReportDlg::testPrint(){
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

			//for (j = 1; j <= page; j++)
			//{
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

				printed.EndPage();//此页结束 
			//}
			font.DeleteObject();
			if (oldfont != NULL)
				printed.SelectObject(oldfont);
			printed.EndDoc();
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
		//CPaintDC   dc(this);
		GetClientRect(&rect);
		dc.FillSolidRect(rect, RGB(255, 255, 255));   //设置背景
		dc.FillPath();
		CDialogEx::OnPaint();
	}
}

void Screen(char filename[])
{
	CDC *pDC;//屏幕DC
	pDC = CDC::FromHandle(GetDC(NULL));//获取当前整个屏幕DC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);

	printf("当前屏幕色彩模式为%d位色彩n", BitPerPixel);
	printf("屏幕宽度：%dn", Width);
	printf("屏幕高度：%dn", Height);

	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息

	FILE *fp = fopen(filename, "w+b");

	BITMAPINFOHEADER bih = { 0 };//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度

	BITMAPFILEHEADER bfh = { 0 };//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据

	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, Height, p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);//获取位图数据

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据

	delete[] p;

	fclose(fp);

	memDC.SelectObject(oldmemBitmap);
}