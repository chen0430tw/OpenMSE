// [!output DIALOG_IMPL] : 实现文件
//

#include "stdafx.h"
#include "[!output APP_HEADER]"
#include "[!output DIALOG_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// [!output DIALOG_CLASS] 对话框

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


// [!output DIALOG_CLASS] 消息处理程序

BOOL [!output DIALOG_CLASS]::OnInitDialog()
{
	[!output DIALOG_BASE_CLASS]::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

[!if !MULTIPLE_PLATFORM]
[!if SMARTPHONE2003_UI_MODEL]
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建 CommandBar\n");
		return FALSE;      // 未能创建
	}
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!endif]
	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	// TODO: 在此处实现帮助
	MessageBox(_T("帮助"));
}
[!endif]
