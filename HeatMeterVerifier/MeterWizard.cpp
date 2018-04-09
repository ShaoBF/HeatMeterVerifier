#include "stdafx.h"
#include "MeterWizard.h"


CMeterWizard::CMeterWizard()
{
	//meterInfoList = new MeterInfo*[128];
}


CMeterWizard::~CMeterWizard()
{
	//ʩ��MeterInfo
	/*vector<PMeterInfo>::iterator it;
	for (it = meterInfoList.begin(); it != meterInfoList.end(); it++)
		delete *it;
	meterInfoList.clear();*/
	for (int i = 0; i < meterInfoList.GetSize(); i++){
		delete meterInfoList[i];
	}
	meterInfoList.Clear();
}

void CMeterWizard::ChooseMeters(){
	CMeterChooserDlg dlg;
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

void CMeterWizard::ReadMeters(){
	CMeterReadDlg dlg;
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

void CMeterWizard::GenerateReports(){
	CMetersReportDlg dlg;
	dlg.SetReports(GetMeterReports());

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

void CMeterWizard::AddMeterAddressCom(MeterInfo *meterInfo){
	meterInfoList.Add(meterInfo);
	//meterInfoList[meterCount++] = meterInfo;
}

void CMeterWizard::SetSelectMeterIndex(LPINT rgIndex, int count){
	selectedMeterIndex = rgIndex;
	selectedMeterCount = count;
	/*vector<MeterInfo*>::iterator it;
	for (it = meterInfoList.begin(); it != meterInfoList.end(); it++){
		(*it)->selected = false;
	}
	for (int i = 0; i < count; i++){
		meterInfoList[rgIndex[i]]->selected = true;
	}*/
	for (int i = 0; i < meterInfoList.GetSize(); i++){
		meterInfoList[i]->selected = false;
	}
	for (int i = 0; i < count; i++){
		meterInfoList[rgIndex[i]]->selected = true;
	}
}

void CMeterWizard::CloseAllCom(){
	//	vector<MeterInfo*>::iterator it;
	//	for (it = meterInfoList.begin(); it != meterInfoList.end(); it++){
	for (int i = 0; i<meterInfoList.GetSize(); i++){
		if (meterInfoList[i]->active){
			meterInfoList[i]->serial.CloseSerialPort();
		}
	}
}

MeterInfo* CMeterWizard::GetMeterInfo(int index){
	return meterInfoList[index];
}

void CMeterWizard::setRefMeter(int index){
	refMeterIndex = index;
}

bool CMeterWizard::IsRefMeter(UCHAR* address){
	UCHAR* refAddress = meterInfoList[refMeterIndex]->address;
	for (int i = 0; i < CJ188_ADDRESS_LENGTH; i++){
		if (address[i] != refAddress[i]){
			return false;
		}
	}
	return true;
}

MeterInfo* CMeterWizard::GetMeterInfo(UCHAR* address){
	int i;
	int j;
	for (j = 0; j < meterInfoList.GetSize(); j++){
		UCHAR* refAddress = meterInfoList[j]->address;
		for (i = 0; i < CJ188_ADDRESS_LENGTH; i++){
			if (address[i] != refAddress[i]){
				break;
			}
		}
		if (i == CJ188_ADDRESS_LENGTH){
			return meterInfoList[j];
		}
	}
	return nullptr;
}

MyVector<MeterReport*>* CMeterWizard::GetMeterReports(){
	MyVector<MeterReport*>* reports = new MyVector<MeterReport*>();
	for (int i = 0; i < meterInfoList.GetSize(); i++){
		MeterDataInfo* info = (MeterDataInfo*)meterInfoList[i];
		MeterReport* report = info->GetReport();
		reports->Add(report);
	}
	return reports;
}

LPCTSTR CMeterWizard::GetConnectStr(){
	//FIXME: would retrive form config file.
	return _T("ODBC;DSN=HeatMeterDS32;UID=ShaoBF;PWD=shbofe");
}
