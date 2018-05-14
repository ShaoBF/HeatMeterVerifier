
// ChildFrm.h : CChildFrame 类的接口
//

#pragma once

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 特性
protected:
	CSplitterWnd m_wndSplitter;
public:

// 操作
public:

// 重写
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateMeterSerialSettings(CCmdUI *pCmdUI);
	afx_msg void OnMeterSerialSettings();
	afx_msg void OnStartMeterRead();
	afx_msg void OnHtmlTest();
};
