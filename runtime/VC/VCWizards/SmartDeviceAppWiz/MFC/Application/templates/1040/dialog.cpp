// [!output DIALOG_IMPL] : file di implementazione
//

#include "stdafx.h"
#include "[!output APP_HEADER]"
#include "[!output DIALOG_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// finestra di dialogo [!output DIALOG_CLASS]

[!output DIALOG_CLASS]::[!output DIALOG_CLASS](CWnd* pParent /*=NULL*/)
	: [!output DIALOG_BASE_CLASS]([!output DIALOG_CLASS]::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void [!output DIALOG_CLASS]::DoDataExchange(CDataExchange* pDX)
{
	[!output DIALOG_BASE_CLASS]::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP([!output DIALOG_CLASS], [!output DIALOG_BASE_CLASS])
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
[!if HELPSTYLE_WINHELP]
	ON_WM_HELPINFO()
[!endif]
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// gestori di messaggi [!output DIALOG_CLASS]

BOOL [!output DIALOG_CLASS]::OnInitDialog()
{
	[!output DIALOG_BASE_CLASS]::OnInitDialog();

	// Impostare l'icona per questa finestra di dialogo. Il framework non esegue questa operazione automaticamente
	//  se la finestra principale dell'applicazione non è una finestra di dialogo.
	SetIcon(m_hIcon, TRUE);			// Impostare icona grande.
	SetIcon(m_hIcon, FALSE);		// Impostare icona piccola.

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("Impossibile creare CommandBar\n");
		return FALSE;      // impossibile creare
	}
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("Impossibile creare CommandBar\n");
		return FALSE;      // impossibile creare
	}
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]
	// TODO: aggiungere qui inizializzazione aggiuntiva.
	
	return TRUE;  // restituisce TRUE a meno che non venga impostato lo stato attivo su un controllo.
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void [!output DIALOG_CLASS]::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_DIALOG));
	}
}
#endif

[!if HELPSTYLE_WINHELP]
void [!output DIALOG_CLASS]::OnHelpInfo()
{
	// TODO: implementare qui la Guida.
	MessageBox(_T("?"));
}
[!endif]
