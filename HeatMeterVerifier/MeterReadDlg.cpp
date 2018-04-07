// MeterReadDlg.cpp : ʵ���ļ�
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


// CMeterReadDlg ��Ϣ�������

extern CMeterWizard wizard;

BOOL CMeterReadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ������UI�ؼ�
	InitUIs();

	//�������ӵı��ַ��com�ڲ���ʾ
	InitMeterSelectList();

	//��ϸ��Ϣ�б���ʽ��ʾ����

	return true;
}

void CMeterReadDlg::InitUIs(){
	//���ɱ�ͷ
	CreateMeterDataTable();
	//���á�������⡱��Ч
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

	//������Ч�ͱ�ѡ�еı�/Com�˿�
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
	MeterInfo *meterInfo = cj188->meterInfo;
	switch (frame->controlCode){
	case CJ188ReadData | CJ188_RESPONSE_BIT://�Ƕ���ַ��������Ӧ
		ProcessMeterData(data, bufferLen, frame, cj188);
		break;
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
		UCHAR* address = dataFrame->frame->address;
		//�ж��Ƿ�ο���
		if (wizard.IsRefMeter(address)){//��Ϊ�ο�����һ�м�����
			meterReadList.InsertItem(i, L"*");//������
			//meterReadList.InsertItem(i, REF_METER_MARK);//������

		}
		else{//�����һ��Ϊ�հ�
			meterReadList.InsertItem(i, L" ");//������
		}
		int currentColumn = 1;
		meterReadList.SetItemText(i, currentColumn++, dataFrame->GetAddressStr());//������

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
		meterReadList.SetItemText(i, currentColumn++, Converter::HexToString(dataFrame->rawData,dataFrame->rawDataLength));
	}
	g_clsMutex->Unlock();
}

//���¡���ʼ���ԡ�/�����²��ԡ�����
void CMeterReadDlg::OnBnClickedReread()
{
	//���������Ѷ�����
	meterReadList.DeleteAllItems();
	meterDataTable->ClearAllData();
	//���¶�ȡ�������ݲ���������ʾ
	ReadMeters();
	testStarted = true;
	if (testStarted){
		testFinishedButton.EnableWindow(TRUE);
		testStartButton.SetWindowTextW(L"���¼��");
	}
}

//���¡����ζ���������
void CMeterReadDlg::OnBnClickedReadAgain()
{
	//�ط�����ָ������������������һ�ζ���֮�¡�
	ReadMeters();
	testStarted = false;
	if (!testStarted){
		testFinishedButton.EnableWindow(FALSE);
		testStartButton.SetWindowTextW(L"��ʼ���");
	}
}


void CMeterReadDlg::OnBnClickedSelectAll()
{
	//�ж�֮ǰ�Ƿ���ѡ�С�ȫѡ��
	if (selectAllMeterCheck.GetCheck() == 1){//��ѡ��ȫѡ��ȫ��ѡ����ȡ������ѡ���
		meterChooserList.SelItemRange(true, 0, meterChooserList.GetCount());
	}
	else if (selectAllMeterCheck.GetCheck() == 0){//����ȫѡ����ѡ�������
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

void CMeterReadDlg::GetComList_256(CListBox * list)//��ȡ����com��֧�ֵ�256��
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
			wizard.AddMeterAddressCom(new MeterInfo(strCom, nullptr));
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
	//��ȡ��ѡ�еı����ڣ���
	LPINT rgIndex = new int[MAXCOM];
	int count = GetSelectedMeter(rgIndex);

	//��������refMeterChooserBox��ѡ��
	refMeterChooserBox.ResetContent();
	//��������ѡ����
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


//ѡ�в�ͬ���ı��׼��ѡ������
void CMeterReadDlg::OnCbnSelchangeRefMeterChooserList()
{
	//�õ�ѡ�еı���±�
	int refIndex = refMeterChooserBox.GetCurSel();
	//����Ϊ�ο���
	wizard.setRefMeter(refIndex);
}


void CMeterReadDlg::OnHdnItemdblclickMetersList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMeterReadDlg::OnNMDblclkMetersList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int col = 0;
	int row = 0;
	//��ȡѡ�е��к�
	row = pNMListView->iItem;
	col = pNMListView->iSubItem;
	//˫��ĳһ�У���������֡�����Ի���
	CDataFrameDisplayDlg dlg;
	//��ȡѡ���е�����
	DataFrame *frame = meterDataTable->GetDataFrame(row);
	//�������ݵ������Ի���
	dlg.SetDataFrame(frame);
	//��ʾ�Ի���
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}
}
