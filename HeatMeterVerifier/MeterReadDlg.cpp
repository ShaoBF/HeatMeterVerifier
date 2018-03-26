// MeterReadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "MeterReadDlg.h"
#include "afxdialogex.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"


// CMeterReadDlg �Ի���

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


// CMeterReadDlg ��Ϣ�������

extern CMeterWizard wizard;

BOOL CMeterReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//����һ�ζ���
	ReadMeters();
	//��ϸ��Ϣ�б���ʽ��ʾ����

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
	//������Ч�ͱ�ѡ�еı�/Com�˿�
	vector<MeterInfo*>::iterator it;
	for (it = meterInfoList->begin(); it != meterInfoList->end(); it++){
		//����CJ188ʵ������meterInfo����
		CJ188 *cj188 = (*it)->GetCJ188();
		//���Ͷ�����ָ��
		cj188->ReadMeterData(*it, this);
		
	}
	//�������ݣ��첽������ʾ
}


void CMeterReadDlg::OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188){

}