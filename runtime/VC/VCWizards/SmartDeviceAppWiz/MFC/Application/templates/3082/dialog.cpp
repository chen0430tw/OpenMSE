// [!output DIALOG_IMPL]: archivo de implementación
//

#include "stdafx.h"
#include "[!output APP_HEADER]"
#include "[!output DIALOG_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cuadro de diálogo de [!output DIALOG_CLASS]

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


// Controladores de mensaje de [!output DIALOG_CLASS]

BOOL [!output DIALOG_CLASS]::OnInitDialog()
{
	[!output DIALOG_BASE_CLASS]::OnInitDialog();

	// Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("CommandBar no se pudo crear\n");
		return FALSE;      // No se pudo crear
	}
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("CommandBar no se pudo crear\n");
		return FALSE;      // No se pudo crear
	}
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]
	// TODO: agregar aquí inicialización adicional
	
	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
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
	// TODO: implementar la ayuda aquí
	MessageBox(_T("Ayuda"));
}
[!endif]
