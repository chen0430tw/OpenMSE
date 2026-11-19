[!if DLL_APP]
// [!output PROJECT_NAME].cpp : 定义 DLL 应用程序的入口点。
[!endif]
[!if WIN_APP]
// [!output PROJECT_NAME].cpp : 定义应用程序的入口点。
[!endif]
[!if CONSOLE_APP]
// [!output PROJECT_NAME].cpp : 定义控制台应用程序的入口点。
[!endif]
//

#include "stdafx.h"
[!if WIN_APP || SUPPORT_MFC]
#include "[!output PROJECT_NAME].h"
[!else]
[!if DLL_APP && EXPORT_SYMBOLS]
#include "[!output PROJECT_NAME].h"
[!endif]
[!endif]
#include <windows.h>
#include <commctrl.h>

[!if WIN_APP]
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE			g_hInst;			// 当前实例
[!if !MULTIPLE_PLATFORM]
[!if !STANDARDSHELL_UI_MODEL]
HWND				g_hWndMenuBar;		// 菜单栏句柄
[!else]
HWND				g_hWndCommandBar;	// 命令栏句柄
[!endif]
[!else]
[!if !STANDARDSHELL_UI_MODEL]
HWND				g_hWndMenuBar;		// 菜单栏句柄
[!else]
#ifdef SHELL_AYGSHELL
HWND				g_hWndMenuBar;		// 菜单栏句柄
#else // SHELL_AYGSHELL
HWND				g_hWndCommandBar;	// 命令栏句柄
#endif // SHELL_AYGSHELL
[!endif]
[!endif]

// 此代码模块中包含的函数的前向声明:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]));
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]));
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// 标题栏文本
    TCHAR szWindowClass[MAX_LOADSTRING];	// 主窗口类名

    g_hInst = hInstance; // 将实例句柄存储在全局变量中

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // 在应用程序初始化期间，应调用一次 SHInitExtraControls 以初始化
    // 所有设备特定控件，例如，CAPEDIT 和 SIPPREF。
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // 在应用程序初始化期间，应调用一次 SHInitExtraControls 以初始化
    // 所有设备特定控件，例如，CAPEDIT 和 SIPPREF。
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP
[!endif]
[!endif]

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME], szWindowClass, MAX_LOADSTRING);

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    //如果它已经在运行，则将焦点置于窗口上，然后退出
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // 将焦点置于最前面的子窗口
        // “| 0x00000001”用于将所有附属窗口置于前台并
        // 激活这些窗口。
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    //如果它已经在运行，则将焦点置于窗口上，然后退出
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // 将焦点置于最前面的子窗口
        // “| 0x00000001”用于将所有附属窗口置于前台并
        // 激活这些窗口。
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP
[!endif]
[!endif]

    if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }

    hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
    // 使用 CW_USEDEFAULT 创建主窗口时，将不会考虑菜单栏的高度(如果创建了一个
    // 菜单栏)。因此，我们要在创建窗口后调整其大小
    // 如果菜单栏存在
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
    // 使用 CW_USEDEFAULT 创建主窗口时，将不会考虑菜单栏的高度(如果创建了一个
    // 菜单栏)。因此，我们要在创建窗口后调整其大小
    // 如果菜单栏存在
    if (g_hWndMenuBar)
    {
        RECT rc;
        RECT rcMenuBar;

        GetWindowRect(hWnd, &rc);
        GetWindowRect(g_hWndMenuBar, &rcMenuBar);
        rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);
		
        MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
    }
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

[!if !MULTIPLE_PLATFORM]
[!if STANDARDSHELL_UI_MODEL]
    if (g_hWndCommandBar)
    {
        CommandBar_Show(g_hWndCommandBar, TRUE);
    }
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL]
#ifndef SHELL_AYGSHELL
    if (g_hWndCommandBar)
    {
        CommandBar_Show(g_hWndCommandBar, TRUE);
    }
#endif // !SHELL_AYGSHELL
[!endif]
[!endif]

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
    static SHACTIVATEINFO s_sai;
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
    static SHACTIVATEINFO s_sai;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP
[!else]
    static SHACTIVATEINFO s_sai;
[!endif]
[!endif]
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // 分析菜单选择:
            switch (wmId)
            {
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]
[!if !MULTIPLE_PLATFORM]
[!if STANDARDSHELL_UI_MODEL]
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
[!else]
[!if !SMARTPHONE2003_UI_MODEL]
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
                    break;
[!else]
                case IDM_OK:
                    DestroyWindow(hWnd);
                    break;
[!endif]
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL]
#ifndef SHELL_AYGSHELL
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
#endif // !SHELL_AYGSHELL
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef WIN32_PLATFORM_WFSP
                case IDM_OK:
                    DestroyWindow(hWnd);
                    break;
#endif // WIN32_PLATFORM_WFSP
[!endif]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
                case IDM_OK:
                    SendMessage (hWnd, WM_CLOSE, 0, 0);				
                    break;
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
[!if !MULTIPLE_PLATFORM]
[!if STANDARDSHELL_UI_MODEL]
            g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
            CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
            CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
[!else]
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

[!if !SMARTPHONE2003_UI_MODEL]
            // 初始化外壳程序激活信息结构
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
[!endif]
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL]
#ifndef SHELL_AYGSHELL
            g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
            CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
            CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
#endif // !SHELL_AYGSHELL
[!endif]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#ifdef SHELL_AYGSHELL
            SHMENUBARINFO mbi;

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hWnd;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
            // 初始化外壳程序激活信息结构
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
#endif // SHELL_AYGSHELL
[!endif]
[!endif]
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: 在此添加任意绘图代码...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
[!if !MULTIPLE_PLATFORM]
[!if STANDARDSHELL_UI_MODEL]
            CommandBar_Destroy(g_hWndCommandBar);
[!else]
            CommandBar_Destroy(g_hWndMenuBar);
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL]
#ifndef SHELL_AYGSHELL
            CommandBar_Destroy(g_hWndCommandBar);
#endif // !SHELL_AYGSHELL
[!endif]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#ifdef SHELL_AYGSHELL
            CommandBar_Destroy(g_hWndMenuBar);
#endif // SHELL_AYGSHELL
[!endif]
[!endif]
            PostQuitMessage(0);
            break;

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
        case WM_ACTIVATE:
            // 向外壳程序通知我们的激活消息
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(SHELL_AYGSHELL) && !defined(WIN32_PLATFORM_WFSP)
        case WM_ACTIVATE:
            // 向外壳程序通知我们的激活消息
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;
#endif // SHELL_AYGSHELL && !WIN32_PLATFORM_WFSP
[!else]
        case WM_ACTIVATE:
            // 向外壳程序通知我们的激活消息
            SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
            break;
[!endif]
[!endif]

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
[!if STANDARDSHELL_UI_MODEL]
            RECT rectChild, rectParent;
            int DlgWidth, DlgHeight;	// 以像素为单位的对话框宽度和高度
            int NewPosX, NewPosY;

            // 设法使“关于”对话框居中显示
            if (GetWindowRect(hDlg, &rectChild)) 
            {
                GetClientRect(GetParent(hDlg), &rectParent);
                DlgWidth	= rectChild.right - rectChild.left;
                DlgHeight	= rectChild.bottom - rectChild.top ;
                NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
                NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;
				
                // 如果“关于”框比实际屏幕大
                if (NewPosX < 0) NewPosX = 0;
                if (NewPosY < 0) NewPosY = 0;
                SetWindowPos(hDlg, 0, NewPosX, NewPosY,
                    0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
[!else]
            {
                // 创建一个“完成”按钮并调整其大小。
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
[!endif]
            return (INT_PTR)TRUE;

        case WM_COMMAND:
[!if STANDARDSHELL_UI_MODEL]
            if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
[!else]
            if (LOWORD(wParam) == IDOK)
[!endif]
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

[!if POCKETPC2003_UI_MODEL]
[!if SUPPORT_MFC]
#ifdef _DEVICE_RESOLUTION_AWARE
        case WM_SIZE:
            {
		if (AfxIsDRAEnabled())
    		{
			DRA::RelayoutDialog(
				g_hInst, 
				hDlg, 
				DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
		}
            }
            break;
#endif
[!endif]
[!endif]
    }
    return (INT_PTR)FALSE;
}
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
[!if STANDARDSHELL_UI_MODEL]
#ifndef SHELL_AYGSHELL
            RECT rectChild, rectParent;
            int DlgWidth, DlgHeight;	// 以像素为单位的对话框宽度和高度
            int NewPosX, NewPosY;

            // 设法使“关于”对话框居中显示
            if (GetWindowRect(hDlg, &rectChild)) 
            {
                GetClientRect(GetParent(hDlg), &rectParent);
                DlgWidth	= rectChild.right - rectChild.left;
                DlgHeight	= rectChild.bottom - rectChild.top ;
                NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
                NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;
				
                // 如果“关于”框比实际屏幕大
                if (NewPosX < 0) NewPosX = 0;
                if (NewPosY < 0) NewPosY = 0;
                SetWindowPos(hDlg, 0, NewPosX, NewPosY,
                    0, 0, SWP_NOZORDER | SWP_NOSIZE);
            }
#endif // !SHELL_AYGSHELL
[!endif]
[!if POCKETPC2003_UI_MODEL]
#ifdef SHELL_AYGSHELL
            {
                // 创建一个“完成”按钮并调整其大小。
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
#endif // SHELL_AYGSHELL
[!endif]

            return (INT_PTR)TRUE;

        case WM_COMMAND:
[!if STANDARDSHELL_UI_MODEL]
#ifndef SHELL_AYGSHELL
            if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
#endif // !SHELL_AYGSHELL
[!endif]
[!if POCKETPC2003_UI_MODEL]
#ifdef SHELL_AYGSHELL
            if (LOWORD(wParam) == IDOK)
#endif
[!endif]
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return (INT_PTR)TRUE;

[!if POCKETPC2003_UI_MODEL]
[!if SUPPORT_MFC]
#ifdef _DEVICE_RESOLUTION_AWARE
        case WM_SIZE:
            {
		if (AfxIsDRAEnabled())
    		{
			DRA::RelayoutDialog(
				g_hInst, 
				hDlg, 
				DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
		}
            }
            break;
#endif
[!endif]
[!endif]
    }
    return (INT_PTR)FALSE;
}
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]
[!endif]
[!if SUPPORT_MFC]
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	return nRetCode;
}
[!else]
[!if DLL_APP]
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
[!if EXPORT_SYMBOLS]
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
[!endif]
    return TRUE;
}

[!if EXPORT_SYMBOLS]
// 这是导出变量的一个示例
[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_API int n[!output SAFE_PROJECT_IDENTIFIER_NAME]=0;

// 这是导出函数的一个示例。
[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_API int fn[!output SAFE_PROJECT_IDENTIFIER_NAME](void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 [!output PROJECT_NAME].h
C[!output SAFE_PROJECT_IDENTIFIER_NAME]::C[!output SAFE_PROJECT_IDENTIFIER_NAME]()
{ 
	return; 
}
[!endif]
[!endif]
[!if CONSOLE_APP]

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

[!endif]
[!endif]
