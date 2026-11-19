// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <stddef.h>     // for offsetof macro

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Find/FindReplace dialogs

CFindReplaceDialog::CFindReplaceDialog() : CCommonDialog(NULL)
{
	memset(&m_fr, 0, sizeof(m_fr));
	m_szFindWhat[0] = '\0';
	m_szReplaceWith[0] = '\0';

	m_fr.Flags = FR_ENABLEHOOK;
#ifndef _WIN32_WCE
	m_fr.lpfnHook = (COMMDLGPROC)_AfxCommDlgProc;
#else
	m_fr.lpfnHook =(LPFRHOOKPROC) AfxDlgProcEx;
#endif //_WIN32_WCE
	m_fr.lStructSize = sizeof(m_fr);
	m_fr.lpstrFindWhat = (LPTSTR)m_szFindWhat;
#ifdef _WIN32_WCE
	m_pFindReplaceBar=NULL;
	m_pFindDialogCB = NULL;
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel==PocketPC)
	{	//we show the replace bar only for PPC
		m_bShowBar =TRUE;
	}
	else
	{	//other platfroms don't provide any findreplace bar 
		m_bShowBar =FALSE;
	}
	m_bFindDlg =TRUE;
#endif //_WIN32_WCE
}

void CFindReplaceDialog::PostNcDestroy()
{
	ASSERT(m_hWnd == NULL);
	delete this;
}

BOOL CFindReplaceDialog::Create(BOOL bFindDialogOnly,
	LPCTSTR lpszFindWhat, LPCTSTR lpszReplaceWith,
	DWORD dwFlags, CWnd* pParentWnd)
{
	ASSERT_VALID(this);
	ASSERT(m_fr.Flags & FR_ENABLEHOOK);
	ASSERT(m_fr.lpfnHook != NULL);

	m_nIDHelp = bFindDialogOnly ? AFX_IDD_FIND : AFX_IDD_REPLACE;

	m_fr.wFindWhatLen = _countof(m_szFindWhat);
	m_fr.lpstrReplaceWith = (LPTSTR)m_szReplaceWith;
	m_fr.wReplaceWithLen = _countof(m_szReplaceWith);
	m_fr.Flags |= dwFlags;

	if (pParentWnd == NULL)
		m_fr.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	else
	{
		ASSERT_VALID(pParentWnd);
		m_fr.hwndOwner = pParentWnd->m_hWnd;
	}
	ASSERT(m_fr.hwndOwner != NULL); // must have a parent for modeless dialog

	if (lpszFindWhat != NULL)
		Checked::tcsncpy_s(m_szFindWhat, _countof(m_szFindWhat), lpszFindWhat, _TRUNCATE);

	if (lpszReplaceWith != NULL)
		Checked::tcsncpy_s(m_szReplaceWith, _countof(m_szReplaceWith), lpszReplaceWith, _TRUNCATE);

	HWND hWnd;

	AfxHookWindowCreate(this);
	if (bFindDialogOnly)
#ifndef _WIN32_WCE
		hWnd = ::AfxCtxFindText(&m_fr);
#else
	{
		m_bFindDlg = TRUE;
		hWnd = ::ce_FindText(&m_fr);
	}
#endif //!_WIN32_WCE
	else
#ifndef _WIN32_WCE
		hWnd = ::AfxCtxReplaceText(&m_fr);
#else
	{
		m_bFindDlg = FALSE;
		hWnd = ::ce_ReplaceText(&m_fr);
	}
#endif 
	if (!AfxUnhookWindowCreate())
		PostNcDestroy();

	ASSERT(hWnd == NULL || hWnd == m_hWnd);
	return hWnd != NULL;
}

CFindReplaceDialog* PASCAL CFindReplaceDialog::GetNotifier(LPARAM lParam)
{
	ASSERT(lParam != NULL);
	CFindReplaceDialog* pDlg;

	pDlg = (CFindReplaceDialog*)(lParam - offsetof(CFindReplaceDialog, m_fr));
	ASSERT_VALID(pDlg);
	ASSERT_KINDOF(CFindReplaceDialog, pDlg);

	return pDlg;
}

////////////////////////////////////////////////////////////////////////////
// CFindReplaceDialog diagnostics

#ifdef _DEBUG
#ifndef _WIN32_WCE
void CFindReplaceDialog::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);

	dc << "m_fr.hwndOwner = " << m_fr.hwndOwner;
	dc << "\nm_fr.Flags = ";
	dc.DumpAsHex(m_fr.Flags);
	dc << "\nm_fr.lpstrFindWhat = " << m_fr.lpstrFindWhat;
	dc << "\nm_fr.lpstrReplaceWith = " << m_fr.lpstrReplaceWith;

	if (m_fr.lpfnHook == (COMMDLGPROC)_AfxCommDlgProc)
		dc << "\nhook function set to standard MFC hook function";
	else
		dc << "\nhook function set to non-standard hook function";

	dc << "\n";
}
#endif //!_WIN32_WCE
#endif //_DEBUG

#ifdef _WIN32_WCE
// The code in rest of the file is CE specific
class CFindReplaceBar : public CDialogBar
{
// Construction
public:
	CFindReplaceBar(CFindReplaceDialog* pDlg);

// Attributes
public:

// Operations
public:
//virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindReplaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFindReplaceBar();

protected:
	CFindReplaceDialog* m_pFindReplaceDlg;
// Generated message map functions
protected:
	//{{AFX_MSG(CFindReplaceBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnFind();
	afx_msg void OnOK();
	afx_msg void OnReplace();
	afx_msg void OnReplaceAll();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFindReplaceDialog message handlers

BEGIN_MESSAGE_MAP(CFindReplaceDialog, CCommonDialog)
	//{{AFX_MSG_MAP(CFindReplaceDialog)
	ON_COMMAND(ID_BUTTON1, &CFindReplaceDialog::OnReplace)
	ON_COMMAND(ID_BUTTON2, &CFindReplaceDialog::OnReplaceAll)
	ON_COMMAND(ID_BUTTON_HELP, &CFindReplaceDialog::OnHelp)
	ON_CONTROL(EN_CHANGE,  ID_EDIT1, &CFindReplaceDialog::OnFindTextChanged)
	ON_WM_CLOSE() 
	ON_COMMAND(ID_BUTTON_FIND, &CFindReplaceDialog::OnFind)
	ON_MESSAGE( WM_FINDREPLACEBAR, &CFindReplaceDialog::OnFindReplaceBar )
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFindReplaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CFindReplaceDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC) 
	{
		if(IsWindowVisible())
			CDialog::OnSettingChange(uFlags, lpszSection);
		else
		{
			CWnd* pTopLevelParentWnd = (CWnd::FromHandle(m_fr.hwndOwner))->GetTopLevelParent();
			ASSERT(pTopLevelParentWnd != NULL);
			ASSERT(::IsWindow(pTopLevelParentWnd->m_hWnd));
			::SendMessage(pTopLevelParentWnd->m_hWnd, WM_SETTINGCHANGE, uFlags, (LPARAM)lpszSection);
		}
	}
	else
	{
		CCommonDialog::OnSettingChange(uFlags, lpszSection);
	}
}

void CFindReplaceDialog::OnFind() 
{
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC) 
	{
		// Check this is a valid call
		ASSERT_VALID(this);

		if (!UpdateData(TRUE))
		{
			TRACE0("UpdateData failed during dialog termination.\n");
			// the UpdateData routine will set focus to correct item
			return;
		}

		// Prepare m_fr for parent notification
		m_fr.Flags = FR_ENABLEHOOK | FR_FINDNEXT;

		// Get 'Find what' value
		GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat, 
			m_fr.wFindWhatLen);

		// If find/replace dialog, get 'Replace with' value
		if (m_nIDHelp == AFX_IDD_REPLACE)
			GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
			m_fr.wReplaceWithLen);

		// Get state of 'Match whole word' check box
		if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
			m_fr.Flags |= FR_WHOLEWORD;

		// Get state of 'Match case' check box
		if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
			m_fr.Flags |= FR_MATCHCASE;

		// Get 'Direction' state
		m_fr.Flags |= FR_DOWN;
		if (m_nIDHelp == AFX_IDD_FIND)
		{
			if (IsDlgButtonChecked(ID_RADIO1))
				m_fr.Flags &= ~FR_DOWN; // "up" direction (for Find dialog only)
		}

		// create the find/replace bar if CFindReplaceDialog still exists when returning from parent window
		m_bShowBar = TRUE;
		CWnd* pBtnWnd = GetDlgItem(ID_BUTTON1);
		if(pBtnWnd == NULL)
			PostMessage(WM_FINDREPLACEBAR, _AFXCE_FINDBAR);
		else
			if(pBtnWnd->IsWindowVisible() && pBtnWnd->IsWindowEnabled())
				PostMessage(WM_FINDREPLACEBAR);
			else
				PostMessage(WM_FINDREPLACEBAR, _AFXCE_FINDBAR);

		// Notify parent window of user input to find replace dialog
		::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
			0, (LPARAM)&m_fr);
	}
}

BOOL CFindReplaceDialog::OnInitDialog()
{
	int aygshellUIModel = AfxGetAygshellUIModel();
	if((aygshellUIModel == PocketPC) || (aygshellUIModel == Smartphone))
	{
		CDialog::OnInitDialog();
		
		m_pFindDialogCB = new CCommandBar;
		if( m_pFindDialogCB != NULL)
		{
			((CCommandBar*) m_pFindDialogCB)->CreateEx(this);
			//in case of smartphone insert a menubar in the commandbar
			if(aygshellUIModel == Smartphone)
			{
				if(m_bFindDlg)
				{
					if(!((CCommandBar*) m_pFindDialogCB)->InsertMenuBar(AFX_IDR_FIND_SHMENU_SP))
						TRACE0("MenuBar insertion failed for find dialog.\n");
				}
				else
				{
					if(!((CCommandBar*) m_pFindDialogCB)->InsertMenuBar(AFX_IDR_REPLACE_SHMENU_SP))
						TRACE0("MenuBar insertion failed for replace dialog.\n");
				}
			}
		}
		return TRUE;
	}
	else
	{
		return CCommonDialog::OnInitDialog();
	}
}

void CFindReplaceDialog::OnOK() 
{
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC) 
	{
		if(!GetDlgItem(ID_BUTTON_FIND)->IsWindowEnabled())
		{
			CFindReplaceDialog::OnClose();
			return;
		}

		if (m_nIDHelp == AFX_IDD_FIND)	
			CFindReplaceDialog::OnFind();
		else
			CFindReplaceDialog::OnReplace();
	}
	else
	{
		// Check this is a valid call
		ASSERT_VALID(this);

		if (!UpdateData(TRUE))
		{
			TRACE0("UpdateData failed during dialog termination.\n");
			// the UpdateData routine will set focus to correct item
			return;
		}

		// Prepare m_fr for parent notification
		m_fr.Flags = FR_ENABLEHOOK | FR_FINDNEXT;

		// Get 'Find what' value
		GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat, 
			m_fr.wFindWhatLen);

		// If find/replace dialog, get 'Replace with' value
		if (m_nIDHelp == AFX_IDD_REPLACE)
			GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
			m_fr.wReplaceWithLen);

		// Get state of 'Match whole word' check box
		if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
			m_fr.Flags |= FR_WHOLEWORD;

		// Get state of 'Match case' check box
		if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
			m_fr.Flags |= FR_MATCHCASE;

		// Get 'Direction' state
		m_fr.Flags |= FR_DOWN;
		if (m_nIDHelp == AFX_IDD_FIND)
		{
			if (IsDlgButtonChecked(ID_RADIO1))
				m_fr.Flags &= ~FR_DOWN; // "up" direction (for Find dialog only)
		}

		// Notify parent window of user input to find replace dialog
		::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
			0, (LPARAM)&m_fr);
	}
}

LRESULT CFindReplaceDialog::OnFindReplaceBar(WPARAM wParam, LPARAM /*lParam*/)
{
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC) 
	{
	
	 	if(m_pFindReplaceBar == NULL && m_bShowBar)
		{
			m_pFindReplaceBar = new CFindReplaceBar(this);
			if(m_pFindReplaceBar == NULL)
			{
				CFindReplaceDialog::OnClose();
				return 0;
			}
			ShowWindow(FALSE);
			m_pFindReplaceBar->m_bAutoDelete = TRUE;

			if(m_pFindDialogCB != NULL)
			{
				m_pFindDialogCB->DestroyWindow();
				delete m_pFindDialogCB;
				m_pFindDialogCB = NULL;
			}
			
			CWnd* pTopLevelParentWnd = (CWnd::FromHandle(m_fr.hwndOwner))->GetTopLevelParent();
			ASSERT(pTopLevelParentWnd != NULL);
				
			m_pFindReplaceBar->Create(pTopLevelParentWnd, _T("WCE_FINDREPLACEBAR"), CBRS_BOTTOM, 0xE821);
			
			if(pTopLevelParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
			{
				((CFrameWnd*)pTopLevelParentWnd)->RecalcLayout(FALSE);
				CView* pView = ((CFrameWnd*)pTopLevelParentWnd)->GetActiveView();
				if(pView->IsKindOf(RUNTIME_CLASS(CEditView)))
				{
					CEdit &edit = ((CEditView*)pView)->GetEditCtrl();
					int nStartChar, nEndChar;
					edit.GetSel(nStartChar, nEndChar);
					edit.SetSel(nStartChar, nEndChar);
				}
			}
			else
			{
				CRect rect;
				pTopLevelParentWnd->GetClientRect(&rect);
				CRect barRect;
				m_pFindReplaceBar->GetWindowRect(&barRect);
				rect.top = rect.bottom - barRect.bottom + barRect.top; 
				m_pFindReplaceBar->MoveWindow(rect, TRUE);
				m_pFindReplaceBar->ShowWindow(SW_SHOW);
				m_pFindReplaceBar->UpdateWindow();
			}

			if(wParam == _AFXCE_FINDBAR)
			{
				(m_pFindReplaceBar->GetDlgItem(ID_BUTTON1))->EnableWindow(FALSE);
				(m_pFindReplaceBar->GetDlgItem(ID_BUTTON2))->EnableWindow(FALSE);
			}
			else
			{
				(m_pFindReplaceBar->GetDlgItem(ID_BUTTON1))->EnableWindow(TRUE);
				(m_pFindReplaceBar->GetDlgItem(ID_BUTTON2))->EnableWindow(TRUE);
			}
			pTopLevelParentWnd->SetFocus();

			return 1;
		}

		return 0;
	}
	else
	{	//simply return for other platfroms
		return 0;
	}
}

void CFindReplaceDialog::OnClose() 
{
	ASSERT_VALID(this);

	// Do not modify the flags, except to add
	// FR_DIALOGTERM
	m_fr.Flags |= FR_DIALOGTERM;

	// Delete other flags which may be left from
	// the last button press
	if ((m_fr.Flags & FR_FINDNEXT) == FR_FINDNEXT)
		m_fr.Flags &= ~FR_FINDNEXT;
	if ((m_fr.Flags & FR_REPLACE) == FR_REPLACE)
		m_fr.Flags &= ~FR_REPLACE;
	if ((m_fr.Flags & FR_REPLACEALL) == FR_REPLACEALL)
		m_fr.Flags &= ~FR_REPLACEALL;

	// Notify parent window of user input to find replace dialog
	// NOTE: This activates CEditView::OnFindReplaceCmd(), which
	//       in turn calls CFindReplaceDialog::GetNotifier().
	//       
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);

	int aygshellUIModel = AfxGetAygshellUIModel();
	if((aygshellUIModel == PocketPC) || (aygshellUIModel == Smartphone))
	{
		if(m_pFindDialogCB != NULL)
		{
			m_pFindDialogCB->DestroyWindow();
			delete m_pFindDialogCB;
			m_pFindDialogCB = NULL;
		}


		if((aygshellUIModel == PocketPC) && (m_pFindReplaceBar))
		{
			m_pFindReplaceBar->ShowWindow(FALSE);
			::SetFocus(m_fr.hwndOwner);
			CFrameWnd* pFrameWnd = m_pFindReplaceBar->GetTopLevelFrame();
			if(pFrameWnd != NULL)
				pFrameWnd->RecalcLayout(TRUE); 
			m_pFindReplaceBar->DestroyWindow();
			m_pFindReplaceBar = NULL;
		}
	}
	
	EndDialog(IDCANCEL);
}

void CFindReplaceDialog::OnCancel() 
{
	CFindReplaceDialog::OnClose();
}

void CFindReplaceDialog::EndDialog(int nEndID)
{
	ASSERT_VALID(this);

	m_nModalResult = nEndID;
	// For modeless, call DestroyWindow().
	// NOTE: For modal, we would have called PostMessage(WM_COMMAND, IDCANCEL);
	DestroyWindow();
}


void CFindReplaceDialog::OnReplace()
{
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed (psh1).\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	// Prepare m_fr for parent notification
	m_fr.Flags = FR_ENABLEHOOK | FR_REPLACE | FR_DOWN;

	// Get 'Find what' value
	GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat,
		m_fr.wFindWhatLen);

	// Get 'Replace with' value
	GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
		m_fr.wReplaceWithLen);

	// Get state of 'Match whole word' check box
	if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
		m_fr.Flags |= FR_WHOLEWORD;

	// Get state of 'Match case' check box
	if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
		m_fr.Flags |= FR_MATCHCASE;

	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC) 
	{
		// create the find/replace bar if CFindReplaceDialog still exists when returning from parent window
		m_bShowBar = TRUE;
		PostMessage(WM_FINDREPLACEBAR);
	}
	
	// Notify parent window of user input to find replace dialog
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);
}

void CFindReplaceDialog::OnReplaceAll()
{
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed (psh1).\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	// Prepare m_fr for parent notification
	m_fr.Flags = FR_ENABLEHOOK | FR_REPLACEALL | FR_DOWN;

	// Get 'Find what' value
	GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat,
		m_fr.wFindWhatLen);

	// Get 'Replace with' value
	GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
		m_fr.wReplaceWithLen);

	// Get state of 'Match whole word' check box
	if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
		m_fr.Flags |= FR_WHOLEWORD;

	// Get state of 'Match case' check box
	if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
		m_fr.Flags |= FR_MATCHCASE;

	// Notify parent window of user input to find replace dialog
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);

	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC)
	{ 
		// close the find/replace dialog
		// first check if it was not already destroyed because the replace failed
		if(::IsWindow(m_hWnd))
			CFindReplaceDialog::OnClose();
	}
}

void CFindReplaceDialog::OnHelp()
{
	SendMessage(m_nIDHelp);
}

void CFindReplaceDialog::OnFindTextChanged()
{
	CString strFind;

	GetDlgItemText(ID_EDIT1, strFind);
	BOOL bEnable = !strFind.IsEmpty();
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC)
	{
		GetDlgItem(ID_BUTTON_FIND)->EnableWindow(bEnable);
	}
	else 
	{
		if(aygshellUIModel != Smartphone)
		{	//for non smartphone platfroms disable/enable the ok/next button
			GetDlgItem(IDOK)->EnableWindow(bEnable);
		}
		else
		{	//for smartphone disable/enable the menu item.
			ASSERT(m_pFindDialogCB != NULL);
			HWND hMenuMB = m_pFindDialogCB->m_hCommandBar;
			HMENU hMenu = (HMENU) ::SendMessage(hMenuMB, SHCMBM_GETSUBMENU,0,ID_MENU_SP);
			CMenu *submenu = CMenu::FromHandle(hMenu);
			//chnage the state of the menu item
			if(bEnable)
				submenu->EnableMenuItem(IDOK,MF_BYCOMMAND|MF_ENABLED);
			else
				submenu->EnableMenuItem(IDOK,MF_BYCOMMAND|MF_DISABLED | MF_GRAYED);
		}
	}

	if(aygshellUIModel != Smartphone)
	{
		if (m_nIDHelp != AFX_IDD_FIND)
		{
			GetDlgItem(ID_BUTTON1)->EnableWindow(bEnable);
			GetDlgItem(ID_BUTTON2)->EnableWindow(bEnable);
		}
	}
	else
	{	//for smartphone enable/disable menu entries for Replace and Replace all
		ASSERT(m_pFindDialogCB != NULL);
		HWND hMenuMB = this->m_pFindDialogCB->m_hCommandBar;
		HMENU hMenu = (HMENU) ::SendMessage(hMenuMB, SHCMBM_GETSUBMENU,0,ID_MENU_SP);
		CMenu *submenu = CMenu::FromHandle(hMenu);
		//chnage the state of the menu item
		if(bEnable)
		{
			submenu->EnableMenuItem(ID_BUTTON1,MF_BYCOMMAND|MF_ENABLED);
			submenu->EnableMenuItem(ID_BUTTON2,MF_BYCOMMAND|MF_ENABLED);
		}
		else
		{
			submenu->EnableMenuItem(ID_BUTTON1,MF_BYCOMMAND|MF_DISABLED | MF_GRAYED);
			submenu->EnableMenuItem(ID_BUTTON2,MF_BYCOMMAND|MF_DISABLED | MF_GRAYED);
		}
	}
}


HWND AFXAPI ce_FindText(LPFINDREPLACE lpfr)
{
	// Create a modeless dialog.
	HWND hwnd;
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC)
	{
		HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_FINDDLG_PPC"), RT_DIALOG );
		if(!hInst)
			return (NULL);
		hwnd = ::CreateDialog(hInst, _T("WCE_FINDDLG_PPC"), lpfr->hwndOwner, 
			            (DLGPROC)lpfr->lpfnHook );
	}
	else if( aygshellUIModel == Smartphone)
	{
		HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_FINDDLG_SP"), RT_DIALOG );
		if(!hInst)
			return (NULL);
		hwnd = ::CreateDialog(hInst, _T("WCE_FINDDLG_SP"), lpfr->hwndOwner, 
			            (DLGPROC)lpfr->lpfnHook );
	}
	else
	{
		HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_FINDDLG"), RT_DIALOG );
		if(!hInst)
			return (NULL);
		hwnd = ::CreateDialog(hInst, _T("WCE_FINDDLG"), lpfr->hwndOwner, 
			            (DLGPROC)lpfr->lpfnHook );
	}

	// Check it has been created successfully
	ASSERT(hwnd != NULL);

	// Set the value for the 'Find what' edit box
	if (SetWindowText(
		GetDlgItem(hwnd, ID_EDIT1),	// Handle of ID_EDIT1
		lpfr->lpstrFindWhat))
		TRACE0("Setting ID_EDIT1 successful\n");

	// Hide/Show 'Whole word only' check box
	if (lpfr->Flags & FR_HIDEWHOLEWORD) 
		ShowWindow(GetDlgItem(hwnd, ID_CHECK1), SW_HIDE);
	else
	{
		// Disable/Enable 'Whole word only' check box
		if (lpfr->Flags & FR_NOWHOLEWORD) 
			EnableWindow(GetDlgItem(hwnd, ID_CHECK1), FALSE);
		else
			ShowWindow(GetDlgItem(hwnd, ID_CHECK1), SW_SHOW);
	}

	// Hide/Show 'Match case' check box
	if (lpfr->Flags & FR_HIDEMATCHCASE) 
		ShowWindow(GetDlgItem(hwnd, ID_CHECK2), SW_HIDE);
	else
	{
		// Disable/Enable 'Match case' check box
		if (lpfr->Flags & FR_NOMATCHCASE) 
			EnableWindow(GetDlgItem(hwnd, ID_CHECK2), FALSE);
		else
			ShowWindow(GetDlgItem(hwnd, ID_CHECK2), SW_SHOW);
	}

	// Check Up/Down radio button
	if (lpfr->Flags & FR_DOWN) 
	{
		SendMessage(GetDlgItem(hwnd, ID_RADIO1), BM_SETCHECK, 0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RADIO2), BM_SETCHECK, 1, 0);
	}
	else
	{
		SendMessage(GetDlgItem(hwnd, ID_RADIO1), BM_SETCHECK, 1, 0);
		SendMessage(GetDlgItem(hwnd, ID_RADIO2), BM_SETCHECK, 0, 0);
	}

	// Hide/Show 'Direction' radio buttons
	if (lpfr->Flags & FR_HIDEUPDOWN) 
	{
		ShowWindow(GetDlgItem(hwnd, ID_GROUP1), SW_HIDE);
		ShowWindow(GetDlgItem(hwnd, ID_RADIO1), SW_HIDE);
		ShowWindow(GetDlgItem(hwnd, ID_RADIO2), SW_HIDE);
	}
	else
	{
		// Disable/Enable 'Direction radio buttons'
		if (lpfr->Flags & FR_NOUPDOWN) 
		{
			EnableWindow(GetDlgItem(hwnd, ID_GROUP1), FALSE);
			EnableWindow(GetDlgItem(hwnd, ID_RADIO1), FALSE);
			EnableWindow(GetDlgItem(hwnd, ID_RADIO2), FALSE);
		}
		else
		{
			ShowWindow(GetDlgItem(hwnd, ID_GROUP1), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, ID_RADIO1), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, ID_RADIO2), SW_SHOW);
		}
	}

	// Show/Hide Help button
	if (lpfr->Flags & FR_SHOWHELP) 
		ShowWindow(GetDlgItem(hwnd, ID_BUTTON_HELP), SW_SHOW);
	else
	{	//in case of Smartphone we don't have buttons
		if(aygshellUIModel !=Smartphone)
		{
			ShowWindow(GetDlgItem(hwnd, ID_BUTTON_HELP), SW_HIDE);
		}
	}
	
	SetFocus(GetDlgItem(hwnd, ID_EDIT1));

	// Show find dialog
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}


HWND AFXAPI ce_ReplaceText(LPFINDREPLACE lpfr)
{
	// Create a modeless dialog.
	HWND hWnd;
	int aygshellUIModel = AfxGetAygshellUIModel();
	if(aygshellUIModel == PocketPC) 
	{
		HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_REPLACEDLG_PPC"), RT_DIALOG );
		if(!hInst)
			return (NULL);
		hWnd = ::CreateDialog(hInst, _T("WCE_REPLACEDLG_PPC"), lpfr->hwndOwner, 
			            (DLGPROC)lpfr->lpfnHook );
	}
	else if (aygshellUIModel == Smartphone)
	{
		HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_REPLACEDLG_SP"), RT_DIALOG );
		if(!hInst)
			return (NULL);
		hWnd = ::CreateDialog(hInst, _T("WCE_REPLACEDLG_SP"), lpfr->hwndOwner, 
			            (DLGPROC)lpfr->lpfnHook );
	}
	else
	{
		HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_REPLACEDLG"), RT_DIALOG );
		if(!hInst)
			return (NULL);
		 hWnd = ::CreateDialog(hInst, _T("WCE_REPLACEDLG"), lpfr->hwndOwner, 
		          	     (DLGPROC)lpfr->lpfnHook );
	}
	
	// Check it has been created successfully
	ASSERT(hWnd != NULL);

	// Set the value for the 'Find what' edit box
	if (SetWindowText(
		GetDlgItem(hWnd, ID_EDIT1),	// Handle of ID_EDIT1
		lpfr->lpstrFindWhat))
		TRACE0("Setting ID_EDIT1 successful\n");

	// Set the value for the 'Replace with' edit box
	if (SetWindowText(
		GetDlgItem(hWnd, ID_EDIT2),	// Handle of ID_EDIT2
		lpfr->lpstrReplaceWith))
		TRACE0("Setting ID_EDIT2 successful\n");

	// Hide/Show 'Whole word only' check box
	if ((lpfr->Flags & FR_HIDEWHOLEWORD) == FR_HIDEWHOLEWORD)
		ShowWindow(GetDlgItem(hWnd, ID_CHECK1), SW_HIDE);
	else
	{
		// Disable/Enable 'Whole word only' check box
		if ((lpfr->Flags & FR_NOWHOLEWORD) == FR_NOWHOLEWORD)
			EnableWindow(GetDlgItem(hWnd, ID_CHECK1), FALSE);
		else
			ShowWindow(GetDlgItem(hWnd, ID_CHECK1), SW_SHOW);
	}

	// Hide/Show 'Match case' check box
	if ((lpfr->Flags & FR_HIDEMATCHCASE) == FR_HIDEMATCHCASE)
		ShowWindow(GetDlgItem(hWnd, ID_CHECK2), SW_HIDE);
	else
	{
		// Disable/Enable 'Match case' check box
		if ((lpfr->Flags & FR_NOMATCHCASE) == FR_NOMATCHCASE)
			EnableWindow(GetDlgItem(hWnd, ID_CHECK2), FALSE);
		else
			ShowWindow(GetDlgItem(hWnd, ID_CHECK2), SW_SHOW);
	}

	// Show/Hide Help button
	if ((lpfr->Flags & FR_SHOWHELP) == FR_SHOWHELP)
		ShowWindow(GetDlgItem(hWnd, ID_BUTTON_HELP), SW_SHOW);
	else
	{	//in case of Smartphone we don't have buttons
		if(aygshellUIModel != Smartphone)
		{
			ShowWindow(GetDlgItem(hWnd, ID_BUTTON_HELP), SW_HIDE);
		}
	}
	
	// Show find/replace dialog
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return hWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CFindReplaceBar

CFindReplaceBar::CFindReplaceBar(CFindReplaceDialog* pDlg)
{
	m_pFindReplaceDlg = pDlg;
}

CFindReplaceBar::~CFindReplaceBar()
{
}


BEGIN_MESSAGE_MAP(CFindReplaceBar, CDialogBar)
	//{{AFX_MSG_MAP(CFindReplaceBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_BUTTON_FIND, &CFindReplaceBar::OnFind)
	ON_COMMAND(ID_BUTTON1, &CFindReplaceBar::OnReplace)
	ON_COMMAND(ID_BUTTON2, &CFindReplaceBar::OnReplaceAll)
	ON_COMMAND(IDOK, &CFindReplaceBar::OnOK)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFindReplaceBar message handlers
void CFindReplaceBar::OnFind() 
{
	m_pFindReplaceDlg->OnFind();
}

void CFindReplaceBar::OnOK() 
{
	m_pFindReplaceDlg->OnClose(); //rely on this one to do the work!!!
}

void CFindReplaceBar::OnReplace() 
{
	m_pFindReplaceDlg->OnReplace();	
}

void CFindReplaceBar::OnReplaceAll() 
{
	m_pFindReplaceDlg->OnReplaceAll();
}

void CFindReplaceBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL /*bDisableIfNoHndler*/)
{
	UpdateDialogControls(pTarget, FALSE);
}

#endif //_WIN32_WCE

IMPLEMENT_DYNAMIC(CFindReplaceDialog, CDialog)

////////////////////////////////////////////////////////////////////////////
