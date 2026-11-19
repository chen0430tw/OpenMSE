// [!output VIEW_IMPL] : implémentation de la classe [!output VIEW_CLASS]
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
	// Commandes d'impression standard
	ON_COMMAND(ID_FILE_PRINT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &[!output VIEW_BASE_CLASS]::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &[!output VIEW_BASE_CLASS]::OnFilePrintPreview)
[!endif]
END_MESSAGE_MAP()

// construction ou destruction de [!output VIEW_CLASS]

[!output VIEW_CLASS]::[!output VIEW_CLASS]()
[!if FORM_VIEW]
	: [!output VIEW_BASE_CLASS]([!output VIEW_CLASS]::IDD)
[!endif]
{
	// TODO : ajoutez ici du code de construction

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
	// TODO : changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs

	return [!output VIEW_BASE_CLASS]::PreCreateWindow(cs);
}

[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]

// dessin de [!output VIEW_CLASS]
void [!output VIEW_CLASS]::OnDraw(CDC* /*pDC*/)
{
	[!output DOC_CLASS]* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO : ajoutez ici le code de dessin pour les données natives
}
[!endif] 

[!if SCROLL_VIEW || LIST_VIEW || TREE_VIEW || FORM_VIEW]

void [!output VIEW_CLASS]::OnInitialUpdate()
{
	[!output VIEW_BASE_CLASS]::OnInitialUpdate();

[!if LIST_VIEW]
	// TODO : vous pouvez remplir votre contrôle ListView avec des éléments en accédant directement
	//  au contrôle List par un appel à GetListCtrl().
[!endif]
[!if TREE_VIEW]
	// TODO : vous pouvez remplir votre contrôle TreeView avec des éléments en accédant directement
	//  au contrôle Tree par un appel à GetTreeCtrl().
[!endif]
[!if SCROLL_VIEW]
	CSize sizeTotal;
	// TODO : calculez la taille totale de cette vue
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
[!endif]

}
[!endif]
[!if PRINTING]

// impression de [!output VIEW_CLASS]

BOOL [!output VIEW_CLASS]::OnPreparePrinting(CPrintInfo* pInfo)
{
	// TODO : ajoutez ici le code d'impression personnalisé
	// préparation par défaut
	return DoPreparePrinting(pInfo);
}

void [!output VIEW_CLASS]::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO : ajoutez une initialisation supplémentaire avant l'impression
}

void [!output VIEW_CLASS]::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO : ajoutez un nettoyage après l'impression
}

[!if FORM_VIEW]

void [!output VIEW_CLASS]::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO : ajoutez ici le code d'impression personnalisé
}
[!endif]
[!endif]


// diagnostics pour [!output VIEW_CLASS]

#ifdef _DEBUG
void [!output VIEW_CLASS]::AssertValid() const
{
	[!output VIEW_BASE_CLASS]::AssertValid();
}

[!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const // la version non déboguée est en ligne
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

// gestionnaires de messages pour [!output VIEW_CLASS]
