// MeterChooserDlg.cpp : ʵ���ļ�
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


// CMeterChooserDlg �Ի���

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


// CMeterChooserDlg ��Ϣ�������



extern CMeterWizard wizard;

void CMeterChooserDlg::OnBnClickedSelectAll()
{
	//�ж�֮ǰ�Ƿ���ѡ�С�ȫѡ��
	;
	if (SelectAllCheck.GetCheck() == 1){//��ѡ��ȫѡ��ȫ��ѡ����ȡ������ѡ���
		meterChooserList.SelItemRange(true, 0, meterChooserList.GetCount());
	}
	else if (SelectAllCheck.GetCheck() == 0){//����ȫѡ����ѡ�������
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

void CMeterChooserDlg::GetComList_256(CListBox * list)//��ȡ����com��֧�ֵ�256��
{
	CString  strCom, strComOpen;
	int  nCom = 0;
	int  count = 0;
	HANDLE    hCom;
	//��ȡ����COM��
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
			DWORD error = ::GetLastError();//ȡ�ô�����Ϣ 
		}
		else
		{
			//�������ڶ�Ӧ��Ϣ����MeterCodeCom�б���
			wizard.AddMeterAddressCom(new MeterInfo(strCom,nullptr));
			count++;
		}
		CloseHandle(hCom);
	} while (nCom<256);


	if (count == 0){
		AfxMessageBox(L"δ�ҵ����ڡ�");
	}
	else{
		//��ȡ���
		ReadMetersAddress(wizard.GetMeterInfoList());
		//�����ַ���
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
	//��ȡ��ѡ�еı����ڣ���
	LPINT rgIndex = new int[MAXCOM];
	int count = GetSelectedMeter(rgIndex);
	wizard.SetSelectMeterIndex(rgIndex,count);
	//�����򵼵Ķ�����
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
	case CJ188ReadAddress | CJ188_RESPONSE_BIT://�Ƕ���ַ��������Ӧ
		if (CJ188::IsValidFrame(frame)){//ȷ��֡��Ч
			//����ֵַ��Ϊ�ַ�������meterInfo
			meterInfo->address = frame->address;
			//meterInfo->SetActive(true);
			//��meterInfo��Ϣ�����б�
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
	//�����ر����д򿪴���
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
