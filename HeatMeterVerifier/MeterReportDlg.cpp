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

// CMeterReportDlg �Ի���

IMPLEMENT_DYNAMIC(CMeterReportDlg, CDialogEx)

extern CMeterWizard wizard;

CMeterReportDlg::CMeterReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterReportDlg::IDD, pParent)
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
}


BEGIN_MESSAGE_MAP(CMeterReportDlg, CDialogEx)
	ON_EN_CHANGE(IDC_METER_TYPE, &CMeterReportDlg::OnEnChangeMeterType)
	ON_BN_CLICKED(IDC_PRINT, &CMeterReportDlg::OnBnClickedPrint)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMeterReportDlg ��Ϣ�������

BOOL CMeterReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ������UI�ؼ�
	InitUIs();

	//��ʾ��ⱨ���������б�
	DisplayReport();

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

void CMeterReportDlg::SetMeterReport(MeterReport* report){
	this->report = report;
}
void CMeterReportDlg::SetRefMeterData(MeterDataInfo* info){
	this->refData = info;
}

void CMeterReportDlg::InitUIs(){
	//�����ı�����Ϣ
	//SetDialogBkColor
	//FIXME:�Ժ�ĳɿɱ������
	testUnit.SetWindowTextW(L"�����м�����ѧ�о�Ժ");
	meterID.SetWindowTextW(report->GetAddressStr());
	factoryID.SetWindowTextW(Converter::HexToString(&(report->address[5]), 2, 0));
	MeterDataInfo* meter = (MeterDataInfo*)report->meter;
	meterType.SetWindowTextW(meter->GetMeterTypeStr());
	startTime.SetWindowTextW(meter->GetStartTimeStr());
	endTime.SetWindowTextW(meter->GetEndTimeStr());

	//�����б���ʾ����
	CreateReportList();
}

void CMeterReportDlg::DisplayReport(){
	int currentRow = 0;
	MeterReport* refReport = refData->GetReport();

	double rate = wizard.GetVerifyRate();
	//����
	InsertData(currentRow++, &(report->heat), &(refReport->heat), L"", rate);
	//����
	InsertData(currentRow++, &(report->capacity), &(refReport->capacity), L"", rate);
	//��ˮ�¶�
	InsertData(currentRow++, &(report->temperatureIn), &(refReport->temperatureIn), L"��", rate);
	//��ˮ�¶�
	InsertData(currentRow++, &(report->temperatureOut), &(refReport->temperatureOut), L"��", rate);
	//����ʱ��
	InsertData(currentRow++, &(report->duration), &(refReport->duration), L"sec", rate);
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
	CString verifyFlag;
	switch (item->VerifyWith(refItem,rate)){
	case -1:
		verifyFlag = "��";
		break;
	case 0:
		verifyFlag = "-";
		break;
	case 1:
		verifyFlag = "��";
		break;
	}
	resultList.SetItemText(row, currentColumn++, verifyFlag);
}

void CMeterReportDlg::OnBnClickedPrint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

				printed.EndPage();//��ҳ���� 
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
		//CPaintDC   dc(this);
		GetClientRect(&rect);
		dc.FillSolidRect(rect, RGB(255, 255, 255));   //���ñ���
		dc.FillPath();
		CDialogEx::OnPaint();
	}
}

void Screen(char filename[])
{
	CDC *pDC;//��ĻDC
	pDC = CDC::FromHandle(GetDC(NULL));//��ȡ��ǰ������ĻDC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);

	printf("��ǰ��Ļɫ��ģʽΪ%dλɫ��n", BitPerPixel);
	printf("��Ļ��ȣ�%dn", Width);
	printf("��Ļ�߶ȣ�%dn", Height);

	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ

	FILE *fp = fopen(filename, "w+b");

	BITMAPINFOHEADER bih = { 0 };//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//���

	BITMAPFILEHEADER bfh = { 0 };//λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����

	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, Height, p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);//��ȡλͼ����

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����

	delete[] p;

	fclose(fp);

	memDC.SelectObject(oldmemBitmap);
}