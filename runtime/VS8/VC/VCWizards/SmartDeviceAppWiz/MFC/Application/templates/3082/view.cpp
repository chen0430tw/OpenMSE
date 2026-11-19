// [!output VIEW_IMPL]: implementación de la clase [!output VIEW_CLASS]
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
	// Comandos de impresión estándar
	ON_COMMAND(ID_FILE_PRINT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &[!output VIEW_BASE_CLASS]::OnFilePrintPreview)
[!endif]
END_MESSAGE_MAP()

// Construcción o destrucción de [!output VIEW_CLASS]

[!output VIEW_CLASS]::[!output VIEW_CLASS]()
[!if FORM_VIEW]
	: [!output VIEW_BASE_CLASS]([!output VIEW_CLASS]::IDD)
[!endif]
{
	// TODO: agregar aquí el código de construcción

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
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return [!output VIEW_BASE_CLASS]::PreCreateWindow(cs);
}

[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]

// Dibujo de [!output VIEW_CLASS]
void [!output VIEW_CLASS]::OnDraw(CDC* /*pDC*/)
{
	[!output DOC_CLASS]* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: agregar aquí el código de dibujo para datos nativos
}
[!endif] 

[!if SCROLL_VIEW || LIST_VIEW || TREE_VIEW || FORM_VIEW]

void [!output VIEW_CLASS]::OnInitialUpdate()
{
	[!output VIEW_BASE_CLASS]::OnInitialUpdate();

[!if LIST_VIEW]
	// TODO: Puede rellenar ListView con elementos obteniendo acceso directamente
	//  a su control de lista mediante una llamada a GetListCtrl().
[!endif]
[!if TREE_VIEW]
	// TODO: puede rellenar TreeView con elementos obteniendo acceso directamente
	//  a través de una llamada a GetTreeCtrl().
[!endif]
[!if SCROLL_VIEW]
	CSize sizeTotal;
	// TODO: calcular el tamaño total de esta vista
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
[!endif]

}
[!endif]
[!if PRINTING]

// Impresión de [!output VIEW_CLASS]

BOOL [!output VIEW_CLASS]::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO: agregar aquí el código de impresión personalizado
	// preparación predeterminada
	return DoPreparePrinting(pInfo);
}

void [!output VIEW_CLASS]::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar inicialización adicional antes de imprimir
}

void [!output VIEW_CLASS]::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar limpieza después de imprimir
}

[!if FORM_VIEW]

void [!output VIEW_CLASS]::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar aquí el código de impresión personalizado
}
[!endif]
[!endif]


// Diagnósticos de [!output VIEW_CLASS]

#ifdef _DEBUG
void [!output VIEW_CLASS]::AssertValid() const
{
	[!output VIEW_BASE_CLASS]::AssertValid();
}

[!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const // La versión de no depuración es en línea
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

// Controladores de mensaje de [!output VIEW_CLASS]
