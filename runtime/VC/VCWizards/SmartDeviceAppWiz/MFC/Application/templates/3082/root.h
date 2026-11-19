// [!output APP_HEADER]: archivo de encabezado principal para la aplicación [!output PROJECT_NAME]
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
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
// Consulte la sección [!output APP_IMPL] para obtener información sobre la implementación de esta clase
//

class [!output APP_CLASS] : public [!output APP_BASE_CLASS]
{
public:
[!if HELPSTYLE_WINHELP]
	[!output APP_CLASS]((LPCTSTR lpszHelpName));
[!else]
	[!output APP_CLASS]();
[!endif]

// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación
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
