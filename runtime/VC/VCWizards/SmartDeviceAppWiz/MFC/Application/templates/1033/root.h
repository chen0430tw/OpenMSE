// [!output APP_HEADER] : main header file for the [!output PROJECT_NAME] application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

[!if !MULTIPLE_PLATFORM]
[!if STANDARDSHELL_UI_MODEL]
#include "resource.h"
[!endif]
[!if POCKETPC2003_UI_MODEL]
#include "resourceppc.h"
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#include "resourcesp.h"
[!endif]
[!if AYGSHELL_UI_MODEL]
#include "resourceayg.h"
[!endif]
[!else]
[!if STANDARDSHELL_UI_MODEL]
#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif
[!endif]
[!if POCKETPC2003_UI_MODEL]
#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#ifdef SMARTPHONE2003_UI_MODEL
#include "resourcesp.h"
#endif
[!endif]
[!if AYGSHELL_UI_MODEL]
#ifdef AYGSHELL_UI_MODEL
#include "resourceayg.h"
#endif
[!endif]
[!endif]

// [!output APP_CLASS]:
// See [!output APP_IMPL] for the implementation of this class
//

class [!output APP_CLASS] : public [!output APP_BASE_CLASS]
{
public:
[!if HELPSTYLE_WINHELP]
	[!output APP_CLASS]((LPCTSTR lpszHelpName));
[!else]
	[!output APP_CLASS]();
[!endif]

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
public:
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
	afx_msg void OnAppAbout();
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
	afx_msg void OnAppAbout();
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]

	DECLARE_MESSAGE_MAP()
};

extern [!output APP_CLASS] theApp;
