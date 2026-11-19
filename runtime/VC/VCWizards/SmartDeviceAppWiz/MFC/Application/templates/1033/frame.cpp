// [!output MAIN_FRAME_IMPL] : implementation of the [!output MAIN_FRAME_CLASS] class
//

#include "stdafx.h"
#include "[!output APP_HEADER]"

#include "[!output MAIN_FRAME_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
#define TOOLBAR_HEIGHT 24
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
#define TOOLBAR_HEIGHT 24
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]

[!if HELPSTYLE_WINHELP]
const DWORD dwAdornmentFlags = CMDBAR_HELP; // help and exit buttons
[!else]
const DWORD dwAdornmentFlags = 0; // exit button
[!endif]

// [!output MAIN_FRAME_CLASS]

[!if DOCVIEW]
IMPLEMENT_DYNCREATE([!output MAIN_FRAME_CLASS], [!output MAIN_FRAME_BASE_CLASS])
[!else]
IMPLEMENT_DYNAMIC([!output MAIN_FRAME_CLASS], [!output MAIN_FRAME_BASE_CLASS])
[!endif]

BEGIN_MESSAGE_MAP([!output MAIN_FRAME_CLASS], [!output MAIN_FRAME_BASE_CLASS])
	ON_WM_CREATE()
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
[!if !DOCVIEW]
	ON_WM_SETFOCUS()
[!endif] 
[!endif] 
[!if HELPSTYLE_WINHELP]
	ON_WM_HELPINFO()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, &[!output MAIN_FRAME_BASE_CLASS]::OnHelpFinder)
	ON_COMMAND(ID_HELP, &[!output MAIN_FRAME_BASE_CLASS]::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &[!output MAIN_FRAME_BASE_CLASS]::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &[!output MAIN_FRAME_BASE_CLASS]::OnHelpFinder)
[!endif]
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	ON_NOTIFY(DLN_DOCLIST_CREATE, AFX_ID_DOCLIST, &[!output MAIN_FRAME_CLASS]::OnCreateDocList)
	ON_NOTIFY(DLN_DOCLIST_DESTROY, AFX_ID_DOCLIST, &[!output MAIN_FRAME_CLASS]::OnDestroyDocList)
	ON_WM_DESTROY()
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	ON_NOTIFY(DLN_DOCLIST_CREATE, AFX_ID_DOCLIST, &[!output MAIN_FRAME_CLASS]::OnCreateDocList)
	ON_NOTIFY(DLN_DOCLIST_DESTROY, AFX_ID_DOCLIST, &[!output MAIN_FRAME_CLASS]::OnDestroyDocList)
	ON_WM_DESTROY()
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!if APP_TYPE_SDI]
[!if LIST_VIEW]
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &[!output MAIN_FRAME_CLASS]::OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, &[!output MAIN_FRAME_CLASS]::OnViewStyle)
[!endif]
[!endif]
[!endif]
END_MESSAGE_MAP()
[!if STATUS_BAR]

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
[!if DO_KANA]
	ID_INDICATOR_KANA,
[!endif]
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
[!endif]


// [!output MAIN_FRAME_CLASS] construction/destruction

[!output MAIN_FRAME_CLASS]::[!output MAIN_FRAME_CLASS]()
{
	// TODO: add member initialization code here
}

[!output MAIN_FRAME_CLASS]::~[!output MAIN_FRAME_CLASS]()
{
}

int [!output MAIN_FRAME_CLASS]::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ([!output MAIN_FRAME_BASE_CLASS]::OnCreate(lpCreateStruct) == -1)
		return -1;
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
[!if !DOCVIEW]
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
[!endif]
[!endif]

[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]

[!if CMD_BAR_MENUONLY]
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
	CWnd* pWnd = CWnd::FromHandlePermanent(m_wndCommandBar.m_hWnd);

	RECT rect, rectDesktop;
	pWnd->GetWindowRect(&rect);
	pWnd->GetDesktopWindow()->GetWindowRect(&rectDesktop);

	int cx = rectDesktop.right - rectDesktop.left;
	int cy = (rectDesktop.bottom - rectDesktop.top) - (rect.bottom - rect.top);
	this->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
[!if !DOCVIEW]
	this->SetForegroundWindow();
[!endif]
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
	CWnd* pWnd = CWnd::FromHandlePermanent(m_wndCommandBar.m_hWnd);

	RECT rect, rectDesktop;
	pWnd->GetWindowRect(&rect);
	pWnd->GetDesktopWindow()->GetWindowRect(&rectDesktop);

	int cx = rectDesktop.right - rectDesktop.left;
	int cy = (rectDesktop.bottom - rectDesktop.top) - (rect.bottom - rect.top);
	this->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
[!if !DOCVIEW]
	this->SetForegroundWindow();
[!endif]
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]
[!else]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags) ||
	    !m_wndCommandBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);

	CWnd* pWnd = CWnd::FromHandlePermanent(m_wndCommandBar.m_hWnd);

	RECT rect, rectDesktop;
	pWnd->GetWindowRect(&rect);
	pWnd->GetDesktopWindow()->GetWindowRect(&rectDesktop);

	int cx = rectDesktop.right - rectDesktop.left;
	int cy = (rectDesktop.bottom - rectDesktop.top) - (rect.bottom - rect.top);
	this->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
[!if !DOCVIEW]
	this->SetForegroundWindow();
[!endif]
[!endif]
[!if STANDARDSHELL_UI_MODEL || AYGSHELL_UI_MODEL]
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.InsertSeparator() ||
	    !m_wndCommandBar.LoadToolBar(IDR_MAINFRAME)  ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}
	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);
[!endif]
[!else] 
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags) ||
	    !m_wndCommandBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);

	CWnd* pWnd = CWnd::FromHandlePermanent(m_wndCommandBar.m_hWnd);

	RECT rect, rectDesktop;
	pWnd->GetWindowRect(&rect);
	pWnd->GetDesktopWindow()->GetWindowRect(&rectDesktop);

	int cx = rectDesktop.right - rectDesktop.left;
	int cy = (rectDesktop.bottom - rectDesktop.top) - (rect.bottom - rect.top);
	this->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
[!if !DOCVIEW]
	this->SetForegroundWindow();
[!endif]
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!if STANDARDSHELL_UI_MODEL || AYGSHELL_UI_MODEL]
#if !defined(WIN32_PLATFORM_PSPC) && !defined(WIN32_PLATFORM_WFSP)
	if (!m_wndCommandBar.Create(this) ||
	    !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	    !m_wndCommandBar.InsertSeparator() ||
	    !m_wndCommandBar.LoadToolBar(IDR_MAINFRAME)  ||
	    !m_wndCommandBar.AddAdornments(dwAdornmentFlags))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}
	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);
#endif // !WIN32_PLATFORM_PSPC && !WIN32_PLATFORM_WFSP
[!endif]
[!endif]
[!endif]
[!endif]
[!if STATUS_BAR]
	if (!m_wndStatusBar.Create(this) ||
	    !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
[!endif]

	return 0;
}

BOOL [!output MAIN_FRAME_CLASS]::PreCreateWindow(CREATESTRUCT& cs)
{
	if (![!output MAIN_FRAME_BASE_CLASS]::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

[!if !MAIN_FRAME_DEFAULT_STYLES]
[!endif]
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
[!if !DOCVIEW]

	cs.lpszClass = AfxRegisterWndClass(0);
[!endif] 
[!endif]
	return TRUE;
}

[!if HELPSTYLE_WINHELP]
void [!output MAIN_FRAME_CLASS]::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: implement help here
	MessageBox(_T("Help"));
}
[!endif]

[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
void [!output MAIN_FRAME_CLASS]::OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result)
{
	CDocList* pDocList = (CDocList*)FromHandle(pNotifyStruct->nmhdr.hwndFrom);
	ASSERT_KINDOF(CDocList, pDocList);

	m_wndDocListCommandBar.m_pWndDocList = pDocList;

	m_wndDocListCommandBar.Create(this);
	m_wndDocListCommandBar.InsertMenuBar(IDR_DOCLIST);

	CFrameWnd::OnCreateDocList(pNotifyStruct, result);	
}

void [!output MAIN_FRAME_CLASS]::OnDestroyDocList(DLNHDR* pNotifyStruct, LRESULT* result)
{
	m_wndDocListCommandBar.DestroyWindow();

	CFrameWnd::OnDestroyDocList(pNotifyStruct, result);	
}

void [!output MAIN_FRAME_CLASS]::OnDestroy()
{
	CFrameWnd::OnDestroy();
}
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
void [!output MAIN_FRAME_CLASS]::OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result)
{
	CDocList* pDocList = (CDocList*)FromHandle(pNotifyStruct->nmhdr.hwndFrom);
	ASSERT_KINDOF(CDocList, pDocList);

	m_wndDocListCommandBar.m_pWndDocList = pDocList;

	m_wndDocListCommandBar.Create(this);
	m_wndDocListCommandBar.InsertMenuBar(IDR_DOCLIST);

	CFrameWnd::OnCreateDocList(pNotifyStruct, result);	
}

void [!output MAIN_FRAME_CLASS]::OnDestroyDocList(DLNHDR* pNotifyStruct, LRESULT* result)
{
	m_wndDocListCommandBar.DestroyWindow();

	CFrameWnd::OnDestroyDocList(pNotifyStruct, result);	
}

void [!output MAIN_FRAME_CLASS]::OnDestroy()
{
	CFrameWnd::OnDestroy();
}
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif]

// [!output MAIN_FRAME_CLASS] diagnostics

#ifdef _DEBUG
void [!output MAIN_FRAME_CLASS]::AssertValid() const
{
	[!output MAIN_FRAME_BASE_CLASS]::AssertValid();
}
#endif //_DEBUG

// [!output MAIN_FRAME_CLASS] message handlers

[!if !DOCVIEW]
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
void [!output MAIN_FRAME_CLASS]::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL [!output MAIN_FRAME_CLASS]::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
[!endif] 
[!endif]


