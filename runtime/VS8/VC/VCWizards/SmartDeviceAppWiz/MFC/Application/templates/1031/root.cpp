// [!output APP_IMPL] : Definiert das Klassenverhalten für die Anwendung.
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
	// Standarddruckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, &[!output APP_BASE_CLASS]::OnFilePrintSetup)
[!endif]
[!if DOCVIEW]
	ON_COMMAND(ID_FILE_NEW, &[!output APP_BASE_CLASS]::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &[!output APP_BASE_CLASS]::OnFileOpen)
[!endif]
END_MESSAGE_MAP()



// [!output APP_CLASS]-Erstellung
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS]::[!output APP_CLASS](LPCTSTR lpszHelpName)
	: [!output APP_BASE_CLASS](lpszHelpName)
[!else]
[!output APP_CLASS]::[!output APP_CLASS]()
	: [!output APP_BASE_CLASS]()
[!endif]
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}

[!if ATL_SUPPORT]
// ATL-Modulobjekt
CComModule _Module;
[!endif]

// Das einzige [!output APP_CLASS]-Objekt
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS] theApp(_T("HELP_NAME"));
[!else]
[!output APP_CLASS] theApp;
[!endif]

// [!output APP_CLASS]-Initialisierung

BOOL [!output APP_CLASS]::InitInstance()
{
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // SHInitExtraControls sollte einmal während der Anwendungsinitialisierung aufgerufen werden, um alle
    // Windows Mobile-spezifischen Steuerelemente wie CAPEDIT und SIPPREF zu initialisieren.
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // SHInitExtraControls sollte einmal während der Anwendungsinitialisierung aufgerufen werden, um alle
    // Windows Mobile-spezifischen Steuerelemente wie CAPEDIT und SIPPREF zu initialisieren.
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
	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));
[!if !DOCVIEW]
	// Dieser Code erstellt ein neues Rahmenfensterobjekt und legt dieses
	// als Hauptfensterobjekt der Anwendung fest, um das Hauptfenster zu erstellen.
	CMainFrame* pFrame = new [!output MAIN_FRAME_CLASS];
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	// Rahmen mit Ressourcen erstellen und laden
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// Wenn keine Datei in der Dokumentliste vorhanden ist, erstellen wir eine neue.
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Haupt-SDI-Rahmenfenster
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// Wenn keine Datei in der Dokumentliste vorhanden ist, erstellen wir eine neue.
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Haupt-SDI-Rahmenfenster
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Haupt-SDI-Rahmenfenster
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
	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle. Gibt FALSE zurück, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!else]
#ifndef WIN32_PLATFORM_PSPC
	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle. Gibt FALSE zurück, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#endif // !WIN32_PLATFORM_PSPC
[!endif]
[!else]
	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle. Gibt FALSE zurück, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!endif]

[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if !POCKETPC2003_UI_MODEL]
	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
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
	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
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
	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
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
// [!output APP_CLASS]-Meldungshandler

[!if DOCVIEW]
void [!output APP_CLASS]::OnFileNew() 
{
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	CString strUntitled;
	CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, m_pMainWnd);
	if (pFrameWnd != NULL)
	{
		//TODO: Alle dateispezifischen Daten im Arbeitsspeicher schließen
		// und Löschen. Danach führen Sie die folgende if-Anweisung aus,
		// um den Titel des Rahmenfensters zurückzusetzen.
		if (strUntitled.LoadString(AFX_IDS_UNTITLED))
			pFrameWnd->UpdateFrameTitleForDocument(strUntitled);
		else
			pFrameWnd->UpdateFrameTitleForDocument(NULL);
	}
[!else]
	[!output MAIN_FRAME_CLASS]* pFrame = STATIC_DOWNCAST([!output MAIN_FRAME_CLASS], m_pMainWnd);
	// Neues untergeordnetes MDI-Fenster erstellen
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_[!output SAFE_DOC_TYPE_NAME]TYPE, m_hMDIMenu, m_hMDIAccel);
[!endif]
}
[!endif]
[!endif]

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
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

	return TRUE;	// TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
			// AUSNAHME: OCX-Eigenschaftenseiten müssen FALSE zurückgeben
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

// Anwendungsbefehl zum Ausführen des Dialogfelds
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
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
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

	return TRUE;	// TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
			// AUSNAHME: OCX-Eigenschaftenseiten müssen FALSE zurückgeben
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

// Anwendungsbefehl zum Ausführen des Dialogfelds
void [!output APP_CLASS]::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]