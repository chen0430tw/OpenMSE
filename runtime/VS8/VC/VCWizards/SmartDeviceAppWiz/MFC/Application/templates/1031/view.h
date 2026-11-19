// [!output VIEW_HEADER] : Schnittstelle der Klasse [!output VIEW_CLASS]
//


#pragma once

class [!output VIEW_CLASS] : public [!output VIEW_BASE_CLASS]
{
protected: // Nur aus Serialisierung erstellen
	[!output VIEW_CLASS]();
	DECLARE_DYNCREATE([!output VIEW_CLASS])
[!if FORM_VIEW]

public:
	enum{ IDD = IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_FORM };
[!endif]

// Attribute
public:
	[!output DOC_CLASS]* GetDocument() const;

// Vorgänge
public:

// Überschreibungen
public:
[!if !FORM_VIEW && !TREE_VIEW && !LIST_VIEW]
	virtual void OnDraw(CDC* pDC);  // Überschrieben, um diese Ansicht darzustellen
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
[!endif] 
[!if SCROLL_VIEW || FORM_VIEW || TREE_VIEW || LIST_VIEW]
	virtual void OnInitialUpdate(); // Erster Aufruf nach Erstellung
[!endif]
[!if PRINTING]
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
[!if FORM_VIEW]
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
[!endif]
[!endif]

// Implementierung
public:
	virtual ~[!output VIEW_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
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

#ifndef _DEBUG  // Debugversion in [!output VIEW_IMPL]
inline [!output DOC_CLASS]* [!output VIEW_CLASS]::GetDocument() const
   { return reinterpret_cast<[!output DOC_CLASS]*>(m_pDocument); }
#endif

