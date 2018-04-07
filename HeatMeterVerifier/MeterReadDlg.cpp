// MeterReadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "HeatMeterVerifier.h"
#include "MeterReadDlg.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"
#include "DataFrame.h"
#include "Converter.h"
#include "DataFrameDisplayDlg.h"

#define REF_METER_MARK L"*"

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
	meterDataTable->ClearAllData();
	delete meterDataTable;
	if (g_clsMutex){
		delete g_clsMutex;
	}
}

void CMeterReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_METERS_LIST, meterReadList);
	DDX_Control(pDX, IDC_METER_CHOOSER_LIST, meterChooserList);
	DDX_Control(pDX, IDC_REF_METER_CHOOSER_LIST, refMeterChooserBox);
	DDX_Control(pDX, IDC_SELECT_ALL, selectAllMeterCheck);
	DDX_Control(pDX, IDC_READ_AGAIN, testFinishedButton);
	DDX_Control(pDX, IDC_REREAD, testStartButton);
}


BEGIN_MESSAGE_MAP(CMeterReadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMeterReadDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDOK, &CMeterReadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMeterReadDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_REREAD, &CMeterReadDlg::OnBnClickedReread)
	ON_BN_CLICKED(IDC_READ_AGAIN, &CMeterReadDlg::OnBnClickedReadAgain)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CMeterReadDlg::OnBnClickedSelectAll)
	ON_LBN_SELCHANGE(IDC_METER_CHOOSER_LIST, &CMeterReadDlg::OnLbnSelchangeMeterChooserList)
	ON_CBN_SELCHANGE(IDC_REF_METER_CHOOSER_LIST, &CMeterReadDlg::OnCbnSelchangeRefMeterChooserList)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CMeterReadDlg::OnHdnItemdblclickMetersList)
	ON_NOTIFY(NM_DBLCLK, IDC_METERS_LIST, &CMeterReadDlg::OnNMDblclkMetersList)
END_MESSAGE_MAP()


// CMeterReadDlg 消息处理程序

extern CMeterWizard wizard;

BOOL CMeterReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化所有UI控件
	InitUIs();

	//读已连接的表地址与com口并显示
	InitMeterSelectList();

	//详细信息列表形式显示读数

	return true;
}

void CMeterReadDlg::InitUIs(){
	//生成表头
	CreateMeterDataTable();
	//设置“结束检测”无效
	testFinishedButton.EnableWindow(FALSE);
	meterReadList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}


void CMeterReadDlg::OnBnClickedPrevious()
{
	meterDataTable->ClearAllData();
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

	//遍历有效和被选中的表/Com端口
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
	MeterInfo *meterInfo = cj188->meterInfo;
	switch (frame->controlCode){
	case CJ188ReadData | CJ188_RESPONSE_BIT://是读地址操作的响应
		ProcessMeterData(data, bufferLen, frame, cj188);
		break;
	case CJ188ReadAddress | CJ188_RESPONSE_BIT://是读地址操作的响应
		if (CJ188::IsValidFrame(frame)){//确认帧有效
			//将地址值变为字符串存入meterInfo
			meterInfo->address = frame->address;
			//meterInfo->SetActive(true);
			//将meterInfo信息加入列表
			AddToMeterList(meterInfo);
		}
		break;
	}

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
		UCHAR* address = dataFrame->frame->address;
		//判断是否参考表
		if (wizard.IsRefMeter(address)){//若为参考表，第一列加入标记
			meterReadList.InsertItem(i, L"*");//插入行
			//meterReadList.InsertItem(i, REF_METER_MARK);//插入行

		}
		else{//否则第一列为空白
			meterReadList.InsertItem(i, L" ");//插入行
		}
		int currentColumn = 1;
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetAddressStr());//插入行

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
		meterReadList.SetItemText(i, currentColumn++, Converter::HexToString(dataFrame->rawData,dataFrame->rawDataLength));
	}
	g_clsMutex->Unlock();
}

//按下“开始测试”/“重新测试”按键
void CMeterReadDlg::OnBnClickedReread()
{
	//清理所有已读数据
	meterReadList.DeleteAllItems();
	meterDataTable->ClearAllData();
	//重新读取所有数据并放入表格显示
	ReadMeters();
	testStarted = true;
	if (testStarted){
		testFinishedButton.EnableWindow(TRUE);
		testStartButton.SetWindowTextW(L"重新检测");
	}
}

//按下“二次读数”按键
void CMeterReadDlg::OnBnClickedReadAgain()
{
	//重发读表指令，并将读数结果插入上一次读数之下。
	ReadMeters();
	testStarted = false;
	if (!testStarted){
		testFinishedButton.EnableWindow(FALSE);
		testStartButton.SetWindowTextW(L"开始检测");
	}
}


void CMeterReadDlg::OnBnClickedSelectAll()
{
	//判断之前是否已选中“全选”
	if (selectAllMeterCheck.GetCheck() == 1){//已选中全选则“全不选”（取消所有选中项）
		meterChooserList.SelItemRange(true, 0, meterChooserList.GetCount());
	}
	else if (selectAllMeterCheck.GetCheck() == 0){//否则“全选”（选中所有项）
		meterChooserList.SelItemRange(false, 0, meterChooserList.GetCount());
	}
	OnLbnSelchangeMeterChooserList();
}

void CMeterReadDlg::InitMeterSelectList(){
	MyVector<MeterInfo*>* meters = wizard.GetMeterInfoList();
	int meterCount = meters->GetSize();
	if (meterCount == 0){
		GetComList_256(&meterChooserList);
	}
	else{
		//vector<MeterInfo*>::iterator it;
		//for (it = meters->begin(); it != meters->end(); it++){
		for (int i = 0; i < meterCount; i++){
			CString meterStr = (*meters)[i]->MeterToString();
			meterChooserList.AddString(meterStr);
		}
	}
}

void CMeterReadDlg::GetComList_256(CListBox * list)//获取可用com口支持到256个
{
	CString  strCom, strComOpen;
	int  nCom = 0;
	int  count = 0;
	HANDLE    hCom;
	//读取可用COM口
	list->ResetContent();
	do
	{
		nCom++;
		strCom.Format(L"COM%d:", nCom);
		strComOpen.Format(L"\\\\.\\COM%d", nCom);
		hCom = CreateFile(strComOpen, GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		if (INVALID_HANDLE_VALUE == hCom)
		{
			DWORD error = ::GetLastError();//取得错误信息 
		}
		else
		{
			//将表、串口对应信息放入MeterCodeCom列表中
			wizard.AddMeterAddressCom(new MeterInfo(strCom, nullptr));
			count++;
		}
		CloseHandle(hCom);
	} while (nCom<256);


	if (count == 0){
		AfxMessageBox(L"未找到串口。");
	}
	else{
		//读取表号
		ReadMetersAddress(wizard.GetMeterInfoList());
		//加入字符串
	}

}
void CMeterReadDlg::ReadMetersAddress(MyVector<MeterInfo*>* meterInfoList){
	//vector<MeterInfo*>::iterator it;
	//for (it = meterInfoList->begin(); it != meterInfoList->end(); it++){
	for (int i = 0; i < meterInfoList->GetSize(); i++){
		CJ188 *cj188 = (*meterInfoList)[i]->GetCJ188();
		(*meterInfoList)[i]->SetCJ188(cj188);
		cj188->ReadAddress((*meterInfoList)[i], this);
	}
}

void CMeterReadDlg::AddToMeterList(MeterInfo* meterInfo){
	CString meterStr = meterInfo->GetAddressString() + "(" + meterInfo->com + ")";
	meterChooserList.AddString(meterStr);
}


void CMeterReadDlg::OnLbnSelchangeMeterChooserList()
{
	//读取已选中的表（串口）项
	LPINT rgIndex = new int[MAXCOM];
	int count = GetSelectedMeter(rgIndex);

	//清理所有refMeterChooserBox内选项
	refMeterChooserBox.ResetContent();
	//加入所有选中项
	for (int i = 0; i < count; i++){
		MeterInfo* meterInfo = wizard.GetMeterInfo(rgIndex[i]);
		CString meterStr = meterInfo->GetAddressString() + "(" + meterInfo->com + ")";
		refMeterChooserBox.AddString(meterStr);
	}
}


int CMeterReadDlg::GetSelectedMeter(LPINT rgIndex){
	int selCount = meterChooserList.GetSelCount();
	//LPINT rgIndex = new int[selCount];
	meterChooserList.GetSelItems(meterChooserList.GetCount(), rgIndex);
	return selCount;
}


//选中不同表，改变标准表选项设置
void CMeterReadDlg::OnCbnSelchangeRefMeterChooserList()
{
	//得到选中的表的下标
	int refIndex = refMeterChooserBox.GetCurSel();
	//设置为参考表
	wizard.setRefMeter(refIndex);
}


void CMeterReadDlg::OnHdnItemdblclickMetersList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMeterReadDlg::OnNMDblclkMetersList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int col = 0;
	int row = 0;
	//获取选中的行号
	row = pNMListView->iItem;
	col = pNMListView->iSubItem;
	//双击某一行，弹出数据帧解析对话框
	CDataFrameDisplayDlg dlg;
	//获取选中行的数据
	DataFrame *frame = meterDataTable->GetDataFrame(row);
	//设置数据到解析对话框
	dlg.SetDataFrame(frame);
	//显示对话框
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}
}
