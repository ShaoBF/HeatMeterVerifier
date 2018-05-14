#pragma once
#include <afxhtml.h>
#include <atlsafe.h>
////////////////////////////////////////////////////////////////
// 该结构在命令映射中定义一个入口，这个映射将文本串映射到命令IDs，
// 如果命令映射中有一个映射到 ID_APP_ABOUT 的入口 “about”，并且
// HTML 有一个链接锚 ,那么单击该链接时将执行
struct HTMLCMDMAP
{
	LPCTSTR name;
	int nID;
};// command name used in "app:name" HREF in
//这个类将 CHtmlView 转换为普通的能在对话框和框架中使用的控制
class CHtmlCtrl : public CHtmlView
{
	DECLARE_DYNCREATE(CHtmlCtrl)
public:
	CHtmlCtrl() :m_bHideMenu(FALSE), m_cmdmap(NULL){}; // protected constructor used by dynamic creation
	virtual ~CHtmlCtrl(){};
	HTMLCMDMAP* m_cmdmap;// command map
	BOOL m_bHideMenu;// hide context menu
	// get/set HideContextMenu property
	BOOL GetHideContextMenu(){ return m_bHideMenu; }
	void SetHideContextMenu(BOOL val){ m_bHideMenu = val; }
	// Set doc contents from string
	HRESULT SetHTML(LPCTSTR strHTML);
	// set command map
	void SetCmdMap(HTMLCMDMAP* val){ m_cmdmap = val; }
	// create control in same place as static control
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);
	// create control from scratch
	BOOL Create(const RECT& rc, CWnd* pParent, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE, CCreateContext* pContext = NULL)
	{
		return CHtmlView::Create(NULL, NULL, dwStyle, rc, pParent, nID, pContext);
	}
	// 重写该函数可以截获子窗口消息，从而禁用上下文菜单。
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 通常，CHtmlView 自己是在 PostNcDestroy 销毁的，但对于一个界面控制来说
	// 我们不想那样做，因为控制一般都是作为另一个窗口对象的成员实现的。
	virtual void PostNcDestroy(){}
	// 重写以便旁路掉对 MFC doc/view 框架的依赖，CHtmView 仅仅在这里依附于框架。
	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	// 重写以便截获 "app:" 伪协议
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

	// 你可以重写处理 "app:" 命令的代码。注意只是在不使用命令映射机制时才需要重写
	virtual void OnAppCmd(LPCTSTR lpszCmd);
	DECLARE_MESSAGE_MAP();
};
