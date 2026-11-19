// [!output VIEW_IMPL] : [!output VIEW_CLASS] 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &[!output VIEW_BASE_CLASS]::OnFilePrintPreview)
[!endif]
END_MESSAGE_MAP()

// [!output VIEW_CLASS] 构造/析构

[!output VIEW_CLASS]::[!output VIEW_CLASS]()
[!if FORM_VIEW]
	: [!output VIEW_BASE_CLASS]([!output VIEW_CLASS]::IDD)
[!endif]
{
	// TODO: 在此处添加构造代码

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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return [!output VIEW_BASE_CLASS]::PreCreateWindow(cs);
}

[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]

// [!output VIEW_CLASS] 绘制
void [!output VIEW_CLASS]::OnDraw(CDC* /*pDC*/)
{
	[!output DOC_CLASS]* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 在此处为本机数据添加绘制代码
}
[!endif] 

[!if SCROLL_VIEW || LIST_VIEW || TREE_VIEW || FORM_VIEW]

void [!output VIEW_CLASS]::OnInitialUpdate()
{
	[!output VIEW_BASE_CLASS]::OnInitialUpdate();

[!if LIST_VIEW]
	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
[!endif]
[!if TREE_VIEW]
	// TODO: 调用 GetTreeCtrl() 直接访问 TreeView 的树控件，
	//  从而可以用项填充 TreeView。
[!endif]
[!if SCROLL_VIEW]
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
[!endif]

}
[!endif]
[!if PRINTING]

// [!output VIEW_CLASS] 打印

BOOL [!output VIEW_CLASS]::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO: 在此处添加自定义打印代码
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void [!output VIEW_CLASS]::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void [!output VIEW_CLASS]::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}

[!if FORM_VIEW]

void [!output VIEW_CLASS]::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此处添加自定义打印代码
}
[!endif]
[!endif]


// [!output VIEW_CLASS] 诊断

#ifdef _DEBUG
void [!output VIEW_CLASS]::AssertValid() const
{
	[!output VIEW_BASE_CLASS]::AssertValid();
}

[!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const // 非调试版本是内联的
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

// [!output VIEW_CLASS] 消息处理程序
