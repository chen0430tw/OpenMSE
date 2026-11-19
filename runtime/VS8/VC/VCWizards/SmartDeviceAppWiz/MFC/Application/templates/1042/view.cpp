// [!output VIEW_IMPL] : [!output VIEW_CLASS] 클래스의 구현
//

#include "stdafx.h"
#include "[!output APP_HEADER]"

#include "[!output DOC_HEADER]"
#include "[!output VIEW_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// [!output VIEW_CLASS]

IMPLEMENT_DYNCREATE([!output VIEW_CLASS], [!output VIEW_BASE_CLASS])

BEGIN_MESSAGE_MAP([!output VIEW_CLASS], [!output VIEW_BASE_CLASS])
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif]
[!if PRINTING]
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &[!output VIEW_BASE_CLASS]::OnFilePrintPreview)
[!endif]
END_MESSAGE_MAP()

// [!output VIEW_CLASS] 생성/소멸

[!output VIEW_CLASS]::[!output VIEW_CLASS]()
[!if FORM_VIEW]
	: [!output VIEW_BASE_CLASS]([!output VIEW_CLASS]::IDD)
[!endif]
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

[!output VIEW_CLASS]::~[!output VIEW_CLASS]()
{
}
[!if FORM_VIEW]

void [!output VIEW_CLASS]::DoDataExchange(CDataExchange* pDX)
{
	[!output VIEW_BASE_CLASS]::DoDataExchange(pDX);
}
[!endif]

BOOL [!output VIEW_CLASS]::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return [!output VIEW_BASE_CLASS]::PreCreateWindow(cs);
}

[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]

// [!output VIEW_CLASS] 그리기
void [!output VIEW_CLASS]::OnDraw(CDC* /*pDC*/)
{
	[!output DOC_CLASS]* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}
[!endif] 

[!if SCROLL_VIEW || LIST_VIEW || TREE_VIEW || FORM_VIEW]

void [!output VIEW_CLASS]::OnInitialUpdate()
{
	[!output VIEW_BASE_CLASS]::OnInitialUpdate();

[!if LIST_VIEW]
	// TODO: GetListCtrl()을 호출하여 해당 list 컨트롤을 직접 액세스함으로써
	//  ListView를 항목으로 채울 수 있습니다.
[!endif]
[!if TREE_VIEW]
	// TODO: TreeView를 항목으로 채우려면
	//  GetTreeCtrl()을 호출하여 해당 tree 컨트롤을 직접 액세스하십시오.
[!endif]
[!if SCROLL_VIEW]
	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
[!endif]

}
[!endif]
[!if PRINTING]

// [!output VIEW_CLASS] 인쇄

BOOL [!output VIEW_CLASS]::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void [!output VIEW_CLASS]::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void [!output VIEW_CLASS]::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

[!if FORM_VIEW]

void [!output VIEW_CLASS]::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}
[!endif]
[!endif]


// [!output VIEW_CLASS] 진단

#ifdef _DEBUG
void [!output VIEW_CLASS]::AssertValid() const
{
	[!output VIEW_BASE_CLASS]::AssertValid();
}

[!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS([!output DOC_CLASS])));
	return ([!output DOC_CLASS]*)m_pDocument;
}
#endif //_DEBUG

[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
int [!output VIEW_CLASS]::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	int iResult = CView::OnCreate(lpCreateStruct);

	if(iResult == 0)
		SHDoneButton(TRUE);

	return iResult;
}
void [!output VIEW_CLASS]::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
}

void [!output VIEW_CLASS]::OnDestroy()
{
	CView::OnDestroy();
}

BOOL [!output VIEW_CLASS]::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
int [!output VIEW_CLASS]::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	int iResult = CView::OnCreate(lpCreateStruct);

	if(iResult == 0)
		SHDoneButton(TRUE);

	return iResult;
}
void [!output VIEW_CLASS]::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
}

void [!output VIEW_CLASS]::OnDestroy()
{
	CView::OnDestroy();
}

BOOL [!output VIEW_CLASS]::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif]

// [!output VIEW_CLASS] 메시지 처리기
