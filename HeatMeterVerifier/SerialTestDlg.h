#pragma once
#include "afxwin.h"
#include "resource.h"
#include "CSerial.h"
#include "CJ188.h"


// CSerialTestDlg 对话框
#define COMMAND_READ_DATA		0
#define COMMAND_READ_ADDRESS	1


class CSerialTestDlg : public CDialogEx, public ComDataReciever
{
	DECLARE_DYNAMIC(CSerialTestDlg)

public:
	CSerialTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSerialTestDlg();

	void Init();
	void OnDataRecieved(UCHAR* data, DWORD bufferLen);

// 对话框数据
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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
	//得到当前串口信息
	void CollectCurrentConfig();
	//打开当前串口
	void OpenCurrentComm();
	//关闭当前串口
	void CloseCurrentComm();

	//切换显示
	void UpdateDisplay();

	DECLARE_MESSAGE_MAP()
public:
	// 串口列表
	CComboBox comBox;
	// 波特率选项
	CComboBox baudRateBox;
	// 校验位选项
	CComboBox parityBitsBox;
	// 数据位选项
	CComboBox dataBitsBox;
	// 停止位选项
	CComboBox stopBitsBox;

	// 开关串口
	CButton comOnOffButton;

	// 接收区
	CEdit recievedDataBox;
	// 发送区
	CEdit sendDataBox;

	//十六进制发送复选框
	CButton hexSendCheck;
	// 十六进制接收显示
	CButton hexRecieveCheck;

	//指令发送选择栏
	CComboBox commandBox;
	//表计地址选择栏
	CComboBox meterAddressBox;
	// 数据标识选择
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