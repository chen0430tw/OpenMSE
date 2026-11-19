// [!output DIALOG_HEADER] : fichier d'en-tête
//

#pragma once

// Boîte de dialogue [!output DIALOG_CLASS]
class [!output DIALOG_CLASS] : public [!output DIALOG_BASE_CLASS]
{
// Construction
public:
	[!output DIALOG_CLASS](CWnd* pParent = NULL);	// constructeur standard

// Données de boîte de dialogue
	enum { IDD = IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_DIALOG };

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
protected:  // membres incorporés de la barre de contrôle
	CCommandBar m_dlgCommandBar;
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
protected:  // membres incorporés de la barre de contrôle
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge de DDX/DDV

// Implémentation
protected:
	HICON m_hIcon;

	// Fonctions générées de la table des messages
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
[!if HELPSTYLE_WINHELP]
	afx_msg void OnHelpInfo()
[!endif]
	DECLARE_MESSAGE_MAP()
};
