// MeterReadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterReadDlg.h"
#include "afxdialogex.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"


// CMeterReadDlg 对话框

IMPLEMENT_DYNAMIC(CMeterReadDlg, CDialogEx)

CMeterReadDlg::CMeterReadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterReadDlg::IDD, pParent)
{

}

CMeterReadDlg::~CMeterReadDlg()
{
}

void CMeterReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_METERS_LIST, MeterReadList);
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
}

void CMeterReadDlg::ReadMeters(){
	vector<MeterInfo*>* meterInfoList = wizard.GetMeterInfoList();
	//遍历有效和被选中的表/Com端口
	vector<MeterInfo*>::iterator it;
	for (it = meterInfoList->begin(); it != meterInfoList->end(); it++){
		//创建CJ188实例并将meterInfo置入
		CJ188 *cj188 = (*it)->GetCJ188();
		//发送读数据指令
		cj188->ReadMeterData(*it, this);
		
	}
	//解析数据（异步）并显示
}


void CMeterReadDlg::OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188){

}