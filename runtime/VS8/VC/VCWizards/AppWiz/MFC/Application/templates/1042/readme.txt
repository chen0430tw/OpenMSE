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
    헤더(Resource.h 포함)가 들어 있고 [!output APP_CLASS] 응용 프로그램
    클래스를 선언합니다.

[!output APP_IMPL]
    응용 프로그램 클래스 [!output APP_CLASS]이(가) 들어 있는 기본 응용 프로그램
    소스 파일입니다.

[!output PROJECT_NAME].rc
    프로그램에서 사용하는 모든 Microsoft Windows 리소스의 목록입니다.
 여기에는 RES 하위 디렉터리에 저장된 아이콘, 비트맵 및 커서가
    포함됩니다. 이 파일은 Microsoft Visual C++에서 직접
    편집할 수 있습니다. 프로젝트 리소스는 [!output LCID]에 있습니다.

res\[!output PROJECT_NAME].ico
    응용 프로그램의 아이콘으로 사용되는 아이콘 파일입니다. 이 아이콘은
    주 리소스 파일인 [!output PROJECT_NAME].rc에 의해 포함됩니다.

res\[!output RC2_FILE_NAME].rc2
    이 파일에는 Microsoft Visual C++ 이외의 다른 도구에서 편집한 리소스가 
    들어 있습니다. 리소스 편집기로 편집할 수 없는 모든 리소스는
    이 파일에 넣어야 합니다.
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
    프레임워크에서 자동으로 설정되는 등록 설정의 종류를 보여 주는 예제
    .reg 파일입니다. 이 파일을 사용자 응용 프로그램의
[!if APP_TYPE_DLG]
    .reg 파일로 사용할 수 있습니다.
[!else]
    .reg 파일로 사용하거나, 이 파일을 삭제하고 기본 RegisterShellFileTypes
    등록을 사용할 수 있습니다.
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
    응용 프로그램의 형식 라이브러리에 대한 인터페이스 정의 언어 소스 코드가
    들어 있습니다.
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

주 프레임 창의 경우:
[!if PROJECT_STYLE_EXPLORER]
    Windows 탐색기 스타일: 프로젝트에는 두 개의 프레임으로 구성된 Windows 탐색기
    스타일의 인터페이스가 포함됩니다.
[!else]
    프로젝트에는 표준 MFC 인터페이스가 포함됩니다.
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    이 파일에는 프레임 클래스 [!output MAIN_FRAME_CLASS]이(가) 들어 있습니다.
[!if APP_TYPE_MDI]
    이 클래스는 CMDIFrameWnd에서 파생되며 모든 MDI 프레임 기능을 제어합니다.
[!else]
    이 클래스는CFrameWnd에서 파생되며 모든 SDI 프레임 기능을 제어합니다.
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    이 파일에는 CTreeView에서 파생되는 왼쪽 프레임 클래스 [!output TREE_VIEW_CLASS]이(가)
    들어 있습니다.
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    이 비트맵 파일은 도구 모음을 위한 바둑판식 이미지를 만드는 데 사용됩니다.
    초기 도구 모음과 상태 표시줄은 [!output MAIN_FRAME_CLASS] 클래스에서 생성됩니다.
 리소스 편집기를 사용하여 이 도구 모음 비트맵을 편집하고
    [!output PROJECT_NAME].rc의 IDR_MAINFRAME TOOLBAR 배열을 업데이트하여 도구 모음
    단추를 추가합니다.
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    이 비트맵 파일은 서버 응용 프로그램이 다른 컨테이너 안에서 내부
    활성화되었을 때 도구 모음을 위한 바둑판식 이미지를 만드는 데
    사용됩니다. 이 도구 모음은 [!output INPLACE_FRAME_CLASS] 클래스에서 생성됩니다.
 서버 명령 이외의 명령이 많이 제거되었다는 점을 제외하면
    이 비트맵은 res\Toolbar.bmp의 비트맵과 유사합니다.

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

자식 프레임 창의 경우:

ChildFrm.h, ChildFrm.cpp
    이 파일은 MDI 응용 프로그램에서 자식 창을 지원하는 [!output CHILD_FRAME_CLASS]
    클래스를 정의하고 구현합니다.

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
응용 프로그램 마법사에서 하나의 문서 형식과 하나의 뷰가 만들어집니다.

[!output DOC_HEADER], [!output DOC_IMPL] ? 문서
    이 파일에는 [!output DOC_CLASS] 클래스가 들어 있습니다. 이 파일을 편집하여
    특수한 문서 데이터를 추가하고 [!output DOC_CLASS]::Serialize를 통해 파일
    저장 및 로드를 구현합니다.
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    이 문서에는 다음 문자열이 포함됩니다.
        파일 확장명:      [!output FILE_EXTENSION]
        파일 형식 ID:        [!output FILE_TYPE_ID]
        주 프레임 캡션:  [!output MAIN_FRAME_CAPTION]
        문서 형식 이름:       [!output DOC_TYPE_NAME]
        필터 이름:         [!output FILTER_NAME]
        파일의 새 약식 이름: [!output FILE_NEW_NAME_SHORT]
        파일 형식의 긴 이름: [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
응용 프로그램 마법사에서 하나의 뷰가 만들어집니다.
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] ? 문서 뷰
    이 파일에는 [!output VIEW_CLASS] 클래스가 들어 있습니다.
[!if !DB_VIEW_NO_FILE]
    [!output VIEW_CLASS] 개체는 [!output DOC_CLASS] 개체를 보는 데 사용됩니다.
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
    [!output DOC_CLASS] 클래스의 MDI 자식 창에 대한 아이콘으로 사용되는
    아이콘 파일입니다. 이 아이콘은 주 리소스 파일인 [!output PROJECT_NAME].rc에
    의해 포함됩니다.
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

데이터베이스 지원:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    이 파일에는 [!output ROWSET_CLASS] 클래스가 들어 있습니다. 이 클래스는 마법사에서 선택한
    데이터 소스에 액세스하는 데 사용됩니다.
[!if DB_VIEW_NO_FILE]
    serialization 지원은 추가되지 않습니다.
[!else]
    serialization 지원이 추가되었습니다.
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

응용 프로그램 마법사가 OLE 관련 클래스도 만들었습니다.

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    이 파일에는 [!output CONTAINER_ITEM_CLASS] 클래스가 들어 있습니다. 이 클래스는 OLE 개체를
    조작하는 데 사용됩니다. OLE 개체는 대개 [!output VIEW_CLASS]
    클래스에 의해 표시되며 [!output DOC_CLASS] 클래스의 일부로 serialize됩니다.
[!if ACTIVE_DOC_CONTAINER]
    프로그램에서 프레임 내에 액티브 문서를 포함할 수 있도록 지원합니다.
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    이 파일에는 [!output SERVER_ITEM_CLASS]이(가) 들어 있습니다. 이 클래스는
    [!output DOC_CLASS] 클래스를 OLE 시스템에 연결하고 선택적으로 문서에 대한
    링크를 제공하는 데 사용됩니다.
[!if ACTIVE_DOC_SERVER]
    프로젝트에서 액티브 문서를 만들고 관리하도록 지원합니다.
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    이 파일에는 [!output INPLACE_FRAME_CLASS]이(가) 들어 있습니다. 이 클래스는
    COleIPFrameWnd에서 파생되고 내부 활성화 중에 모든 프레임 기능을 제어합니다.
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    프로젝트에서 복합 파일을 지원합니다. 복합 파일 형식을 사용하면 하나 이상의 자동화 개체를
    포함하는 문서를 하나의 파일에 저장하고 그 안의 개별 개체에 액세스할 수 있습니다.
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
응용 프로그램 마법사에서 하나의 대화 상자와 자동화 프록시 클래스가 만들어집니다.
[!else]
응용 프로그램 마법사에서 하나의 대화 상자 클래스가 만들어집니다.
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL] ? 대화 상자
    이 파일에는 [!output DIALOG_CLASS] 클래스가 들어 있습니다. 이 클래스는
    응용 프로그램의 주 대화 상자 동작을 정의합니다. 대화 상자의 템플릿은
    Microsoft Visual C++에서 편집할 수 있는 [!output PROJECT_NAME].rc에 있습니다.
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] ? 자동화 개체
    이 파일에는 [!output DIALOG_AUTO_PROXY_CLASS] 클래스가 들어 있습니다. 이 클래스는
    자동화 컨트롤러가 대화 상자에 액세스하기 위해 사용할 수 있는 자동화 메서드 및 속성의
    노출을 관리하므로 대화 상자에 대한 자동화 프록시 클래스라고 합니다. 모달 대화 상자
    기반 MFC 응용 프로그램의 경우 자동화 개체를 사용자 인터페이스와 별도로
    유지하는 것이 보다 명확하고 편리하므로 이러한 메서드 및 속성은 대화 상자
    클래스에서 직접 노출되지 않습니다.
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

도움말 지원:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
    도움말 프로젝트 파일입니다. 여기에는 도움말 파일을 .chm 파일로
    컴파일하는 데 필요한 데이터가 들어 있습니다.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
    도움말 프로젝트의 내용을 보여 줍니다.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
    도움말 항목의 색인이 들어 있습니다.

hlp\afxcore.htm
    표준 MFC 명령 및 화면 개체에 대한 표준 도움말 항목이 들어
    있습니다. 이 파일에 사용자 지정 도움말 항목을 추가합니다.

[!if PRINTING]
hlp\afxprint.htm
    인쇄 명령에 대한 도움말 항목이 들어 있습니다.

[!endif]
makehtmlhelp.bat
    이 파일은 빌드 시스템에서 도움말 파일을 컴파일하는 데 사용됩니다.

hlp\Images\*.gif
    MFC 라이브러리 표준 명령에 대한 표준 도움말 파일 항목에 필요한
    비트맵 파일입니다.

[!else]
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
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

기타 기능:
[!if ACTIVEX_CONTROLS]

ActiveX 컨트롤
    응용 프로그램에서 ActiveX 컨트롤을 사용할 수 있도록 지원합니다.
[!endif]
[!if PRINTING]

인쇄 및 인쇄 미리 보기 지원
    응용 프로그램 마법사가 MFC 라이브러리에서 CView 클래스의 멤버 함수를 호출하여 인쇄, 인쇄 설정
    및 인쇄 미리 보기 명령을 처리하는 코드를 생성했습니다.
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

데이터베이스 지원
    응용 프로그램 마법사가 프로그램에 기본 수준의 데이터베이스 지원을 추가했습니다. 
    필요한 파일만 포함되었습니다.
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

분할 창
    응용 프로그램 마법사가 응용 프로그램 문서에 분할 창 지원을 추가했습니다.
[!endif]
[!if MAPI]

MAPI 지원
    생성된 프로젝트에는 메일 메시지를 작성하고, 조작하고, 전송하고, 저장하는 데 필요한 코드가
    들어 있습니다.
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

Resource.h
    새 리소스 ID를 정의하는 표준 헤더 파일입니다.
    Microsoft Visual C++에서 이 파일을 읽고 업데이트합니다.

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	응용 프로그램 매니페스트 파일은 Windows XP에서 특정 버전의 Side-by-Side 
	어셈블리에 대한 응용 프로그램 종속성을 설명하는 데 사용됩니다. 로더는 이 정보를 
	사용하여 어셈블리 캐시에서 적절한 어셈블리를 로드하거나 응용 프로그램에서 전용 
	어셈블리를 로드합니다. 응용 프로그램 매니페스트는 응용 프로그램 실행 파일과 같은 
	폴더에 설치된 외부 .manifest 파일로 재배포를 위해 포함되거나 리소스의 형태로 
	실행 파일에 포함될 수 있습니다. 
[!endif]
/////////////////////////////////////////////////////////////////////////////

기타 참고:

응용 프로그램 마법사는 "TODO:"를 사용하여 추가하거나 사용자 지정해야 하는
소스 코드 부분을 나타냅니다.
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

응용 프로그램이 공유 DLL에서 MFC를 사용하는 경우 해당 MFC DLL을 
재배포해야 합니다. 그리고 응용 프로그램이 운영 체제의 로캘과 
다른 언어를 사용하는 경우 해당 지역화된 리소스인 MFC80XXX.DLL을 
재배포해야 합니다. 이 두가지 항목에 대한 자세한 내용은 
MSDN 설명서의 Visual C++ 응용 프로그램 재배포 항목을 
참조하십시오.
[!endif]

/////////////////////////////////////////////////////////////////////////////