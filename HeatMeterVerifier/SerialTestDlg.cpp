// SerialTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialTestDlg.h"
#include "afxdialogex.h"
#include "Converter.h"
#include "ComConfig.h"
#include "MeterWizard.h"

#define FULL_FILE_PATH L"D:\\ShaoBF\\IniTest\\Config.ini"

extern KeyValue CJ188ControlList[];
extern KeyValue CJ188DIList[];
extern CMeterWizard wizard;

// CSerialTestDlg �Ի���

IMPLEMENT_DYNAMIC(CSerialTestDlg, CDialogEx)

extern ComConfig comConfig;

CSerialTestDlg::CSerialTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialTestDlg::IDD, pParent)
{
	//InitData();
}

CSerialTestDlg::~CSerialTestDlg()
{
	//�������ڣ��ر����д򿪵�
}

void CSerialTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BAUD_RATE, baudRateBox);
	DDX_Control(pDX, IDC_COM_BOX, comBox);
	DDX_Control(pDX, IDC_PARITY_BITS, parityBitsBox);
	DDX_Control(pDX, IDC_DATA_BITS, dataBitsBox);
	DDX_Control(pDX, IDC_STOP_BITS, stopBitsBox);
	DDX_Control(pDX, IDC_COM_ONOFF, comOnOffButton);
	DDX_Control(pDX, IDC_RECIEVED_DATA, recievedDataBox);
	DDX_Control(pDX, IDC_DATA_TO_SEND, sendDataBox);
	DDX_Control(pDX, IDC_HEX_SEND_CHECK, hexSendCheck);
	DDX_Control(pDX, IDC_HEX_RECEIVE_CHECK, hexRecieveCheck);
	DDX_Control(pDX, IDC_COMMAND_BOX, commandBox);
	DDX_Control(pDX, IDC_ADDRESS_BOX, meterAddressBox);
	DDX_Control(pDX, IDC_DI_BOX, diBox);
}


BEGIN_MESSAGE_MAP(CSerialTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SEND_DATA, &CSerialTestDlg::OnBnClickedSendData)
	ON_BN_CLICKED(IDC_COM_ONOFF, &CSerialTestDlg::OnBnClickedComOnoff)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE, &CSerialTestDlg::OnCbnSelchangeBaudRate)
	ON_CBN_SELCHANGE(IDC_COM_BOX, &CSerialTestDlg::OnCbnSelchangeComBox)
	ON_CBN_SELCHANGE(IDC_PARITY_BITS, &CSerialTestDlg::OnCbnSelchangeParityBits)
	ON_CBN_SELCHANGE(IDC_DATA_BITS, &CSerialTestDlg::OnCbnSelchangeDataBits)
	ON_CBN_SELCHANGE(IDC_STOP_BITS, &CSerialTestDlg::OnCbnSelchangeStopBits)
	ON_BN_CLICKED(IDC_CLEAR_SEND, &CSerialTestDlg::OnBnClickedClearSend)
	ON_BN_CLICKED(IDC_CLEAR_RECIEVE, &CSerialTestDlg::OnBnClickedClearRecieve)
	ON_BN_CLICKED(IDC_SEND_COMMAND, &CSerialTestDlg::OnBnClickedSendCommand)
	ON_CBN_SELCHANGE(IDC_COMMAND_BOX, &CSerialTestDlg::OnCbnSelchangeCommandBox)
	ON_COMMAND(ID_METER_SERIAL_SETTINGS, &CSerialTestDlg::OnMeterSerialSettings)
	ON_UPDATE_COMMAND_UI(ID_METER_SERIAL_SETTINGS, &CSerialTestDlg::OnUpdateMeterSerialSettings)
	ON_BN_CLICKED(IDOK, &CSerialTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialTestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSerialTestDlg ��Ϣ�������


void CSerialTestDlg::OnBnClickedSendData()
{
	CString dataStr;
	sendDataBox.GetWindowTextW(dataStr);
	CStringA strA = CStringA(dataStr);

	char* data;
	if (hexSendCheck.GetCheck() == 0){//δѡ��ʮ�����Ʒ��ͣ������ַ���
		data = strA.GetBuffer();
		serial[comBoxIndex].SendData(data, strlen(data));
	}
	else{//ѡ��ʮ�����Ʒ��ͣ�����ʮ����������
		data = Converter::StringToHex(dataStr);
		//���ڷ���
		serial[comBoxIndex].SendData(data, strlen(data));
	}
}


void CSerialTestDlg::OnBnClickedComOnoff()
{
	//�жϵ�ǰ���ڿ������
	if (comOn[comBoxIndex] == false){	//��ǰ����δ��
		//�õ���ǰ������Ϣ
		CollectCurrentConfig();

		//�򿪵�ǰ����
		OpenCurrentComm();

	}
	else {								//��ǰ�����Ѵ�
		//�رյ�ǰ����
		CloseCurrentComm();
	}
	//�л����ڿ���
	//comOn[comBoxIndex] = !comOn[comBoxIndex];

	//�ı���ʾ
	UpdateDisplay();
}

void CSerialTestDlg::Init(){
	comOn[0] = false;
}

//�õ���ǰ������Ϣ
void CSerialTestDlg::CollectCurrentConfig(){
	CString strText;
	comBoxIndex = comBox.GetCurSel();
	comBox.GetLBText(comBoxIndex, strText);
	int iLen = strText.GetLength();
	comID = new TCHAR[iLen];
	lstrcpy(comID, strText.GetBuffer(iLen));
}

void ComDataRecievedCB(void* receiver, UCHAR* buf,DWORD bufferLen){
	CSerialTestDlg* dlg = (CSerialTestDlg*)receiver;
	dlg->OnDataRecieved(buf, bufferLen);
/*	CEdit* recieveBox = &dlg->recievedDataBox;
	CString str;
	CString rawStr;

	if (dlg->hexRecieveCheck.GetCheck() == 0){//δѡ��ʮ��������ʾ��������ʾ�ַ���
		str = CString(buf);
	}
	else {//ѡ��ʮ��������ʾ����ʮ�����Ʒ�ʽ��ʾ���½�����Ϣ
		str = Converter::HexToString(buf, bufferLen);//CString(hexStr);
	}

	recieveBox->GetWindowTextW(rawStr);
	str = rawStr + str;// +_T("\r\n");
	recieveBox->SetWindowText(str);//ֱ���޸����������ֵ
	*/
}

//�򿪵�ǰ����
void CSerialTestDlg::OpenCurrentComm(){
	if (serial[comBoxIndex].OpenSerialPort(comID, baudRate, dataBits, stopBits, parityIndex,
		//&ComDataRecievedCB, 
		this)){  //�򿪴��ں��Զ���������
		//comHandle[comBoxIndex] = serial.m_hComm;
		comOn[comBoxIndex] = true;
	}
}

//�رյ�ǰ����
void CSerialTestDlg::CloseCurrentComm(){
	serial[comBoxIndex].CloseSerialPort();
	comOn[comBoxIndex] = false;
	//comHandle[comBoxIndex] = INVALID_HANDLE_VALUE;
}

BOOL CSerialTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	InitComBox();
	InitBaudRateBox();
	InitParityBitsBox();
	InitDataBitsBox();
	InitStopBitsBox();

	InitAddressBox();
	InitCommandBox();

	UpdateDisplay();
	return true;
}

void CSerialTestDlg::OnCbnSelchangeBaudRate()
{
	//��ȡ������
	CString strText;
	int nIndex = baudRateBox.GetCurSel();
	baudRateBox.GetLBText(nIndex, strText);
	baudRate = _ttoi(strText);
	if (comOn[comBoxIndex] == true){
		CloseCurrentComm();
		OpenCurrentComm();
	}
}

void CSerialTestDlg::UpdateDisplay(){
	if (comOn[comBoxIndex]){
		SetDlgItemText(IDC_COM_ONOFF, L"�رմ���");
	}
	else {
		SetDlgItemText(IDC_COM_ONOFF, L"��������");
	}
}
void CSerialTestDlg::InitComBox(){
	comBox.ResetContent();
	GetComList_256(&comBox);
}
void CSerialTestDlg::InitBaudRateBox(){
	baudRateBox.ResetContent();
	int baud = 300;
	CString str;
	for (int i = 0; i < 12; i++){
		str.Format(_T("%d"), baud);
		LPCTSTR pStr = LPCTSTR(str);
		baudRateBox.AddString(pStr);
		baud *= 2;
	}
	CString strText;
	int index = comConfig.GetBaudRateIndex();
	baudRateBox.SetCurSel(index);
	baudRate = comConfig.baudRate;
}

void CSerialTestDlg::InitParityBitsBox(){
	parityBitsBox.ResetContent();
	parityBitsBox.AddString(_T("��У��λ"));
	parityBitsBox.AddString(_T("��У��"));
	parityBitsBox.AddString(_T("żУ��"));
	parityIndex = comConfig.GetParityIndex();
	parityBitsBox.SetCurSel(parityIndex);
}
void CSerialTestDlg::InitDataBitsBox(){
	dataBitsBox.ResetContent();
	CString str;
	for (int i = 0; i < 3; i++){
		str.Format(_T("%d"), 8-i);
		LPCTSTR pStr = LPCTSTR(str);
		dataBitsBox.AddString(pStr);
	}
	int index = comConfig.GetDataBitsIndex();
	dataBitsBox.SetCurSel(index);
	dataBits = comConfig.dataBits;
}
void CSerialTestDlg::InitStopBitsBox(){
	stopBitsBox.ResetContent();
	stopBitsBox.AddString(_T("1"));
	stopBitsBox.AddString(_T("2"));
	int index = comConfig.GetStopBitsIndex();
	stopBitsBox.SetCurSel(index);
	stopBits = comConfig.stopBits;
}

void CSerialTestDlg::InitCommandBox(){
	int commandCount = CJ188_CONTROL_CODE_COUNT;
	for (int i = 0; i < commandCount; i++){
		commandBox.AddString(CString(CJ188ControlList[i].value));
	}
}
void CSerialTestDlg::InitAddressBox(){
	meterAddressBox.AddString(L"AA AA AA AA AA AA AA");
}
void CSerialTestDlg::OnCbnSelchangeComBox()
{
	//�ر�ǰһ���򿪵Ĵ���
	if (comOn[comBoxIndex] == true){
		CloseHandle(serial[comBoxIndex].m_hComm);
		comOn[comBoxIndex] = false;
	}
	//��ȡ��������
	CollectCurrentConfig();
	UpdateDisplay();
}

void CSerialTestDlg::GetComList_256(CComboBox * CCombox)//��ȡ����com��֧�ֵ�256��
{
	CString  strCom, strComOpen;
	int  nCom = 0;
	int  count = 0;
	HANDLE    hCom;

	CCombox->ResetContent();
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
			CCombox->AddString(strCom);
			comOn[count++] = false;
		}
		CloseHandle(hCom);
	} while (nCom<256);

	if (count == 0){
		AfxMessageBox(L"δ�ҵ����ڡ�");
	}

	CCombox->SetCurSel(0);
	comBoxIndex = 0;
}

void CSerialTestDlg::OnCbnSelchangeParityBits()
{
	parityIndex = parityBitsBox.GetCurSel();
	if (comOn[comBoxIndex] == true){
		CloseCurrentComm();
		OpenCurrentComm();
	}
}


void CSerialTestDlg::OnCbnSelchangeDataBits()
{
	dataBits = 8 - dataBitsBox.GetCurSel();
	if (comOn[comBoxIndex] == true){
		CloseCurrentComm();
		OpenCurrentComm();
	}
}


void CSerialTestDlg::OnCbnSelchangeStopBits()
{
	stopBits = 1 + stopBitsBox.GetCurSel();
	if (comOn[comBoxIndex] == true){
		CloseCurrentComm();
		OpenCurrentComm();
	}
}


void CSerialTestDlg::OnBnClickedClearSend()
{
	sendDataBox.SetWindowText(L"");
}


void CSerialTestDlg::OnBnClickedClearRecieve()
{
	recievedDataBox.SetWindowText(L"");
}


void CSerialTestDlg::OnBnClickedSendCommand()
{
	//����CJ188ָ��֡
	CJ188Frame* frame = GetRequestFrame();
	//����Ϊ16����ָ��
	int length;
	char* data = new char[CJ188_FRAME_LENGTH_AFTER_DATA + frame->dataLength + CJ188_FRAME_LENGTH_BEFORE_DATA + CJ188_FRAME_INIT_LENGTH];
	CJ188::ToHexCommand(data,frame,length);
	//����ָ��
	SendData(data,length);
	delete data;
	delete frame->data;
	delete frame;
}
void CSerialTestDlg::SendData(char* data,int length){
	serial[comBoxIndex].SendData(data, length);
}
CJ188Frame* CSerialTestDlg::GetRequestFrame(){
	CJ188Frame *frame;

	frame = CJ188::CreateRequestFrame(GetAddress(), GetControlCode(), GetDI());
	return frame;
}



UCHAR* CSerialTestDlg::GetAddress(){
	int index = meterAddressBox.GetCurSel();
	UCHAR* address;
	CString addrStr;
	meterAddressBox.GetLBText(index, addrStr);
	address = (UCHAR*)Converter::StringToHex(addrStr);
	return address;
}
UCHAR CSerialTestDlg::GetControlCode(){
	int index = commandBox.GetCurSel();
	return CJ188ControlList[index].key;
}
UCHAR* CSerialTestDlg::GetDI(){
	int index = diBox.GetCurSel();
	//UCHAR* di=new UCHAR[2];
	return (UCHAR*)(&kvlist[index].key);
}


void CSerialTestDlg::OnCbnSelchangeCommandBox()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡ��ǰѡ�������
	int controlIndex = commandBox.GetCurSel();
	//����DIѡ���
	ResetDIBox(CJ188ControlList[controlIndex].key);
}

void CSerialTestDlg::ResetDIBox(UCHAR controlCode){
	//����ǰDIѡ���
	//diBox.Clear();
	diBox.ResetContent();
	//���ݵ�ǰѡ�еĿ����֣����벻ͬ����
	switch (controlCode){
	case CJ188ReadData:		//ѡ�ж����ݲ���
		//��DIѡ������ݱ�Ϊ������ַ�������
		kvlist = CJ188DIList;
		kvCount = 17;
		break;
	case CJ188WriteData:		//ѡ�ж����ݲ���
		//��DIѡ������ݱ�Ϊ������ַ�������
		kvlist = &CJ188DIList[CJ188_WRITE_DATA_DI_OFFSET];
		kvCount = CJ188_WRITE_DATA_DI_COUNT;
		break;
	case CJ188ReadKeyVersion://ѡ�С�����Կ�汾�š�����
		//��DIѡ������ݱ�Ϊ������Կ�汾�š������
		kvlist = &CJ188DIList[CJ188_READ_VER_DI_OFFSET];
		kvCount = CJ188_READ_VER_DI_COUNT;
		break;
	case CJ188ReadAddress://ѡ�С�����ַ������
		//��DIѡ������ݱ�Ϊ������ַ�������
		kvlist = &CJ188DIList[CJ188_READ_ADDRESS_DI_OFFSET];
		kvCount = CJ188_READ_ADDRESS_DI_COUNT;
		break;
	case CJ188WriteAddress://ѡ�С�д��ַ������
		//��DIѡ������ݱ�Ϊ��д��ַ�������
		kvlist = &CJ188DIList[CJ188_WRITE_ADDRESS_DI_OFFSET];
		kvCount = CJ188_WRITE_ADDRESS_DI_COUNT;
		break;
	case CJ188WriteMeterBase://ѡ�С�д��ַ������
		//��DIѡ������ݱ�Ϊ��д��ַ�������
		kvlist = &CJ188DIList[CJ188_WRITE_BASE_DI_OFFSET];
		kvCount = CJ188_WRITE_BASE_DI_COUNT;
		break;
	default:
		kvCount = 0;
		break;
	}
	//����ѡ�������
	for (int i = 0; i < kvCount; i++){
		diBox.AddString(CString(kvlist[i].value));
	}
}


void CSerialTestDlg::OnMeterSerialSettings()
{
	CSerialTestDlg dlg;
	dlg.Init();
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


void CSerialTestDlg::OnUpdateMeterSerialSettings(CCmdUI *pCmdUI)
{
	if (pCmdUI->m_nID == ID_METER_SERIAL_SETTINGS){
		pCmdUI->Enable(TRUE);
	}
}

void CSerialTestDlg::OnDataRecieved(UCHAR* buf,DWORD bufferLen){
	//CEdit* recieveBox = &recievedDataBox;
	CString str;
	CString rawStr;

	if (hexRecieveCheck.GetCheck() == 0){//δѡ��ʮ��������ʾ��������ʾ�ַ���
		str = CString(buf);
	}
	else {//ѡ��ʮ��������ʾ����ʮ�����Ʒ�ʽ��ʾ���½�����Ϣ
		str = Converter::HexToString((UCHAR*)buf, bufferLen);//CString(hexStr);
	}

	recievedDataBox.GetWindowTextW(rawStr);
	str = rawStr + str;// +_T("\r\n");
	recievedDataBox.SetWindowText(str);//ֱ���޸����������ֵ

}
void CSerialTestDlg::OnBnClickedOk()
{
	//��������ѡ��
	SaveConfig();
	CDialogEx::OnOK();
}

void CSerialTestDlg::SaveConfig(){
	comConfig.baudRate = baudRate;
	comConfig.dataBits = dataBits;
	comConfig.stopBits = stopBits;
	comConfig.parityIndex = parityIndex;
	CString configPath = wizard.GetConfigFilePath();
	//TODO�������ﴢ�洮��������Ϣ��ini�ļ�
	if (comConfig.SaveConfig(configPath)){
		AfxMessageBox(L"����������Ϣ�ѱ��档");
	}
}


void CSerialTestDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
