// MeterChooserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterChooserDlg.h"
#include "afxdialogex.h"
#include "MeterReadDlg.h"
#include "MeterWizard.h"
#include "MeterInfo.h"
#include "CJ188.h"
#include "Converter.h"


// CMeterChooserDlg 对话框

IMPLEMENT_DYNAMIC(CMeterChooserDlg, CDialogEx)

CMeterChooserDlg::CMeterChooserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterChooserDlg::IDD, pParent)
{

}

CMeterChooserDlg::~CMeterChooserDlg()
{
}

void CMeterChooserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_METER_CHOOSER_LIST, meterChooserList);
	DDX_Control(pDX, IDC_SELECT_ALL, SelectAllCheck);
	DDX_Control(pDX, IDC_REF_METER_CHOOSER_LIST, refMeterCombobox);
}
BOOL CMeterChooserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitMeterSelectList();
	return true;
}


BEGIN_MESSAGE_MAP(CMeterChooserDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CMeterChooserDlg::OnBnClickedSelectAll)
	ON_BN_CLICKED(IDOK, &CMeterChooserDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMeterChooserDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMeterChooserDlg 消息处理程序



extern CMeterWizard wizard;

void CMeterChooserDlg::OnBnClickedSelectAll()
{
	//判断之前是否已选中“全选”
	;
	if (SelectAllCheck.GetCheck() == 1){//已选中全选则“全不选”（取消所有选中项）
		meterChooserList.SelItemRange(true, 0, meterChooserList.GetCount());
	}
	else if (SelectAllCheck.GetCheck() == 0){//否则“全选”（选中所有项）
		meterChooserList.SelItemRange(false, 0, meterChooserList.GetCount());
	}
}

void CMeterChooserDlg::InitMeterSelectList(){
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

void CMeterChooserDlg::GetComList_256(CListBox * list)//获取可用com口支持到256个
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
			wizard.AddMeterAddressCom(new MeterInfo(strCom,nullptr));
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
void CMeterChooserDlg::ReadMetersAddress(MyVector<MeterInfo*>* meterInfoList){
	//vector<MeterInfo*>::iterator it;
	//for (it = meterInfoList->begin(); it != meterInfoList->end(); it++){
	for (int i = 0; i < meterInfoList->GetSize(); i++){
		CJ188 *cj188 = (*meterInfoList)[i]->GetCJ188();
		(*meterInfoList)[i]->SetCJ188(cj188);
		cj188->ReadAddress((*meterInfoList)[i], this);
	}
}

void CMeterChooserDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	//读取已选中的表（串口）项
	LPINT rgIndex = new int[MAXCOM];
	int count = GetSelectedMeter(rgIndex);
	wizard.SetSelectMeterIndex(rgIndex,count);
	//调出向导的读表功能
	wizard.ReadMeters();
}

int CMeterChooserDlg::GetSelectedMeter(LPINT rgIndex){
	int selCount = meterChooserList.GetSelCount();
	//LPINT rgIndex = new int[selCount];
	meterChooserList.GetSelItems(meterChooserList.GetCount(), rgIndex);
	return selCount;
}

void CMeterChooserDlg::OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame *frame, CJ188* cj188){
	MeterInfo *meterInfo = cj188->meterInfo;
	switch (frame->controlCode){
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

void CMeterChooserDlg::AddToMeterList(MeterInfo* meterInfo){
	CString meterStr = meterInfo->GetAddressString() + "(" + meterInfo->com + ")";
	meterChooserList.AddString(meterStr);
}

void CMeterChooserDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	//遍历关闭所有打开串口
	wizard.CloseAllCom();
	/*
	vector<MeterInfo*>* meterInfoList=wizard.GetMeterInfoList();
	vector<MeterInfo*>::iterator it;
	for (it = meterInfoList->begin(); it != meterInfoList->end(); it++){
		if ((*it)->active){
			(*it)->serial.CloseSerialPort();
		}
	}*/
}
