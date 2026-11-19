// [!output DIALOG_HEADER] : file di intestazione
//

#pragma once

// finestra di dialogo [!output DIALOG_CLASS]
class [!output DIALOG_CLASS] : public [!output DIALOG_BASE_CLASS]
{
// Costruzione
public:
	[!output DIALOG_CLASS](CWnd* pParent = NULL);	// costruttore standard

// Dati della finestra di dialogo
	enum { IDD = IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_DIALOG };

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
protected:  // membri incorporati della barra di controllo
	CCommandBar m_dlgCommandBar;
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
protected:  // membri incorporati della barra di controllo
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// supporto DDX/DDV

// Implementazione
protected:
	HICON m_hIcon;

	// Funzioni generate per la mappa dei messaggi
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
[!if HELPSTYLE_WINHELP]
	afx_msg void OnHelpInfo()
[!endif]
	DECLARE_MESSAGE_MAP()
};
