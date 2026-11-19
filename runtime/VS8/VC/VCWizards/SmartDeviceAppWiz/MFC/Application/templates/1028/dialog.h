// [!output DIALOG_HEADER] : 標頭檔
//

#pragma once

// [!output DIALOG_CLASS] 對話方塊
class [!output DIALOG_CLASS] : public [!output DIALOG_BASE_CLASS]
{
// 建構
public:
	[!output DIALOG_CLASS](CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_DIALOG };

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
protected:  // 控制列內嵌的成員
	CCommandBar m_dlgCommandBar;
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
protected:  // 控制列內嵌的成員
	CCommandBar m_dlgCommandBar;
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
[!if HELPSTYLE_WINHELP]
	afx_msg void OnHelpInfo()
[!endif]
	DECLARE_MESSAGE_MAP()
};
