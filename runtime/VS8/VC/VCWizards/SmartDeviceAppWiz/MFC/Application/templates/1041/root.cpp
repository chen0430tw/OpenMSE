// [!output APP_IMPL] : アプリケーションのクラス動作を定義します。
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
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, &[!output APP_BASE_CLASS]::OnFilePrintSetup)
[!endif]
[!if DOCVIEW]
	ON_COMMAND(ID_FILE_NEW, &[!output APP_BASE_CLASS]::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &[!output APP_BASE_CLASS]::OnFileOpen)
[!endif]
END_MESSAGE_MAP()



// [!output APP_CLASS] コンストラクション
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS]::[!output APP_CLASS](LPCTSTR lpszHelpName)
	: [!output APP_BASE_CLASS](lpszHelpName)
[!else]
[!output APP_CLASS]::[!output APP_CLASS]()
	: [!output APP_BASE_CLASS]()
[!endif]
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

[!if ATL_SUPPORT]
// ATL モジュール オブジェクト
CComModule _Module;
[!endif]

// 唯一の [!output APP_CLASS] オブジェクトです。
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS] theApp(_T("HELP_NAME"));
[!else]
[!output APP_CLASS] theApp;
[!endif]

// [!output APP_CLASS] 初期化

BOOL [!output APP_CLASS]::InitInstance()
{
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // CAPEDIT および SIPPREF のような Windows Mobile 特有のコントロールを初期化するには、アプリケーションの
    // 初期化中に SHInitExtraControls を一度呼び出す必要があります。
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // CAPEDIT および SIPPREF のような Pocket PC の特別なコントロールを初期化するには、アプリケーションの
    // 初期化中に SHInitExtraControls を一度呼び出す必要があります。
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
	// 標準初期化
	// これらの機能を使わずに、最終的な実行可能ファイルのサイズを縮小したい場合は、
	// 以下から、不要な初期化ルーチンを
	// 削除してください。
	
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: この文字列を、会社名または組織名などの、
	// 適切な文字列に変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
[!if !DOCVIEW]
	// メイン ウィンドウを作成するとき、このコードは新しいフレーム ウィンドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウィンドウにセットします
	CMainFrame* pFrame = new [!output MAIN_FRAME_CLASS];
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	// フレームをリソースからロードして作成します
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// ドキュメント一覧にファイルがない場合、新しいファイルを作成します。
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // メイン SDI フレーム ウィンドウ
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS([!output VIEW_CLASS]));
	AddDocTemplate(pDocTemplate);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	pDocTemplate->ShowDocList();	
	
	// ドキュメント一覧にファイルがない場合、新しいファイルを作成します。
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // メイン SDI フレーム ウィンドウ
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
		RUNTIME_CLASS([!output MAIN_FRAME_CLASS]),       // メイン SDI フレーム ウィンドウ
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
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!else]
#ifndef WIN32_PLATFORM_PSPC
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、FALSE を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
#endif // !WIN32_PLATFORM_PSPC
[!endif]
[!else]
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、FALSE を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
[!endif]
[!endif]

[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if !POCKETPC2003_UI_MODEL]
	// メイン ウィンドウが初期化されたので、表示と更新を行います
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
	// メイン ウィンドウが初期化されたので、表示と更新を行います
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
	// メイン ウィンドウが初期化されたので、表示と更新を行います
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
// [!output APP_CLASS] メッセージ ハンドラ

[!if DOCVIEW]
void [!output APP_CLASS]::OnFileNew() 
{
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
	CString strUntitled;
	CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, m_pMainWnd);
	if (pFrameWnd != NULL)
	{
		//TODO: メモリのファイル固有のデータを閉じて、破棄してください。
		// 一度終了したフレームウィンドウのタイトルをリセットするために
		// 次の if 文を実行してください。
		if (strUntitled.LoadString(AFX_IDS_UNTITLED))
			pFrameWnd->UpdateFrameTitleForDocument(strUntitled);
		else
			pFrameWnd->UpdateFrameTitleForDocument(NULL);
	}
[!else]
	[!output MAIN_FRAME_CLASS]* pFrame = STATIC_DOWNCAST([!output MAIN_FRAME_CLASS], m_pMainWnd);
	// 新しい MDI 子ウィンドウを作成します
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_[!output SAFE_DOC_TYPE_NAME]TYPE, m_hMDIMenu, m_hMDIAccel);
[!endif]
}
[!endif]
[!endif]

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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

	return TRUE;	// フォーカスをコントロールに設定した場合を除き、TRUE を返します。
			// 例外: OCX プロパティ ページは FALSE を返さなければなりません
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

// ダイアログを実行するためのアプリケーション コマンド
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
// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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

	return TRUE;	// フォーカスをコントロールに設定した場合を除き、TRUE を返します。
			// 例外: OCX プロパティ ページは FALSE を返さなければなりません
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

// ダイアログを実行するためのアプリケーション コマンド
void [!output APP_CLASS]::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]