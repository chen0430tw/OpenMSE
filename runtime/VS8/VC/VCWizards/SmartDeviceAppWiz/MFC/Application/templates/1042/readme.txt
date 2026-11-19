================================================================================
    MFC 라이브러리 : [!output PROJECT_NAME] 프로젝트 개요
===============================================================================

응용 프로그램 마법사에서 이 [!output PROJECT_NAME] 응용 프로그램을 
만들었습니다. 이 응용 프로그램은 MFC의 기본 사용법을 보여 줄 뿐만 아니라 응용 프로그램
작성을 위한 기본 구조를 제공합니다.

이 파일에는 [!output PROJECT_NAME] 응용 프로그램을 구성하는 각 파일에 대한
요약 설명이 포함되어 있습니다.

[!output PROJECT_NAME].vcproj
    응용 프로그램 마법사를 사용하여 생성된 VC++ 프로젝트의 주 프로젝트 파일입니다. 
    파일을 생성한 Visual C++ 버전에 대한 정보와 응용 프로그램 마법사를 사용하여 선택한 
    플랫폼, 구성 및 프로젝트 기능에 대한 정보가 들어 있습니다.

[!output APP_HEADER]
    응용 프로그램의 기본 헤더 파일입니다. 여기에는 다른 프로젝트 관련
    헤더가 들어 있고 [!output APP_CLASS] 응용 프로그램 클래스를 선언합니다.

[!output APP_IMPL]
    응용 프로그램 클래스 [!output APP_CLASS]이(가) 들어 있는 기본 응용 프로그램
    소스 파일입니다.

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    프로젝트에서 사용하는 모든 Microsoft Windows 리소스를 나열하는
    프로젝트의 주 리소스 파일입니다. 여기에는 RES 하위 디렉터리에 
저장된 아이콘, 비트맵 및 커서가 포함됩니다. 이 파일은 Microsoft Visual C++에서
    직접 편집할 수 있습니다. 프로젝트 리소스는 [!output LCID]에 있습니다.
    .rc 파일이 유지되면 데이터 섹션의 정의는 해당 정의의 이름이 아닌 정의된 16진수
    숫자 값으로 유지됩니다.

res\[!output RC2_FILE_NAME].rc2
    이 파일에는 Microsoft Visual C++ 이외의 다른 도구에서 편집한 리소스가 
    들어 있습니다. 리소스 편집기로 편집할 수 없는 모든 리소스는
    이 파일에 넣어야 합니다.
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    Pocket PC 플랫폼 또는 동일한 사용자 인터페이스 모델을 지원하는 플랫폼용으로
    컴파일할 때 프로젝트에서 사용하는 모든 Microsoft Windows 리소스를 나열하는
    프로젝트의 주 리소스 파일입니다. 여기에는 RES 하위 디렉터리에
    저장된 아이콘, 비트맵 및 커서가 포함됩니다. 이 파일은 Microsoft
    Visual C++에서 직접 편집할 수 있습니다. 프로젝트 리소스는 [!output LCID]에
    있습니다. .rc 파일이 유지되면 데이터 섹션의 정의는 해당 정의의 이름이
    아닌 정의된 16진수 숫자 값으로 유지됩니다.

res\[!output RC2_FILE_NAME]ppc.rc2
    이 파일에는 Microsoft Visual C++ 이외의 다른 도구에서 편집한 리소스가 
    들어 있습니다. 리소스 편집기로 편집할 수 없는 모든 리소스는
    이 파일에 넣어야 합니다.
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    Smartphone 플랫폼 또는 동일한 사용자 인터페이스 모델을 지원하는 플랫폼용으로
    컴파일할 때 프로젝트에서 사용하는 모든 Microsoft Windows 리소스를 나열하는
    프로젝트의 주 리소스 파일입니다. 여기에는 RES 하위 디렉터리에
    저장된 아이콘, 비트맵 및 커서가 포함됩니다. 이 파일은 Microsoft
    Visual C++에서 직접 편집할 수 있습니다. 프로젝트 리소스는 [!output LCID]에
    있습니다. .rc 파일이 유지되면 데이터 섹션의 정의는 해당 정의의 이름이
    아닌 정의된 16진수 숫자 값으로 유지됩니다.

res\[!output RC2_FILE_NAME]sp.rc2
    이 파일에는 Microsoft Visual C++ 이외의 다른 도구에서 편집한 리소스가 
    들어 있습니다. 리소스 편집기로 편집할 수 없는 모든 리소스는
    이 파일에 넣어야 합니다.
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    AYGShell 사용자 인터페이스 모델을 지원하는 플랫폼용으로 컴파일할 때
    프로젝트에서 사용하는 모든 Microsoft Windows 리소스를 나열하는 프로젝트의
    주 리소스 파일입니다. 여기에는 RES 하위 디렉터리에 
저장된 아이콘, 비트맵 및 커서가 포함됩니다. 이 파일은 Microsoft Visual C++에서
    직접 편집할 수 있습니다. 프로젝트 리소스는 [!output LCID]에 있습니다.
    .rc 파일이 유지되면 데이터 섹션의 정의는 해당 정의의 이름이 아닌 정의된 16진수
    숫자 값으로 유지됩니다.

res\[!output RC2_FILE_NAME]ayg.rc2
    이 파일에는 Microsoft Visual C++ 이외의 다른 도구에서 편집한 리소스가 
    들어 있습니다. 리소스 편집기로 편집할 수 없는 모든 리소스는
    이 파일에 넣어야 합니다.
[!endif]

res\[!output PROJECT_NAME].ico
    응용 프로그램의 아이콘으로 사용되는 아이콘 파일입니다. 이 아이콘은
    주 리소스 파일에 의해 포함됩니다.

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
    프레임워크에서 자동으로 설정되는 등록 설정의 종류를 보여 주는 예제
    .reg 파일입니다. 이 파일을 사용자 응용 프로그램의
[!if APP_TYPE_DLG]
    .reg 파일로 사용할 수 있습니다.
[!else]
    .reg 파일로 사용하거나, 이 파일을 삭제하고 기본 RegisterShellFileTypes
    등록을 사용할 수 있습니다.
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

주 프레임 창의 경우:
    프로젝트에는 표준 MFC 인터페이스가 포함됩니다.

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    이 파일에는 CframeWnd에서 파생되며 모든 SDI 프레임 기능을 제어하는 프레임 클래스
    [!output MAIN_FRAME_CLASS]이(가) 들어 있습니다.

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    이 비트맵 파일은 명령 모음을 위한 바둑판식 이미지를 만드는 데 사용됩니다.
    초기 명령 모음은 [!output MAIN_FRAME_CLASS] 클래스에 생성됩니다. 
    이 cmdbar 비트맵을 [!output MAIN_FRAME_IMPL]의 배열과 함께 편집하여
    명령 모음 단추를 추가합니다.
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
응용 프로그램 마법사에서 하나의 문서 형식과 하나의 뷰가 만들어집니다.

[!output DOC_HEADER], [!output DOC_IMPL] ? 문서
    이 파일에는 [!output DOC_CLASS] 클래스가 들어 있습니다. 이 파일을 편집하여
    특수한 문서 데이터를 추가하고 [!output DOC_CLASS]::Serialize를 통해
    파일 저장 및 로드를 구현합니다.
[!if HAS_SUFFIX]
    이 문서에는 다음 문자열이 포함됩니다.
        파일 확장명:      [!output FILE_EXTENSION]
        파일 형식 ID:        [!output FILE_TYPE_ID]
        주 프레임 캡션:  [!output MAIN_FRAME_CAPTION]
        문서 형식 이름:       [!output DOC_TYPE_NAME]
        필터 이름:         [!output FILTER_NAME]
        파일의 새 약식 이름: [!output FILE_NEW_NAME_SHORT]
        파일 형식의 긴 이름: [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] ? 문서 뷰
    이 파일에는 [!output VIEW_CLASS] 클래스가 들어 있습니다.
    [!output VIEW_CLASS] 개체는 [!output DOC_CLASS] 개체를 보는 데 사용됩니다.

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

응용 프로그램 마법사에서 하나의 대화 상자 클래스가 만들어집니다.

[!output DIALOG_HEADER], [!output DIALOG_IMPL] ? 대화 상자
    이 파일에는 [!output DIALOG_CLASS] 클래스가 들어 있습니다. 이 클래스는
    응용 프로그램의 주 대화 상자 동작을 정의합니다. 대화 상자의 템플릿은
    Microsoft Visual C++에서 편집할 수 있는 주 리소스 파일에 있습니다.
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

도움말 지원:

hlp\[!output PROJECT_NAME].hpj
    도움말 컴파일러에서 응용 프로그램의 도움말 파일을 만드는 데 사용되는
    도움말 프로젝트 파일입니다.

hlp\*.bmp
    MFC 라이브러리 표준 명령에 대한 표준 도움말 파일 항목에 필요한
    비트맵 파일입니다.

hlp\*.rtf
    표준 MFC 명령 및 화면 개체에 대한 표준 도움말 항목이 들어
    있습니다.
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

기타 기능:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

   메뉴 및 장식이 있는 주 프레임의 명령 모음
[!else]
[!if CMD_BAR_MENUNBUTTON]
   메뉴, 단추 및 장식이 있는 주 프레임의 메뉴 모음
[!endif]
[!endif]
[!if STATUS_BAR]
   주 프레임의 초기 상태 표시줄
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

   Windows CE 도움말 지원
[!endif]
[!if DYNAMIC_MFC]

   공유 DLL 구현 사용
[!endif]
[!if ACTIVEX_CONTROLS]

ActiveX 컨트롤
    응용 프로그램에서 ActiveX 컨트롤을 사용할 수 있도록 지원합니다.
[!endif]
[!if PRINTING]

인쇄 및 인쇄 미리 보기 지원
    응용 프로그램 마법사가 MFC 라이브러리에서 CView 클래스의 멤버 함수를 호출하여 인쇄, 인쇄 설정
    및 인쇄 미리 보기 명령을 처리하는 코드를 생성했습니다.
[!endif]
[!if SOCKETS]

Windows 소켓
    응용 프로그램에서 TCP/IP 네트워크를 사용한 통신을 지원합니다.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

기타 표준 파일:

StdAfx.h, StdAfx.cpp
    이 파일은 미리 컴파일된 헤더 파일(PCH)인 [!output PROJECT_NAME].pch와
    미리 컴파일된 형식 파일인 StdAfx.obj를 빌드하는 데 사용됩니다.

[!if STANDARDSHELL_UI_MODEL]
Resource.h
[!endif]
[!if POCKETPC2003_UI_MODEL]
Resourceppc.h
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
Resourcesp.h
[!endif]
[!if AYGSHELL_UI_MODEL]
Resourceayg.h
[!endif]
    새 리소스 ID를 정의하는 표준 헤더 파일입니다.
    Microsoft Visual C++에서 이 파일을 읽고 업데이트합니다.

/////////////////////////////////////////////////////////////////////////////

기타 참고:

응용 프로그램 마법사는 "TODO:"를 사용하여 추가하거나 사용자 지정해야 하는
소스 코드 부분을 나타냅니다.
[!if APP_TYPE_SDI || APP_TYPE_DLG]

응용 프로그램이 공유 DLL에서 MFC를 사용하고 운영 체제의 현재 언어와 다른
언어를 사용하는 경우 해당 지역화된 리소스인 MFC80XXX.DLL을 응용 프로그램
디렉터리에 복사해야 합니다. "XXX"는 언어 약어를 나타냅니다. 예를 들어,
MFC80DEU.DLL에는 독일어로 번역된 리소스가 포함되어 있습니다. 이렇게 하지 않으면
응용 프로그램의 일부 UI 요소가 운영 체제의 언어로 남아 있게 됩니다.
[!endif]

/////////////////////////////////////////////////////////////////////////////