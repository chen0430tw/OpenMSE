// [!output APP_IMPL] : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "[!output APP_HEADER]"
#include "[!output MAIN_FRAME_HEADER]"

[!if DOCVIEW]
#include "[!output DOC_HEADER]"
#include "[!output VIEW_HEADER]"
[!endif]

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// [!output APP_CLASS]

BEGIN_MESSAGE_MAP([!output APP_CLASS], [!output APP_BASE_CLASS])
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
	ON_COMMAND(ID_APP_ABOUT, &[!output APP_CLASS]::OnAppAbout)
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
	ON_COMMAND(ID_APP_ABOUT, &[!output APP_CLASS]::OnAppAbout)
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]
[!if PRINTING && DOCVIEW]
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &[!output APP_BASE_CLASS]::OnFilePrintSetup)
[!endif]
[!if DOCVIEW]
	ON_COMMAND(ID_FILE_NEW, &[!output APP_BASE_CLASS]::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &[!output APP_BASE_CLASS]::OnFileOpen)
[!endif]
END_MESSAGE_MAP()



// [!output APP_CLASS] construction
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS]::[!output APP_CLASS](LPCTSTR lpszHelpName)
	: [!output APP_BASE_CLASS](lpszHelpName)
[!else]
[!output APP_CLASS]::[!output APP_CLASS]()
	: [!output APP_BASE_CLASS]()
[!endif]
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

[!if ATL_SUPPORT]
// ATL Module object
CComModule _Module;
[!endif]

// The one and only [!output APP_CLASS] object
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS] theApp(_T("HELP_NAME"));
[!else]
[!output APP_CLASS] theApp;
[!endif]

// [!output APP_CLASS] initialization

BOOL [!output APP_CLASS]::InitInstance()
{
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP
[!endif]
[!endif]

[!if SOCKETS]
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

[!endif]
[!if ACTIVEX_CONTROLS]
	AfxEnableControlContainer();
[!endif]
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
[!if !DOCVIEW]
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new [!output MAIN_FRAME_CLASS];
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
[!endif]
[!else]
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	CDocListDocTemplate* pDocTemplate;
	pDocTemplate = new CDocListDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS([!output DOC_CLASS]),
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // main SDI frame window
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// if there is no file in the doclist, we will create a new one.
	CDocList* pDocList = pDocTemplate->m_pWndDocList;	
	if (pDocList->GetItemCount() == 0) 
	{
		pDocList->OnClose();
		OnFileNew();
	}
[!else]
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS([!output DOC_CLASS]),
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // main SDI frame window
		RUNTIME_CLASS([!output VIEW_CLASS]));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
[!endif]
[!else]
#ifdef WIN32_PLATFORM_PSPC
	CDocListDocTemplate* pDocTemplate;
	pDocTemplate = new CDocListDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS([!output DOC_CLASS]),
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // main SDI frame window
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// if there is no file in the doclist, we will create a new one.
	CDocList* pDocList = pDocTemplate->m_pWndDocList;	
	if (pDocList->GetItemCount() == 0) 
	{
		pDocList->OnClose();
		OnFileNew();
	}
#else
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS([!output DOC_CLASS]),
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // main SDI frame window
		RUNTIME_CLASS([!output VIEW_CLASS]));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!else]
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS([!output DOC_CLASS]),
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // main SDI frame window
		RUNTIME_CLASS([!output VIEW_CLASS]));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
[!endif]
[!endif]

[!if DOCVIEW]
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if !POCKETPC2003_UI_MODEL]
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!else]
#ifndef WIN32_PLATFORM_PSPC
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#endif // !WIN32_PLATFORM_PSPC
[!endif]
[!else]
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!endif]

[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if !POCKETPC2003_UI_MODEL]
	// The one and only window has been initialized, so show and update it
[!if DOCVIEW]
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
[!else]
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
[!endif]
[!endif]
[!else]
#ifndef WIN32_PLATFORM_PSPC
	// The one and only window has been initialized, so show and update it
[!if DOCVIEW]
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
[!else]
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
[!endif]
#endif // !WIN32_PLATFORM_PSPC
[!endif]
[!else]
[!if APP_TYPE_SDI]
	// The one and only window has been initialized, so show and update it
[!if DOCVIEW]
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
[!else]
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
[!endif]
[!endif]
[!endif]
	return TRUE;
}

[!if !DOCVIEW]
// [!output APP_CLASS] message handlers

[!if DOCVIEW]
void [!output APP_CLASS]::OnFileNew() 
{
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	CString strUntitled;
	CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, m_pMainWnd);
	if (pFrameWnd != NULL)
	{
		//TODO: close and destroy any file-specific data you've got
		// in memory. Once done, execute the following if statement
		// to reset the title of the frame window
		if (strUntitled.LoadString(AFX_IDS_UNTITLED))
			pFrameWnd->UpdateFrameTitleForDocument(strUntitled);
		else
			pFrameWnd->UpdateFrameTitleForDocument(NULL);
	}
[!else]
	[!output MAIN_FRAME_CLASS]* pFrame = STATIC_DOWNCAST([!output MAIN_FRAME_CLASS], m_pMainWnd);
	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_[!output SAFE_DOC_TYPE_NAME]TYPE, m_hMDIMenu, m_hMDIAccel);
[!endif]
}
[!endif]
[!endif]

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
[!if POCKETPC2003_UI_MODEL]
#ifdef _DEVICE_RESOLUTION_AWARE
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
[!endif]
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;	// return TRUE unless you set the focus to a control
			// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
[!if POCKETPC2003_UI_MODEL]
#ifdef _DEVICE_RESOLUTION_AWARE
	ON_WM_SIZE()
#endif
[!endif]
END_MESSAGE_MAP()

[!if POCKETPC2003_UI_MODEL]
#ifdef _DEVICE_RESOLUTION_AWARE
void CAboutDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
    	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
	}
}
#endif
[!endif]

// App command to run the dialog
void [!output APP_CLASS]::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
#ifdef _DEVICE_RESOLUTION_AWARE
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;	// return TRUE unless you set the focus to a control
			// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
#ifdef _DEVICE_RESOLUTION_AWARE
	ON_WM_SIZE()
#endif
END_MESSAGE_MAP()

#ifdef _DEVICE_RESOLUTION_AWARE
void CAboutDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
    	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
	}
}
#endif

// App command to run the dialog
void [!output APP_CLASS]::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]