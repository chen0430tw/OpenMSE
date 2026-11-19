// stdafx.h : file di inclusione per file di inclusione del sistema standard
// o file di inclusione specifici del progetto utilizzati di frequente,
// ma modificati di rado.

#pragma once

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

// NOTA: questo valore non è strettamente correlato alla versione Windows CE OS di destinazione
#define WINVER _WIN32_WCE

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Escludere dalle intestazioni Windows gli elementi utilizzati di rado.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// alcuni costruttori CString saranno espliciti.
#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif

// disattiva l'operazione delle classi MFC che consiste nel nascondere alcuni comuni messaggi di avviso che vengono spesso ignorati.
#define _AFX_ALL_WARNINGS

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#include <afxwin.h>         // componenti MFC di base e standard
#include <afxext.h>         // estensioni MFC

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// Supporto MFC per controlli comuni di Internet Explorer 4
#endif

[!if TREE_VIEW || LIST_VIEW]
#include <afxcview.h>
[!endif]

[!if ACTIVEX_CONTROLS]
#include <afxdisp.h>        // classi di automazione MFC
[!endif]

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Supporto MFC per controlli comuni di Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

[!if SOCKETS]
#include <afxsock.h>		// estensioni socket MFC
[!endif] 

[!if ATL_SUPPORT]
#include <atlbase.h>
extern CComModule _Module;
[!endif]

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

[!if !MULTIPLE_PLATFORM]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 
[!endif]
[!else]
[!if POCKETPC2003_UI_MODEL || SMARTPHONE2003_UI_MODEL]
#ifdef SHELL_AYGSHELL
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib") 
#endif // SHELL_AYGSHELL
[!endif]
[!endif]

#if (_WIN32_WCE < 0x500) && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#include <altcecrt.h>
