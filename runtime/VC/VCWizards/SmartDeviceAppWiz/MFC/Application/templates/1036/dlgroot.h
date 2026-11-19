// [!output APP_HEADER] : fichier d'en-tête principal pour l'application PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

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

// [!output APP_CLASS]:
// Consultez [!output APP_IMPL] pour l'implémentation de cette classe
//

class [!output APP_CLASS] : public [!output APP_BASE_CLASS]
{
public:
[!if HELPSTYLE_WINHELP]
	[!output APP_CLASS](LPCTSTR lpszHelpName);
[!else]
	[!output APP_CLASS]();
[!endif]
	
// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation

	DECLARE_MESSAGE_MAP()
};

extern [!output APP_CLASS] theApp;
