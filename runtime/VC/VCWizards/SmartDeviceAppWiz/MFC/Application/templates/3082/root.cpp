// [!output APP_IMPL] : define los comportamientos de las clases para la aplicación.
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
	// Comando de configuración de impresión estándar
	ON_COMMAND(ID_FILE_PRINT_SETUP, &[!output APP_BASE_CLASS]::OnFilePrintSetup)
[!endif]
[!if DOCVIEW]
	ON_COMMAND(ID_FILE_NEW, &[!output APP_BASE_CLASS]::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &[!output APP_BASE_CLASS]::OnFileOpen)
[!endif]
END_MESSAGE_MAP()



// Construcción de [!output APP_CLASS]
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS]::[!output APP_CLASS](LPCTSTR lpszHelpName)
	: [!output APP_BASE_CLASS](lpszHelpName)
[!else]
[!output APP_CLASS]::[!output APP_CLASS]()
	: [!output APP_BASE_CLASS]()
[!endif]
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}

[!if ATL_SUPPORT]
// Objeto de módulo ATL
CComModule _Module;
[!endif]

// El único objeto [!output APP_CLASS]
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS] theApp(_T("HELP_NAME"));
[!else]
[!output APP_CLASS] theApp;
[!endif]

// Inicialización de [!output APP_CLASS]

BOOL [!output APP_CLASS]::InitInstance()
{
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // SHInitExtraControls se debe llamar una vez durante la inicialización de la aplicación para inicializar cualquiera
    // de los controles específicos de Windows Mobile como por ejemplo CAPEDIT SIPPREF.
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // SHInitExtraControls se debe llamar una vez durante la inicialización de la aplicación para inicializar cualquiera
    // de los controles específicos de Windows Mobile como por ejemplo CAPEDIT SIPPREF.
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
	// Inicialización estándar
	// Si no utiliza estas características y desea reducir el tamaño
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicialización específicas que no necesite
	
	// Cambie la clave del Registro en la que se almacena la configuración
	// TODO: debe modificar esta cadena para que contenga información correcta
	// como el nombre de su compañía u organización
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));
[!if !DOCVIEW]
	// Para crear la ventana principal, este código crea un nuevo objeto de ventana
	// de marco y, a continuación, lo establece como el objeto de ventana principal de la aplicación
	CMainFrame* pFrame = new [!output MAIN_FRAME_CLASS];
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	// Crear y cargar el marco con sus recursos
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Ventana de marco SDI principal
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// si no hay archivos en la lista de documentos, crearemos uno.
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Ventana de marco SDI principal
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Ventana de marco SDI principal
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// si no hay archivos en la lista de documentos, crearemos uno.
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Ventana de marco SDI principal
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // Ventana de marco SDI principal
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
	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enviar comandos especificados en la línea de comandos. Devolverá FALSE si
	// la aplicación se inició con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!else]
#ifndef WIN32_PLATFORM_PSPC
	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enviar comandos especificados en la línea de comandos. Devolverá FALSE si
	// la aplicación se inició con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#endif // !WIN32_PLATFORM_PSPC
[!endif]
[!else]
	// Analizar línea de comandos para comandos Shell estándar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enviar comandos especificados en la línea de comandos. Devolverá FALSE si
	// la aplicación se inició con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!endif]

[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if !POCKETPC2003_UI_MODEL]
	// Se ha inicializado la única ventana; mostrarla y actualizarla
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
	// Se ha inicializado la única ventana; mostrarla y actualizarla
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
	// Se ha inicializado la única ventana; mostrarla y actualizarla
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
// Controladores de mensaje de [!output APP_CLASS]

[!if DOCVIEW]
void [!output APP_CLASS]::OnFileNew() 
{
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	CString strUntitled;
	CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, m_pMainWnd);
	if (pFrameWnd != NULL)
	{
		//TODO: cerrar y destruir cualquier dato específico de archivo que existe en
		// memoria. A continuación, ejecutar la siguiente instrucción if
		// para restablecer el título de la ventana de marco
		if (strUntitled.LoadString(AFX_IDS_UNTITLED))
			pFrameWnd->UpdateFrameTitleForDocument(strUntitled);
		else
			pFrameWnd->UpdateFrameTitleForDocument(NULL);
	}
[!else]
	[!output MAIN_FRAME_CLASS]* pFrame = STATIC_DOWNCAST([!output MAIN_FRAME_CLASS], m_pMainWnd);
	// Crear una nueva ventana MDI secundaria
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_[!output SAFE_DOC_TYPE_NAME]TYPE, m_hMDIMenu, m_hMDIAccel);
[!endif]
}
[!endif]
[!endif]

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // compatibilidad con DDX/DDV

// Implementación
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

	return TRUE;	// Devuelve TRUE a menos que establezca el foco en un control
			// EXCEPCIÓN: las páginas de propiedades OCX deben devolver FALSE
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

// Comando de la aplicación para ejecutar el cuadro de diálogo
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
// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // compatibilidad con DDX/DDV

// Implementación
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

	return TRUE;	// Devuelve TRUE a menos que establezca el foco en un control
			// EXCEPCIÓN: las páginas de propiedades OCX deben devolver FALSE
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

// Comando de la aplicación para ejecutar el cuadro de diálogo
void [!output APP_CLASS]::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]