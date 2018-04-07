// DataFrameDisplayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "DataFrameDisplayDlg.h"
#include "afxdialogex.h"
#include "MyVector.cpp"
#include "Converter.h"


// CDataFrameDisplayDlg 对话框

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


// CDataFrameDisplayDlg 消息处理程序

BOOL CDataFrameDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化所有UI控件
	InitUIs();

	//绘制数据项展示表格
	DisplayDataList();

	//详细信息列表形式显示读数

	return true;
}


void CDataFrameDisplayDlg::SetDataFrame(DataFrame* frame){
	this->dataFrame = frame;
}

//初始化所有UI控件
void CDataFrameDisplayDlg::InitUIs(){
	//columnPropertyList = new ColumnProperty*[columnCount];
	int current = 0;
	ColumnProperty* column;
	column = new ColumnProperty(L"数据项目", LVCFMT_LEFT, 100);
	columnPropertyList.Add(column);
	column = new ColumnProperty(L"数据值", LVCFMT_LEFT, 200);
	columnPropertyList.Add(column);
	//column = new ColumnProperty(L"单位", LVCFMT_LEFT, 60);
	//columnPropertyList.Add(column);

}

//绘制数据项展示表格
void CDataFrameDisplayDlg::DisplayDataList(){
	dataListCtrl.DeleteAllItems();
	//生成表头
	dataListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < columnPropertyList.GetSize(); i++){
		ColumnProperty* columnProperty = columnPropertyList[i];
		dataListCtrl.InsertColumn(i, columnProperty->name, columnProperty->format, columnProperty->width, i);//设置列
	}
	//填充数据
	UpdateDataList();

}
void CDataFrameDisplayDlg::UpdateDataList(){
	//重填数据
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
	dataListCtrl.InsertItem(row, item->name);//插入行
	int currentColumn = 1;
	dataListCtrl.SetItemText(row, currentColumn++, item->ToString());//插入行

}