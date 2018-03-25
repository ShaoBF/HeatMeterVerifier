
// HeatMeterVerifierView.h : CHeatMeterVerifierView ��Ľӿ�
//

#pragma once

class CHeatMeterVerifierCntrItem;

class CHeatMeterVerifierView : public CView
{
protected: // �������л�����
	CHeatMeterVerifierView();
	DECLARE_DYNCREATE(CHeatMeterVerifierView)

// ����
public:
	CHeatMeterVerifierDoc* GetDocument() const;
	// m_pSelection ����ѡ���ݱ����ڵ�ǰ�� CHeatMeterVerifierCntrItem �С�
	// �������Ӧ�ó������ֳ�Ա����������
	//  ��ʾĳ��ѡ�������ڲ����� CHeatMeterVerifierCntrItem �Ķ�����
	//  ѡ����һ����������  �ṩ����ѡ��
	//  ���Ƶ�Ŀ��ֻ�ǰ���������

	// TODO:  ���ʺ�Ӧ�ó����ѡ������滻��ѡ�����
	CHeatMeterVerifierCntrItem* m_pSelection;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// ����֧��

// ʵ��
public:
	virtual ~CHeatMeterVerifierView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnFilePrint();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HeatMeterVerifierView.cpp �еĵ��԰汾
inline CHeatMeterVerifierDoc* CHeatMeterVerifierView::GetDocument() const
   { return reinterpret_cast<CHeatMeterVerifierDoc*>(m_pDocument); }
#endif

