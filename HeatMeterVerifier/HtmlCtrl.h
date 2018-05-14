#pragma once
#include <afxhtml.h>
#include <atlsafe.h>
////////////////////////////////////////////////////////////////
// �ýṹ������ӳ���ж���һ����ڣ����ӳ�佫�ı���ӳ�䵽����IDs��
// �������ӳ������һ��ӳ�䵽 ID_APP_ABOUT ����� ��about��������
// HTML ��һ������ê ,��ô����������ʱ��ִ��
struct HTMLCMDMAP
{
	LPCTSTR name;
	int nID;
};// command name used in "app:name" HREF in
//����ཫ CHtmlView ת��Ϊ��ͨ�����ڶԻ���Ϳ����ʹ�õĿ���
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
	// ��д�ú������Խػ��Ӵ�����Ϣ���Ӷ����������Ĳ˵���
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// ͨ����CHtmlView �Լ����� PostNcDestroy ���ٵģ�������һ�����������˵
	// ���ǲ�������������Ϊ����һ�㶼����Ϊ��һ�����ڶ���ĳ�Աʵ�ֵġ�
	virtual void PostNcDestroy(){}
	// ��д�Ա���·���� MFC doc/view ��ܵ�������CHtmView ���������������ڿ�ܡ�
	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	// ��д�Ա�ػ� "app:" αЭ��
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

	// �������д���� "app:" ����Ĵ��롣ע��ֻ���ڲ�ʹ������ӳ�����ʱ����Ҫ��д
	virtual void OnAppCmd(LPCTSTR lpszCmd);
	DECLARE_MESSAGE_MAP();
};
