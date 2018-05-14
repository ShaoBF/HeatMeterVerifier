
// ChildFrm.cpp : CChildFrame ���ʵ��
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"
#include "TestHtmlDlg.h"
#include "SerialTestDlg.h"
#include "MeterChooserDlg.h"
#include "MeterWizard.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_UPDATE_COMMAND_UI(ID_METER_SERIAL_SETTINGS, &CChildFrame::OnUpdateMeterSerialSettings)
	ON_COMMAND(ID_METER_SERIAL_SETTINGS, &CChildFrame::OnMeterSerialSettings)
	ON_COMMAND(ID_START_METER_READ, &CChildFrame::OnStartMeterRead)
	ON_COMMAND(ID_HTML_TEST, &CChildFrame::OnHtmlTest)
END_MESSAGE_MAP()

// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,			// TODO:  ��������������
		CSize(10, 10),	// TODO:  ������С�����С
		pContext);
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame ���

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame ��Ϣ�������


void CChildFrame::OnUpdateMeterSerialSettings(CCmdUI *pCmdUI)
{
	if (pCmdUI->m_nID == ID_METER_SERIAL_SETTINGS){
		pCmdUI->Enable(TRUE);
	}
}


void CChildFrame::OnMeterSerialSettings()
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

extern CMeterWizard wizard;

void CChildFrame::OnStartMeterRead()
{	
	//wizard.ChooseMeters();
	wizard.ReadMeters();
}


void CChildFrame::OnHtmlTest()
{
	CHtmlReportDlg dlg;
	//dlg.Init();
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
