// MeterReadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "HeatMeterVerifier.h"
#include "MeterReadDlg.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"
#include "DataFrame.h"


// CMeterReadDlg 对话框

IMPLEMENT_DYNAMIC(CMeterReadDlg, CDialogEx)

CMeterReadDlg::CMeterReadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterReadDlg::IDD, pParent)
{
	meterDataTable = new MeterDataTable();
	g_clsMutex = new CMutex(FALSE, L"DataBufferMutex", NULL);
}

CMeterReadDlg::~CMeterReadDlg()
{
	delete meterDataTable;
	if (g_clsMutex){
		delete g_clsMutex;
	}
}

void CMeterReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_METERS_LIST, meterReadList);
}


BEGIN_MESSAGE_MAP(CMeterReadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMeterReadDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDOK, &CMeterReadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMeterReadDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMeterReadDlg 消息处理程序

extern CMeterWizard wizard;

BOOL CMeterReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem); 
	//可以插入列。iCol为列的位置，从零开始，lpszColumnHeading为显示的列名，nFormat为显示对齐方式， nWidth为显示宽度，nSubItem为分配给该列的列索引。
	CreateMeterDataTable();


	//读第一次读数
	ReadMeters();
	//详细信息列表形式显示读数

	return true;
}


void CMeterReadDlg::OnBnClickedPrevious()
{
	this->EndDialog(1);
	wizard.ChooseMeters();
}


void CMeterReadDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	wizard.GenerateReports();
}


void CMeterReadDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	//wizard.ChooseMeters();
	wizard.CloseAllCom();
}

void CMeterReadDlg::ReadMeters(){
	
	//vector<PMeterInfo>* meterInfoList = wizard.GetMeterInfoList();
	MyVector<MeterInfo*>* meterInfoList = wizard.GetMeterInfoList();

	int count = meterInfoList->GetSize();
	for (int i = 0; i < count; i++){
		//PMeterInfo info = (*meterInfoList)[i];
		MeterInfo* info = (*meterInfoList)[i];
		//创建CJ188实例并将meterInfo置入
		CJ188 *cj188 = info->GetCJ188();
		//发送读数据指令
		cj188->ReadMeterData(info, this);
	}
	/*
	//遍历有效和被选中的表/Com端口
	for (vector<MeterInfo*>::iterator it = meterInfoList->begin(); it != meterInfoList->end(); it++){

		//创建CJ188实例并将meterInfo置入
		CJ188 *cj188 = (*it)->GetCJ188();
		//发送读数据指令
		cj188->ReadMeterData(*it, this);
			
	}
	*/
}


void CMeterReadDlg::OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188){
	//解析数据（异步）并显示
	ProcessMeterData(data, bufferLen, frame, cj188);

}

void CMeterReadDlg::CreateMeterDataTable(){
	meterReadList.DeleteAllItems();
	//生成表头
	for (int i = 0; i < meterDataTable->GetColumnCount(); i++){
		ColumnProperty* columnProperty = meterDataTable->GetColumnProperty(i);
		meterReadList.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//设置列
	}
	//填充数据
	//UpdateMeterReadList();

}

void CMeterReadDlg::ProcessMeterData(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188){
	//填入数据
	DataFrame *dataFrame = new DataFrame(frame, data, bufferLen);
	dataFrame->ParseData();
	meterDataTable->AddData(dataFrame);

	//CreateMeterDataTable();
	UpdateMeterReadList();


}

void CMeterReadDlg::UpdateMeterReadList(){
	//重填数据
	g_clsMutex->Lock();
	meterReadList.DeleteAllItems();
	for (int i = 0; i < meterDataTable->GetRowCount(); i++){
		DataFrame* dataFrame = meterDataTable->GetDataFrame(i);
		meterReadList.InsertItem(i, dataFrame->GetAddressStr());//插入行

		int currentColumn = 1;
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetBillingDayHeatStr());//设置该行的不同列的显示字符
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetCurrentHeatStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetHeatPowerStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetFlowRateStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetBillingDayCapacityStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetTemperatureInStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetTemperatureOutStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetWorkHoursStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetCurrentTimeStr());
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetStatusStr());
		//meterReadList.SetItemText(i, currentColumn++, dataFrame->rawData);
	}
	g_clsMutex->Unlock();
}