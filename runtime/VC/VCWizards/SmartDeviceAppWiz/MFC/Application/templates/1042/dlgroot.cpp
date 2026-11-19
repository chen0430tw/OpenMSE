// [!output APP_IMPL] : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// [!output APP_CLASS] 생성
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS]::[!output APP_CLASS](LPCTSTR lpszHelpName)
	: [!output APP_BASE_CLASS](lpszHelpName)
[!else]
[!output APP_CLASS]::[!output APP_CLASS]()
	: [!output APP_BASE_CLASS]()
[!endif]
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 [!output APP_CLASS] 개체입니다.
[!if HELPSTYLE_WINHELP]
[!output APP_CLASS] theApp(_T("HELP_NAME"));
[!else]
[!output APP_CLASS] theApp;
[!endif]

// [!output APP_CLASS] 초기화

BOOL [!output APP_CLASS]::InitInstance()
{
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
    // CAPEDIT 및 SIPPREF와 같은 모든 Windows Mobile 관련 컨트롤을 초기화하기 위해
    // 응용 프로그램을 초기화하는 동안 SHInitExtraControls를 한 번 호출해야 합니다.
    SHInitExtraControls();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // CAPEDIT 및 SIPPREF와 같은 모든 Windows Mobile 관련 컨트롤을 초기화하기 위해
    // 응용 프로그램을 초기화하는 동안 SHInitExtraControls를 한 번 호출해야 합니다.
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
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	[!output DIALOG_CLASS] dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}
