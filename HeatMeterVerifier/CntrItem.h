
// CntrItem.h : CHeatMeterVerifierCntrItem ��Ľӿ�
//

#pragma once

class CHeatMeterVerifierDoc;
class CHeatMeterVerifierView;

class CHeatMeterVerifierCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CHeatMeterVerifierCntrItem)

// ���캯��
public:
	CHeatMeterVerifierCntrItem(CHeatMeterVerifierDoc* pContainer = NULL);
		// ע��:  ���� pContainer Ϊ NULL ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����  OLE ��ͨ������
		//  �� NULL �ĵ�ָ�빹���

// ����
public:
	CHeatMeterVerifierDoc* GetDocument()
		{ return reinterpret_cast<CHeatMeterVerifierDoc*>(COleClientItem::GetDocument()); }
	CHeatMeterVerifierView* GetActiveView()
		{ return reinterpret_cast<CHeatMeterVerifierView*>(COleClientItem::GetActiveView()); }

public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();

protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);

// ʵ��
public:
	~CHeatMeterVerifierCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

