// [!output DIALOG_HEADER] : header file
//

#pragma once

// [!output DIALOG_CLASS] dialog
class [!output DIALOG_CLASS] : public [!output DIALOG_BASE_CLASS]
{
// Construction
public:
	[!output DIALOG_CLASS](CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_DIALOG };

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
protected:  // control bar embedded members
	CCommandBar m_dlgCommandBar;
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
protected:  // control bar embedded members
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
[!if HELPSTYLE_WINHELP]
	afx_msg void OnHelpInfo()
[!endif]
	DECLARE_MESSAGE_MAP()
};
