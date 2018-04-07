// DataFrameDisplayDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "DataFrameDisplayDlg.h"
#include "afxdialogex.h"
#include "MyVector.cpp"
#include "Converter.h"


// CDataFrameDisplayDlg �Ի���

IMPLEMENT_DYNAMIC(CDataFrameDisplayDlg, CDialogEx)

CDataFrameDisplayDlg::CDataFrameDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataFrameDisplayDlg::IDD, pParent)
{


}

CDataFrameDisplayDlg::~CDataFrameDisplayDlg()
{
}

void CDataFrameDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RAW_FRAME, rawFrameDisplay);
	DDX_Control(pDX, IDC_DATA_ITEM_LIST, dataListCtrl);
}


BEGIN_MESSAGE_MAP(CDataFrameDisplayDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataFrameDisplayDlg ��Ϣ�������

BOOL CDataFrameDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ������UI�ؼ�
	InitUIs();

	//����������չʾ���
	DisplayDataList();

	//��ϸ��Ϣ�б���ʽ��ʾ����

	return true;
}


void CDataFrameDisplayDlg::SetDataFrame(DataFrame* frame){
	this->dataFrame = frame;
}

//��ʼ������UI�ؼ�
void CDataFrameDisplayDlg::InitUIs(){
	//columnPropertyList = new ColumnProperty*[columnCount];
	int current = 0;
	ColumnProperty* column;
	column = new ColumnProperty(L"������Ŀ", LVCFMT_LEFT, 100);
	columnPropertyList.Add(column);
	column = new ColumnProperty(L"����ֵ", LVCFMT_LEFT, 200);
	columnPropertyList.Add(column);
	//column = new ColumnProperty(L"��λ", LVCFMT_LEFT, 60);
	//columnPropertyList.Add(column);

}

//����������չʾ���
void CDataFrameDisplayDlg::DisplayDataList(){
	dataListCtrl.DeleteAllItems();
	//���ɱ�ͷ
	dataListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < columnPropertyList.GetSize(); i++){
		ColumnProperty* columnProperty = columnPropertyList[i];
		dataListCtrl.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//������
	}
	//�������
	UpdateDataList();

}
void CDataFrameDisplayDlg::UpdateDataList(){
	//��������
	//g_clsMutex->Lock();
	CString rawDataStr = Converter::HexToString(dataFrame->rawData, dataFrame->rawDataLength);
	rawFrameDisplay.SetWindowTextW(rawDataStr);

	dataListCtrl.DeleteAllItems();
	int row = 0;
	InsertData(&(dataFrame->billingDayHeat), row++);
	InsertData(&(dataFrame->currentHeat), row++);
	InsertData(&(dataFrame->heatPower), row++);
	InsertData(&(dataFrame->flowRate), row++);
	InsertData(&(dataFrame->billingDayCapacity), row++);
	InsertData(&(dataFrame->temperatureIn), row++);
	InsertData(&(dataFrame->temperatureOut), row++);
	InsertData(&(dataFrame->totalWorkHours), row++);

	InsertData(&(dataFrame->currentTime), row++);

	InsertData(&(dataFrame->statusData), row++);



	//g_clsMutex->Unlock();

}

void CDataFrameDisplayDlg::InsertData(DataItem* item,int row){
	dataListCtrl.InsertItem(row, item->name);//������
	int currentColumn = 1;
	dataListCtrl.SetItemText(row, currentColumn++, item->ToString());//������

}