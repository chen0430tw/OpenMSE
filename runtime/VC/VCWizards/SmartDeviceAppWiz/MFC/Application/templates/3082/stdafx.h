// stdafx.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos del proyecto utilizados frecuentemente,
// pero cambiados rara vez

#pragma once

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

// NOTA: este valor no está convenientemente correlacionado con la versión del sistema operativo Windows CE de destino
#define WINVER _WIN32_WCE

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Excluir material rara vez utilizado de encabezados de Windows
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// Algunos constructores CString serán explícitos
#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif

// Desactiva la ocultación de MFC para algunos mensajes de advertencia comunes y, muchas veces, omitidos de forma consciente
#define _AFX_ALL_WARNINGS

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#include <afxwin.h>         // Componentes principales y estándar de MFC
#include <afxext.h>         // Extensiones de MFC

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// Compatibilidad MFC para controles comunes de Internet Explorer 4
#endif

[!if TREE_VIEW || LIST_VIEW]
#include <afxcview.h>
[!endif]

[!if ACTIVEX_CONTROLS]
#include <afxdisp.h>        // Clases de automatización de MFC
[!endif]

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Compatibilidad MFC para controles comunes de Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

[!if SOCKETS]
#include <afxsock.h>		// Extensiones socket de MFC
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
