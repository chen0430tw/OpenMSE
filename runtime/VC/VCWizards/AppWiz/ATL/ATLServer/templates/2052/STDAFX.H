// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 错误说明仅在调试版本中
// 向用户显示。它们总是记录到事件日志中
#ifndef _DEBUG
#define ATL_CRITICAL_ISAPI_ERROR_LOGONLY
#endif

[!if GENERATING_APP]
[!if SOAP]
#define _ATL_APARTMENT_THREADED
[!endif]
[!endif]

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0403
#endif

[!if TODO]
// TODO: 这将禁用对注册由此项目导出的 COM 对象
// 的支持，因为此项目不包含任何
// COM 对象或类型库。如果希望从此项目中导出 COM 对象，
// 请添加类型库并移除此行
[!endif]
#define _ATL_NO_COM_SUPPORT

[!if PERFMON && GENERATING_ISAPI]
#define _ATL_PERF_REGISTER
[!endif]

[!if GENERATING_ISAPI]
#include "resource.h"
#include <atlsrvres.h>
#include <atlisapi.h>
#include <atlstencil.h>
[!endif]
[!if GENERATING_APP]
[!if !GENERATING_ISAPI]
[!if STENCIL || VALIDATION || ATTRIBUTED]
#include <atlstencil.h>
[!else]
#include <atlisapi.h>
[!endif]
[!endif]
[!if SOAP]
#include <atlsoap.h>
[!endif]
[!endif]
[!if SESSION_MEM || SESSION_DB]
#include <atlsession.h>
[!endif]

[!if PERFMON && GENERATING_ISAPI]
#include <atlperf.h>
#include <atlsync.h>
[!endif]
[!if TODO]

// TODO: 在此处引用程序需要的其他头文件
[!endif]
