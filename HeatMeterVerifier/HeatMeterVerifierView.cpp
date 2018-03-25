
// HeatMeterVerifierView.cpp : CHeatMeterVerifierView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CHeatMeterVerifierView 构造/析构

CHeatMeterVerifierView::CHeatMeterVerifierView()
{
	m_pSelection = NULL;
	// TODO:  在此处添加构造代码

}

CHeatMeterVerifierView::~CHeatMeterVerifierView()
{
}

BOOL CHeatMeterVerifierView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHeatMeterVerifierView 绘制

void CHeatMeterVerifierView::OnDraw(CDC* pDC)
{
	if (!pDC)
		return;

	CHeatMeterVerifierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	// TODO:  同时绘制文档中的所有 OLE 项

	// 在任意位置绘制选定项。  一旦
	//  实现了真正的绘制代码后，应移除此代码。  此位置
	//  与 CHeatMeterVerifierCntrItem 返回的矩形完全对应，
	//  从而产生就地编辑的效果。

	// TODO:  最终绘制代码完成后移除此代码。
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


	// TODO:  写入最终选择模式代码之后移除此代码
	m_pSelection = NULL;    // 初始化所选内容

}


// CHeatMeterVerifierView 打印

BOOL CHeatMeterVerifierView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHeatMeterVerifierView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CHeatMeterVerifierView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}

void CHeatMeterVerifierView::OnDestroy()
{
	// 析构时停用此项；这在
	// 使用拆分视图时非常重要 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CView::OnDestroy();
}



// OLE 客户端支持和命令

BOOL CHeatMeterVerifierView::IsSelected(const CObject* pDocItem) const
{
	// 如果所选内容只包括
	//  CHeatMeterVerifierCntrItem 对象，则以下实现就足够了。若要处理其他选择机制，
	//  则应替换此处的实现

	// TODO:  实现对所选 OLE 客户端项进行测试的函数

	return pDocItem == m_pSelection;
}

void CHeatMeterVerifierView::OnInsertObject()
{
	// 调用标准的“插入对象”对话框以获得有关
	//  新 CHeatMeterVerifierCntrItem 对象的信息
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CHeatMeterVerifierCntrItem* pItem = NULL;
	TRY
	{
		// 创建与此文档相连接的新项
		CHeatMeterVerifierDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CHeatMeterVerifierCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// 通过对话框数据初始化该项
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // 任何异常都将导致该结果
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);
		// 作为任意用户界面设计，这会将所选内容
		//  设置为插入的最后一项

		// TODO:  重新实现所选内容，使其适合于您的应用程序
		m_pSelection = pItem;   // 将所选内容设置为插入的最后一项
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

// 以下命令处理程序提供了标准键盘
//  用户界面以取消就地编辑会话。  此处，
//  容器(而不是服务器)将导致停用
void CHeatMeterVerifierView::OnCancelEditCntr()
{
	// 关闭此视图中的任何就地活动项。
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// 在就地编辑一个对象时，容器需要对 OnSetFocus 和 OnSize 
//  进行特殊处理
void CHeatMeterVerifierView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// 如果该项处于同一视图中，则需要将焦点设置到该项
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // 不要调用基类
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
	//默认情况下，要求活动文档使用 IOleCommandTarget 打印自身。
	//如果不需要此行为，
	//请移除对 COleDocObjectItem::DoDefaultPrinting 调用。
	//如果由于某种原因调用失败，则将尝试使用 IPrint 接口打印
	//该文档对象。
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL); 
	if (S_OK == COleDocObjectItem::DoDefaultPrinting(this, &printInfo))
		return;
	
	CView::OnFilePrint();

}



// CHeatMeterVerifierView 诊断

#ifdef _DEBUG
void CHeatMeterVerifierView::AssertValid() const
{
	CView::AssertValid();
}

void CHeatMeterVerifierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHeatMeterVerifierDoc* CHeatMeterVerifierView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHeatMeterVerifierDoc)));
	return (CHeatMeterVerifierDoc*)m_pDocument;
}
#endif //_DEBUG


// CHeatMeterVerifierView 消息处理程序
