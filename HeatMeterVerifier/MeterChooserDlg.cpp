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
	vector<MeterInfo*>* meters = wizard.GetMeterInfoList();
	if (meters->size()==0){
		GetComList_256(&meterChooserList);
	}
	else{
		vector<MeterInfo*>::iterator it;
		for (it = meters->begin(); it != meters->end(); it++){
			CString meterStr = (*it)->MeterToString();
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
			wizard.AddMeterCodeCom(new MeterInfo(strCom));
			count++;
		}
		CloseHandle(hCom);
	} while (nCom<256);


	if (count == 0){
		AfxMessageBox(L"未找到串口。");
	}
	else{
		//读取表号
		ReadMetersID(wizard.GetMeterInfoList());
		//加入字符串
	}

}
void CMeterChooserDlg::ReadMetersID(vector<MeterInfo*>* meterInfoList){
	vector<MeterInfo*>::iterator it;
	for (it = meterInfoList->begin(); it != meterInfoList->end(); it++){
		CJ188 *cj188 = new CJ188(*it);
		(*it)->setCJ188(cj188);
		cj188->ReadAddress((*it),this);
		
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

void CMeterChooserDlg::OnFrameDataRecieved(char* data, DWORD bufferLen, CJ188Frame *frame, CJ188* cj188){
	MeterInfo *meterInfo = cj188->meterInfo;
	switch (frame->controlCode){
	case CJ188ReadAddress | CJ188_RESPONSE_BIT://是读地址操作的响应
		if (CJ188::IsValidFrame(frame)){//确认帧有效
			//将地址值变为字符串存入meterInfo
			meterInfo->code = Converter::HexToString(frame->address, 7, 0);
			//meterInfo->SetActive(true);
			//将meterInfo信息加入列表
			AddToMeterList(meterInfo);
		}
		break;
	}
}

void CMeterChooserDlg::AddToMeterList(MeterInfo* meterInfo){
	CString meterStr=meterInfo->code+"("+meterInfo->com+")";
	meterChooserList.AddString(meterStr);
}