// [!output APP_IMPL] : 定義應用程式的類別行為。
//

#include "stdafx.h"
#include "[!output APP_HEADER]"
#include "[!output DIALOG_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// [!output APP_CLASS]

BEGIN_MESSAGE_MAP([!output APP_CLASS], [!output APP_BASE_CLASS])
[!if HELPSTYLE_WINHELP]
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
[!endif]
END_MESSAGE_MAP()


// [!output APP_CLASS] 建構
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS]::[!output APP_CLASS](LPCTSTR lpszHelpName)
	: [!output APP_BASE_CLASS](lpszHelpName)
[!else]
[!output APP_CLASS]::[!output APP_CLASS]()
	: [!output APP_BASE_CLASS]()
[!endif]
{
	// TODO: 在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 僅有的一個 [!output APP_CLASS] 物件
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS] theApp(_T("HELP_NAME"));
[!else]
[!output APP_CLASS] theApp;
[!endif]

// [!output APP_CLASS] 初始設定

BOOL [!output APP_CLASS]::InitInstance()
{
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // 必須在應用程式初始化過程中呼叫一次 SHInitExtraControls，以初始化
    // 任何 Windows Mobile 專用的控制項，如 CAPEDIT 和 SIPPREF。
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // 必須在應用程式初始化過程中呼叫一次 SHInitExtraControls，以初始化
    // 任何 Windows Mobile 專用的控制項，如 CAPEDIT 和 SIPPREF。
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
	// 標準初始設定
	// 如果您不使用這些功能並且想減少
	// 最後完成的可執行檔大小，您可以
	// 從下列程式碼移除不需要的初始化常式，
	// 變更儲存設定值的登錄機碼
	// TODO: 您應該適度修改此字串
	// (例如，公司名稱或組織名稱)
	SetRegistryKey(_T("本機 AppWizard 所產生的應用程式"));

	[!output DIALOG_CLASS] dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置於使用 [確定] 來停止使用對話方塊時
		// 處理的程式碼
	}

	// 因為已經關閉對話方塊，傳回 FALSE，所以我們會結束應用程式，
	// 而非提示開始應用程式的訊息。
	return FALSE;
}
