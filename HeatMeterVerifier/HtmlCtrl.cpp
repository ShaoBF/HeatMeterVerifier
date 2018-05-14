////////////////////////////////////////////////////////////////
// CHtmlCtrl 实现 -- 控件中的 web 浏览器，只要改写 CHtmlVie
// 你就可以摆脱框架 - 从而将此控制用于对话框和其它任何窗口。
//
// 特性s:
// - SetCmdMap 用于设置“app:command”链接的命令映射。.
// - SetHTML 用于将字符串转换为 HTML 文档。.

#include "StdAfx.h"
#include "HtmlCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// macro to declare a typedef for ATL smart poitners; eg SPIHTMLDocument2
#define DECLARE_SMARTPTR(ifacename) typedef CComQIPtr<ifacename> SP##ifacename;

// smart pointer to IHTMLDocument2
DECLARE_SMARTPTR(IHTMLDocument2)

// useful macro for checking HRESULTs
#define HRCHECK(x) hr = x; if (!SUCCEEDED(hr)) { \
 TRACE(_T("hr=%p\n"),hr);\
 return hr;\
}
IMPLEMENT_DYNAMIC(CHtmlCtrl, CHtmlView)
BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

//////////////////
// Create control in same position as an existing static control with given
// the same ID (could be any kind of control, really)
//
BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	// Get static control rect, convert to parent's client coords.
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
	return Create(rc, pParent, nID);
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
void CHtmlCtrl::OnDestroy()
{
	m_pBrowserApp = NULL; // will call Release
	CWnd::OnDestroy();  // bypass CView doc/frame stuff
}

////////////////
// Override to avoid CView stuff that assumes a frame.
//
int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	// bypass CView doc/frame stuff
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

// Return TRUE iff hwnd is internet explorer window.
inline BOOL IsIEWindow(HWND hwnd)
{
	static LPCSTR IEWNDCLASSNAME = "Internet Explorer_Server";
	char classname[32]; // always char, never TCHAR

	GetClassName(hwnd, (LPWSTR)classname, sizeof(classname));
	return strcmp(classname, IEWNDCLASSNAME) == 0;
}

//////////////////
// Override to trap "Internet Explorer_Server" window context menu messages.
//
BOOL CHtmlCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_bHideMenu)
	{
		switch (pMsg->message)
		{
		case WM_CONTEXTMENU:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			if (IsIEWindow(pMsg->hwnd))
			{
				if (pMsg->message == WM_RBUTTONUP)
					// let parent handle context menu
					GetParent()->SendMessage(WM_CONTEXTMENU, pMsg->wParam, pMsg->lParam);
				return TRUE; // eat it
			}
		}
	}
	return CHtmlView::PreTranslateMessage(pMsg);
}

//////////////////
// Override to pass "app:" links to virtual fn instead of browser.
//
void CHtmlCtrl::OnBeforeNavigate2(LPCTSTR lpszURL,
	DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
	LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	LPCTSTR APP_PROTOCOL = _T("app:");
	int len = (int)_tcslen(APP_PROTOCOL);

	if (_tcsnicmp(lpszURL, APP_PROTOCOL, len) == 0)
	{
		OnAppCmd(lpszURL + len);    // call virtual handler fn
		*pbCancel = TRUE;       // cancel navigation
	}
}

//////////////////
// Called when the browser attempts to navigate to "app:foo". Default handler
// searches command map for "foo" command, and sends parent a WM_COMMAND
// message with the ID if found. Call SetCmdMap to set the command map. Only
// override OnAppCmd if you want to do something more complex.
//
void CHtmlCtrl::OnAppCmd(LPCTSTR lpszCmd)
{
	if (m_cmdmap)
	{
		for (int i = 0; m_cmdmap[i].name; i++)
		{
			if (_tcsicmp(lpszCmd, m_cmdmap[i].name) == 0)
				// Use PostMessage to avoid problems with exit command. (Let
				// browser finish navigation before issuing command.)
				GetParent()->PostMessage(WM_COMMAND, m_cmdmap[i].nID);
		}
	}
}

//////////////////
// Set document contents from string
//
HRESULT CHtmlCtrl::SetHTML(LPCTSTR strHTML)
{
	HRESULT hr;
	// Get document object
	SPIHTMLDocument2 doc = GetHtmlDocument();
	// Create string as one-element BSTR safe array for IHTMLDocument2::write.
	CComSafeArray<VARIANT> sar;
	sar.Create(1, 0);
	sar[0] = CComBSTR(strHTML);
	// open doc and write
	LPDISPATCH lpdRet;
	HRCHECK(doc->open(CComBSTR("text/html"),
		CComVariant(CComBSTR("_self")),
		CComVariant(CComBSTR("")),
		CComVariant((bool)1),
		&lpdRet));

	HRCHECK(doc->write(sar)); // write contents to doc
	HRCHECK(doc->close());  // close
	lpdRet->Release();   // release IDispatch returned
	return S_OK;
}
//使用方法，在自己的对话框上拖进来一个static，然后修改一下ID为IDC_STATIC_HELP，然后在对话框的初始化函数，也就是OnInitDialog()中添加下面代码，注意下面的m_html是一个类成员变量，定义在头文件中 ，CHtmlCtrl m_html; ：

/*BOOL CHelpDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_html.SetHideContextMenu(TRUE);
	m_html.CreateFromStatic(IDC_STATIC_HELP, this);
	m_html.Navigate(m_strUrl);
	//CString m_Text = " 李民录 ";
	//m_html.SetHTML(m_strUrl);

	return TRUE;  // 除非将焦点设置到控件，否则返回TRUE
}*/