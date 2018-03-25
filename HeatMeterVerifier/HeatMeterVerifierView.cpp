
// HeatMeterVerifierView.cpp : CHeatMeterVerifierView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "HeatMeterVerifier.h"
#endif

#include "HeatMeterVerifierDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "HeatMeterVerifierView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHeatMeterVerifierView

IMPLEMENT_DYNCREATE(CHeatMeterVerifierView, CView)

BEGIN_MESSAGE_MAP(CHeatMeterVerifierView, CView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, &CHeatMeterVerifierView::OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, &CHeatMeterVerifierView::OnCancelEditCntr)
	ON_COMMAND(ID_FILE_PRINT, &CHeatMeterVerifierView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHeatMeterVerifierView ����/����

CHeatMeterVerifierView::CHeatMeterVerifierView()
{
	m_pSelection = NULL;
	// TODO:  �ڴ˴���ӹ������

}

CHeatMeterVerifierView::~CHeatMeterVerifierView()
{
}

BOOL CHeatMeterVerifierView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CHeatMeterVerifierView ����

void CHeatMeterVerifierView::OnDraw(CDC* pDC)
{
	if (!pDC)
		return;

	CHeatMeterVerifierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	// TODO:  ͬʱ�����ĵ��е����� OLE ��

	// ������λ�û���ѡ���  һ��
	//  ʵ���������Ļ��ƴ����Ӧ�Ƴ��˴��롣  ��λ��
	//  �� CHeatMeterVerifierCntrItem ���صľ�����ȫ��Ӧ��
	//  �Ӷ������͵ر༭��Ч����

	// TODO:  ���ջ��ƴ�����ɺ��Ƴ��˴��롣
	if (m_pSelection != NULL)
	{
		CSize size;
		CRect rect(10, 10, 210, 210);
		
		if (m_pSelection->GetExtent(&size, m_pSelection->m_nDrawAspect))
		{
			pDC->HIMETRICtoLP(&size);
			rect.right = size.cx + 10;
			rect.bottom = size.cy + 10;
		}
		m_pSelection->Draw(pDC, rect);
	}
}

void CHeatMeterVerifierView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	// TODO:  д������ѡ��ģʽ����֮���Ƴ��˴���
	m_pSelection = NULL;    // ��ʼ����ѡ����

}


// CHeatMeterVerifierView ��ӡ

BOOL CHeatMeterVerifierView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CHeatMeterVerifierView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CHeatMeterVerifierView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void CHeatMeterVerifierView::OnDestroy()
{
	// ����ʱͣ�ô������
	// ʹ�ò����ͼʱ�ǳ���Ҫ 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CView::OnDestroy();
}



// OLE �ͻ���֧�ֺ�����

BOOL CHeatMeterVerifierView::IsSelected(const CObject* pDocItem) const
{
	// �����ѡ����ֻ����
	//  CHeatMeterVerifierCntrItem ����������ʵ�־��㹻�ˡ���Ҫ��������ѡ����ƣ�
	//  ��Ӧ�滻�˴���ʵ��

	// TODO:  ʵ�ֶ���ѡ OLE �ͻ�������в��Եĺ���

	return pDocItem == m_pSelection;
}

void CHeatMeterVerifierView::OnInsertObject()
{
	// ���ñ�׼�ġ�������󡱶Ի����Ի���й�
	//  �� CHeatMeterVerifierCntrItem �������Ϣ
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CHeatMeterVerifierCntrItem* pItem = NULL;
	TRY
	{
		// ��������ĵ������ӵ�����
		CHeatMeterVerifierDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CHeatMeterVerifierCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// ͨ���Ի������ݳ�ʼ������
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // �κ��쳣�������¸ý��
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);
		// ��Ϊ�����û�������ƣ���Ὣ��ѡ����
		//  ����Ϊ��������һ��

		// TODO:  ����ʵ����ѡ���ݣ�ʹ���ʺ�������Ӧ�ó���
		m_pSelection = pItem;   // ����ѡ��������Ϊ��������һ��
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// �������������ṩ�˱�׼����
//  �û�������ȡ���͵ر༭�Ự��  �˴���
//  ����(�����Ƿ�����)������ͣ��
void CHeatMeterVerifierView::OnCancelEditCntr()
{
	// �رմ���ͼ�е��κξ͵ػ�
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// �ھ͵ر༭һ������ʱ��������Ҫ�� OnSetFocus �� OnSize 
//  �������⴦��
void CHeatMeterVerifierView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// ��������ͬһ��ͼ�У�����Ҫ���������õ�����
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // ��Ҫ���û���
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void CHeatMeterVerifierView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

void CHeatMeterVerifierView::OnFilePrint()
{
	//Ĭ������£�Ҫ���ĵ�ʹ�� IOleCommandTarget ��ӡ����
	//�������Ҫ����Ϊ��
	//���Ƴ��� COleDocObjectItem::DoDefaultPrinting ���á�
	//�������ĳ��ԭ�����ʧ�ܣ��򽫳���ʹ�� IPrint �ӿڴ�ӡ
	//���ĵ�����
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL); 
	if (S_OK == COleDocObjectItem::DoDefaultPrinting(this, &printInfo))
		return;
	
	CView::OnFilePrint();

}



// CHeatMeterVerifierView ���

#ifdef _DEBUG
void CHeatMeterVerifierView::AssertValid() const
{
	CView::AssertValid();
}

void CHeatMeterVerifierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHeatMeterVerifierDoc* CHeatMeterVerifierView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHeatMeterVerifierDoc)));
	return (CHeatMeterVerifierDoc*)m_pDocument;
}
#endif //_DEBUG


// CHeatMeterVerifierView ��Ϣ�������
