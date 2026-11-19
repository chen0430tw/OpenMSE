// [!output VIEW_IMPL] : [!output VIEW_CLASS] 類別的實作
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
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &[!output VIEW_BASE_CLASS]::OnFilePrintPreview)
[!endif]
END_MESSAGE_MAP()

// [!output VIEW_CLASS] 建構/解構

[!output VIEW_CLASS]::[!output VIEW_CLASS]()
[!if FORM_VIEW]
	: [!output VIEW_BASE_CLASS]([!output VIEW_CLASS]::IDD)
[!endif]
{
	// TODO: 在此加入建構程式碼

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
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return [!output VIEW_BASE_CLASS]::PreCreateWindow(cs);
}

[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]

// [!output VIEW_CLASS] 描繪
void [!output VIEW_CLASS]::OnDraw(CDC* /*pDC*/)
{
	[!output DOC_CLASS]* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 在此加入原生資料的描繪程式碼
}
[!endif] 

[!if SCROLL_VIEW || LIST_VIEW || TREE_VIEW || FORM_VIEW]

void [!output VIEW_CLASS]::OnInitialUpdate()
{
	[!output VIEW_BASE_CLASS]::OnInitialUpdate();

[!if LIST_VIEW]
	// TODO: 您可以透過對 GetListCtrl() 的呼叫，
	// 直接存取其清單控制項，使用項目填入您的 ListView。
[!endif]
[!if TREE_VIEW]
	// TODO: 您可以透過對 GetTreeCtrl() 的呼叫，
	// 直接存取其樹狀控制項，使用項目填入您的 TreeView。
[!endif]
[!if SCROLL_VIEW]
	CSize sizeTotal;
	// TODO: 計算此檢視的總大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
[!endif]

}
[!endif]
[!if PRINTING]

// [!output VIEW_CLASS] 列印

BOOL [!output VIEW_CLASS]::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO: 在此加入自訂的列印程式碼
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void [!output VIEW_CLASS]::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void [!output VIEW_CLASS]::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}

[!if FORM_VIEW]

void [!output VIEW_CLASS]::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此加入自訂的列印程式碼
}
[!endif]
[!endif]


// [!output VIEW_CLASS] 診斷

#ifdef _DEBUG
void [!output VIEW_CLASS]::AssertValid() const
{
	[!output VIEW_BASE_CLASS]::AssertValid();
}

[!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const // 內嵌非偵錯版本
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

// [!output VIEW_CLASS] 訊息處理常式
