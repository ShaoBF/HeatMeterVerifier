// MeterReadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "HeatMeterVerifier.h"
#include "MeterReadDlg.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"
#include "DataFrame.h"


// CMeterReadDlg �Ի���

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


// CMeterReadDlg ��Ϣ�������

extern CMeterWizard wizard;

BOOL CMeterReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem); 
	//���Բ����С�iColΪ�е�λ�ã����㿪ʼ��lpszColumnHeadingΪ��ʾ��������nFormatΪ��ʾ���뷽ʽ�� nWidthΪ��ʾ��ȣ�nSubItemΪ��������е���������
	CreateMeterDataTable();


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
	wizard.CloseAllCom();
}

void CMeterReadDlg::ReadMeters(){
	
	//vector<PMeterInfo>* meterInfoList = wizard.GetMeterInfoList();
	MyVector<MeterInfo*>* meterInfoList = wizard.GetMeterInfoList();

	int count = meterInfoList->GetSize();
	for (int i = 0; i < count; i++){
		//PMeterInfo info = (*meterInfoList)[i];
		MeterInfo* info = (*meterInfoList)[i];
		//����CJ188ʵ������meterInfo����
		CJ188 *cj188 = info->GetCJ188();
		//���Ͷ�����ָ��
		cj188->ReadMeterData(info, this);
	}
	/*
	//������Ч�ͱ�ѡ�еı�/Com�˿�
	for (vector<MeterInfo*>::iterator it = meterInfoList->begin(); it != meterInfoList->end(); it++){

		//����CJ188ʵ������meterInfo����
		CJ188 *cj188 = (*it)->GetCJ188();
		//���Ͷ�����ָ��
		cj188->ReadMeterData(*it, this);
			
	}
	*/
}


void CMeterReadDlg::OnFrameDataRecieved(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188){
	//�������ݣ��첽������ʾ
	ProcessMeterData(data, bufferLen, frame, cj188);

}

void CMeterReadDlg::CreateMeterDataTable(){
	meterReadList.DeleteAllItems();
	//���ɱ�ͷ
	for (int i = 0; i < meterDataTable->GetColumnCount(); i++){
		ColumnProperty* columnProperty = meterDataTable->GetColumnProperty(i);
		meterReadList.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//������
	}
	//�������
	//UpdateMeterReadList();

}

void CMeterReadDlg::ProcessMeterData(UCHAR* data, DWORD bufferLen, CJ188Frame* frame, CJ188* cj188){
	//��������
	DataFrame *dataFrame = new DataFrame(frame, data, bufferLen);
	dataFrame->ParseData();
	meterDataTable->AddData(dataFrame);

	//CreateMeterDataTable();
	UpdateMeterReadList();


}

void CMeterReadDlg::UpdateMeterReadList(){
	//��������
	g_clsMutex->Lock();
	meterReadList.DeleteAllItems();
	for (int i = 0; i < meterDataTable->GetRowCount(); i++){
		DataFrame* dataFrame = meterDataTable->GetDataFrame(i);
		meterReadList.InsertItem(i, dataFrame->GetAddressStr());//������

		int currentColumn = 1;
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetBillingDayHeatStr());//���ø��еĲ�ͬ�е���ʾ�ַ�
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