
// CntrItem.cpp : CHeatMeterVerifierCntrItem ���ʵ��
//

#include "stdafx.h"
#include "HeatMeterVerifier.h"

#include "HeatMeterVerifierDoc.h"
#include "HeatMeterVerifierView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHeatMeterVerifierCntrItem ��ʵ��

IMPLEMENT_SERIAL(CHeatMeterVerifierCntrItem, COleClientItem, 0)

CHeatMeterVerifierCntrItem::CHeatMeterVerifierCntrItem(CHeatMeterVerifierDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO:  �ڴ����һ���Թ������
}

CHeatMeterVerifierCntrItem::~CHeatMeterVerifierCntrItem()
{
	// TODO:  �ڴ˴�����������
}

void CHeatMeterVerifierCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// �ڱ༭ĳ��ʱ(�͵ر༭����ȫ�򿪽��б༭)��
	//  �����������״̬�ĸ���
	//  �������ݵĿ������״̬�ĸ��ķ��� OnChange ֪ͨ��

	// TODO:  ͨ������ UpdateAllViews ʹ������Ч
	//  (�����ʺ�����Ӧ�ó������ʾ)

	GetDocument()->UpdateAllViews(NULL);
		// ����ֻ����������ͼ/����ʾ
}

BOOL CHeatMeterVerifierCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// �͵ؼ����ڼ䣬���������� CHeatMeterVerifierCntrItem::OnChangeItemPosition
	//  �Ը��ľ͵ش��ڵ�λ�á�
	//  ��ͨ�������ڷ�����
	//  �ĵ��е����ݸ��Ķ����·�Χ�ı����
	//  �͵ص�����С������ġ�
	//
	// �˴���Ĭ�����õ��û��࣬�û��ཫ����
	//  COleClientItem::SetItemRects �Խ�����
	//  �ƶ����µ�λ�á�

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO:  ���¸���ľ���/��Χ���ܴ��ڵ��κλ���

	return TRUE;
}

BOOL CHeatMeterVerifierCntrItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow)
{
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, pFrameWnd);

	if (pMainFrame != NULL)
	{
		ASSERT_VALID(pMainFrame);
		return pMainFrame->OnShowPanes(bShow);
	}

	return FALSE;
}

void CHeatMeterVerifierCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// �͵ؼ����ڼ䣬������ CHeatMeterVerifierCntrItem::OnGetItemPosition
	//  �Ծ��������λ�á�  ͨ����
	//  �þ��λᷴӳ�뼤��������ͼ 
	//  �����ĵ�ǰλ�á�  ���� 
	//  CHeatMeterVerifierCntrItem::GetActiveView �ɵõ���ͼ��

	// TODO:  ���� rPosition �е���ȷ����(�����ر�ʾ)

	CSize size;
	rPosition.SetRectEmpty();
	if (GetExtent(&size, m_nDrawAspect))
	{
		CHeatMeterVerifierView* pView = GetActiveView();
		ASSERT_VALID(pView);
		if (!pView)
			return;
		CDC *pDC = pView->GetDC();
		ASSERT(pDC);
		if (!pDC)
			return;
		pDC->HIMETRICtoLP(&size);
		rPosition.SetRect(10, 10, size.cx + 10, size.cy + 10);
	}
	else
		rPosition.SetRect(10, 10, 210, 210);
}

void CHeatMeterVerifierCntrItem::OnActivate()
{
    // ÿ�����ֻ����һ���͵ؼ�����
    CHeatMeterVerifierView* pView = GetActiveView();
    ASSERT_VALID(pView);
	if (!pView)
		return;
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CHeatMeterVerifierCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CHeatMeterVerifierCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// ���ȵ��û����Զ�ȡ COleClientItem ���ݡ�
	// ��Ϊ���������ô� CHeatMeterVerifierCntrItem::GetDocument ���ص� m_pDocument ָ�룬
	//  ��������ȵ���
	//  ���� Serialize��
	COleClientItem::Serialize(ar);

	// ���ڴ洢/�����ض��� CHeatMeterVerifierCntrItem ������
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}


// CHeatMeterVerifierCntrItem ���

#ifdef _DEBUG
void CHeatMeterVerifierCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CHeatMeterVerifierCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

