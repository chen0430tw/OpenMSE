// [!output MAIN_FRAME_HEADER] : [!output MAIN_FRAME_CLASS] 클래스의 인터페이스
//


#pragma once
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
[!if !DOCVIEW]

#include "[!output WND_VIEW_HEADER]"
[!endif]
[!endif] 



class [!output MAIN_FRAME_CLASS] : public [!output MAIN_FRAME_BASE_CLASS]
{
[!if !DOCVIEW]
public:
	[!output MAIN_FRAME_CLASS]();
protected: 
	DECLARE_DYNAMIC([!output MAIN_FRAME_CLASS])
[!else]
protected: // serialization에서만 만들어집니다.
	[!output MAIN_FRAME_CLASS]();
	DECLARE_DYNCREATE([!output MAIN_FRAME_CLASS])
[!endif]

public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
[!if !DOCVIEW]
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
[!endif]
[!endif]

// 구현입니다.
public:
	virtual ~[!output MAIN_FRAME_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
[!if CMD_BAR_MENUONLY || CMD_BAR_MENUNBUTTON]
	CCommandBar m_wndCommandBar;
[!if STATUS_BAR]
	CStatusBar  m_wndStatusBar;
[!endif]
[!endif]
[!if !DOCVIEW]
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	[!output WND_VIEW_CLASS]    m_wndView;
[!endif]
[!else]
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	CDocListCommandBar m_wndDocListCommandBar;
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	CDocListCommandBar m_wndDocListCommandBar;
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif]
[!endif]

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
[!if HELPSTYLE_WINHELP]
	afx_msg int OnHelpInfo(HELPINFO* pHelpInfo);
[!endif]
[!if !DOCVIEW]
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	afx_msg void OnSetFocus(CWnd *pOldWnd);
[!endif]
[!else]
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	afx_msg void OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnDestroyDocList(DLNHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnDestroy();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	afx_msg void OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnDestroyDocList(DLNHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnDestroy();
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif]
[!endif]
	
	DECLARE_MESSAGE_MAP()
};


