#pragma once
#include "afxwin.h"
#include "resource.h"
#include "CSerial.h"
#include "CJ188.h"


// CSerialTestDlg �Ի���
#define COMMAND_READ_DATA		0
#define COMMAND_READ_ADDRESS	1


class CSerialTestDlg : public CDialogEx, public ComDataReciever
{
	DECLARE_DYNAMIC(CSerialTestDlg)

public:
	CSerialTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSerialTestDlg();

	void Init();
	void OnDataRecieved(UCHAR* data, DWORD bufferLen);

// �Ի�������
	enum { IDD = IDD_SERIALTEST };

protected:
	CSerial serial[MAXCOM];
	bool comOn[8];
	//HANDLE comHandle[8];
	int comBoxIndex = 0;
	int parityIndex = NOPARITY;
	int dataBits = 8;
	int stopBits = 1;
	TCHAR* comID;
	int baudRate=9600;
	KeyValue* kvlist;
	int kvCount;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
	//�õ���ǰ������Ϣ
	void CollectCurrentConfig();
	//�򿪵�ǰ����
	void OpenCurrentComm();
	//�رյ�ǰ����
	void CloseCurrentComm();

	//�л���ʾ
	void UpdateDisplay();

	DECLARE_MESSAGE_MAP()
public:
	// �����б�
	CComboBox comBox;
	// ������ѡ��
	CComboBox baudRateBox;
	// У��λѡ��
	CComboBox parityBitsBox;
	// ����λѡ��
	CComboBox dataBitsBox;
	// ֹͣλѡ��
	CComboBox stopBitsBox;

	// ���ش���
	CButton comOnOffButton;

	// ������
	CEdit recievedDataBox;
	// ������
	CEdit sendDataBox;

	//ʮ�����Ʒ��͸�ѡ��
	CButton hexSendCheck;
	// ʮ�����ƽ�����ʾ
	CButton hexRecieveCheck;

	//ָ���ѡ����
	CComboBox commandBox;
	//��Ƶ�ַѡ����
	CComboBox meterAddressBox;
	// ���ݱ�ʶѡ��
	CComboBox diBox;

	void InitComBox();
	void InitBaudRateBox();
	void InitParityBitsBox();
	void InitDataBitsBox();
	void InitStopBitsBox();
	void InitAddressBox();
	void InitCommandBox();


	afx_msg void OnCbnSelchangeComBox();
	afx_msg void OnCbnSelchangeBaudRate();
	afx_msg void OnBnClickedSendData();
	afx_msg void OnBnClickedComOnoff();

	friend void ComDataRecievedCB(void* receiver, char* buf);
	void GetComList_256(CComboBox * CCombox);
	afx_msg void OnCbnSelchangeParityBits();
	afx_msg void OnCbnSelchangeDataBits();
	afx_msg void OnCbnSelchangeStopBits();
	afx_msg void OnBnClickedClearSend();
	afx_msg void OnBnClickedClearRecieve();
	afx_msg void OnBnClickedSendCommand();

	CJ188Frame* GetRequestFrame();
	void SendData(char* data, int length);
	UCHAR* GetAddress();
	UCHAR GetControlCode();
	UCHAR* GetDI();
	afx_msg void OnCbnSelchangeCommandBox();
	void ResetDIBox(UCHAR controlCode);
	afx_msg void OnMeterSerialSettings();
	afx_msg void OnUpdateMeterSerialSettings(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedOk();
	void SaveConfig();
	afx_msg void OnBnClickedCancel();
};