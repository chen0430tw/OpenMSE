// [!output PROJECT_NAME].cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
[!if WEBDBG]
// 对于 WebDbg.exe 的自定义断言和跟踪处理
#ifdef _DEBUG
CDebugReportHook g_ReportHook;
#endif
[!endif]
[!if GENERATING_APP]

#include "[!output APP_FILE]"
[!endif]
[!if ATTRIBUTED]
[ module(name="My[!output SAFE_PROJECT_IDENTIFIER_NAME]", type=dll) ][!if !GENERATING_APP];[!endif]
[!if GENERATING_APP]

class CDllMainOverride
{
public:
};
[!endif]

[ emitidl(restricted) ];
[!else]

class C[!output SAFE_PROJECT_IDENTIFIER_NAME]Module : public CAtlDllModuleT<C[!output SAFE_PROJECT_IDENTIFIER_NAME]Module>
{
public:
};

C[!output SAFE_PROJECT_IDENTIFIER_NAME]Module _AtlModule;

[!if !ATTRIBUTED && GENERATING_APP]
[!if TODO]

// TODO: 向处理程序映射添加其他请求处理程序
[!endif]

BEGIN_HANDLER_MAP()
	HANDLER_ENTRY("Default", [!output APP_CLASS])
END_HANDLER_MAP()
[!endif]
[!endif]
[!if GENERATING_ISAPI]
[!if DATASOURCE_CACHE || BLOB_CACHE || FILE_CACHE || SESSION_MEM || SESSION_DB || BROWSCAPS]

#include "[!output EXTENSION_FILE]"
[!endif]
[!if PERFMON]

BEGIN_PERFREG_MAP()
	PERFREG_ENTRY(CRequestPerfMon)
END_PERFREG_MAP()
[!endif]
[!endif]
[!if GENERATING_ISAPI]
[!if PERFMON]

typedef [!output EXTENSION_CLASS]<CThreadPool<[!if DATASOURCE_CACHE][!output EXTENSION_CLASS]Worker[!else]CIsapiWorker[!endif]>, CPerfMonRequestStats > ExtensionType;
[!else]

typedef [!output EXTENSION_CLASS]<[!if DATASOURCE_CACHE]CThreadPool<[!output EXTENSION_CLASS]Worker> [!endif]> ExtensionType;
[!endif]

// ATL Server ISAPI 扩展
ExtensionType theExtension;
[!endif]
[!if GENERATING_ISAPI]


// 将 ISAPI 导出委托给 theExtension
//
extern "C" DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpECB)
{
	return theExtension.HttpExtensionProc(lpECB);
}

extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	return theExtension.GetExtensionVersion(pVer);
}

extern "C" BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
	return theExtension.TerminateExtension(dwFlags);
}
[!endif]
[!if !ATTRIBUTED]

#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL 入口点
//
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

[!if GENERATING_ISAPI && PERFMON]


// DllRegisterServer - 将项添加到系统注册表
//
STDAPI DllRegisterServer(void)
{
	return _AtlModule.DllRegisterServer(FALSE);
}


// DllUnregisterServer - 将项从系统注册表中移除
//
STDAPI DllUnregisterServer(void)
{
	return _AtlModule.DllUnregisterServer(FALSE);
}
[!endif]
[!endif]