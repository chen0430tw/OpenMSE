// [!output VIEW_HEADER] : interface of the [!output VIEW_CLASS] class
//


#pragma once

class [!output VIEW_CLASS] : public [!output VIEW_BASE_CLASS]
{
protected: // create from serialization only
	[!output VIEW_CLASS]();
	DECLARE_DYNCREATE([!output VIEW_CLASS])
[!if FORM_VIEW]

public:
	enum{ IDD = IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_FORM };
[!endif]

// Attributes
public:
	[!output DOC_CLASS]* GetDocument() const;

// Operations
public:

// Overrides
public:
[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
[!endif] 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif] 

protected:
[!if FORM_VIEW]
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
[!endif] 
[!if SCROLL_VIEW || FORM_VIEW || TREE_VIEW || LIST_VIEW]
	virtual void OnInitialUpdate(); // called first time after construct
[!endif]
[!if PRINTING]
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
[!if FORM_VIEW]
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
[!endif]
[!endif]

// Implementation
public:
	virtual ~[!output VIEW_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Generated message map functions
protected:
[!if APP_TYPE_SDI_DOCLIST]
[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL]
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnDestroy();
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL]
#ifdef WIN32_PLATFORM_PSPC
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnDestroy();
#endif // WIN32_PLATFORM_PSPC
[!endif]
[!endif]
[!endif]
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in [!output VIEW_IMPL]
inline [!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const
   { return reinterpret_cast<[!output DOC_CLASS]*>(m_pDocument); }
#endif

