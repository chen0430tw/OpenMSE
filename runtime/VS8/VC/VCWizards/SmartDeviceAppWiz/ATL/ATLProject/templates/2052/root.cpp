[!if DLL_APP]
// [!output PROJECT_NAME].cpp : DLL 导出的实现。
[!else]
// [!output PROJECT_NAME].cpp : WinMain 的实现
[!endif]

[!if SUPPORT_COMPLUS]
//
// 注意: COM+ 1.0 信息:
//      请记住运行 Microsoft Transaction Explorer 以安装组件。
//      默认情况下不进行注册。
[!endif]

#include "stdafx.h"

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

[!if !ATTRIBUTED]
#include "[!output SAFE_IDL_NAME].h"
[!endif]
[!if SUPPORT_COMPONENT_REGISTRAR]
#include "compreg.h"
[!endif]
[!if MERGE_PROXY_STUB]
#include "dlldatax.h"
[!endif]

[!if ATTRIBUTED]
[!if DLL_APP]

// 模块属性使 DllMain、DllRegisterServer 和 DllUnregisterServer 为您自动实现
[ module(dll, uuid = "{[!output LIBID_REGISTRY_FORMAT]}", 
		 name = "[!output SAFE_PROJECT_IDENTIFIER_NAME]", 
		 helpstring = "[!output PROJECT_NAME] 1.0 类型库",
		 resource_name = "IDR_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]"[!if SUPPORT_COMPONENT_REGISTRAR], 
		 custom = { "a817e7a1-43fa-11d0-9e44-00aa00b6770a", "{[!output COMPREG_REGISTRY_FORMAT]}"}[!endif]) ]
class [!output SAFE_ATL_MODULE_NAME]
{
public:
// 重写 CAtlDllModuleT 成员
};
		 
[!endif]
[!if EXE_APP]

// 模块属性使 WinMain 为您自动实现
[ module(EXE, uuid = "{[!output LIBID_REGISTRY_FORMAT]}", 
		 name = "[!output SAFE_PROJECT_IDENTIFIER_NAME]", 
		 helpstring = "[!output PROJECT_NAME] 1.0 类型库",
		 resource_name = "IDR_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]") ]
class [!output SAFE_ATL_MODULE_NAME]
{
public:
// 重写 CAtlExeModuleT 成员
};
		 
[!endif]
[!if SERVICE_APP]

// 模块属性使 WinMain 为您自动实现
[ module(SERVICE, uuid = "{[!output LIBID_REGISTRY_FORMAT]}", 
		 name = "[!output SAFE_PROJECT_IDENTIFIER_NAME]", 
		 helpstring = "[!output PROJECT_NAME] 1.0 类型库", 
		 resource_name="IDS_SERVICENAME") ]
class [!output SAFE_ATL_MODULE_NAME]
{
public:
	HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的 
		// 安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 NULL 安全说明符。

		return S_OK;
	}
};

[!endif]
[!else]
[!if DLL_APP]

class [!output SAFE_ATL_MODULE_NAME] : public CAtlDllModuleT< [!output SAFE_ATL_MODULE_NAME] >
[!endif]
[!if EXE_APP]

class [!output SAFE_ATL_MODULE_NAME] : public CAtlExeModuleT< [!output SAFE_ATL_MODULE_NAME] >
[!endif]
[!if SERVICE_APP]
#include <stdio.h>

class [!output SAFE_ATL_MODULE_NAME] : public CAtlServiceModuleT< [!output SAFE_ATL_MODULE_NAME], IDS_SERVICENAME >
[!endif]
{
public :
	DECLARE_LIBID(LIBID_[!output LIB_NAME]Lib)
[!if SUPPORT_DCOM]
#ifdef _CE_DCOM
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]DCOM, "{[!output APPID_REGISTRY_FORMAT]}")
#endif
[!endif]
[!if SUPPORT_NON_DCOM]
#ifndef _CE_DCOM
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME], "{[!output APPID_REGISTRY_FORMAT]}")
#endif
[!endif]
[!if SERVICE_APP]
	HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的 
		// 安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 NULL 安全说明符。

		return S_OK;
	}
[!endif]	
};

[!output SAFE_ATL_MODULE_NAME] _AtlModule;

[!if DLL_APP]
[!if SUPPORT_MFC]
class [!output SAFE_MFC_APP_NAME] : public CWinApp
{
public:

// 重写
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP([!output SAFE_MFC_APP_NAME], CWinApp)
END_MESSAGE_MAP()

[!output SAFE_MFC_APP_NAME] theApp;

BOOL [!output SAFE_MFC_APP_NAME]::InitInstance()
{
[!if MERGE_PROXY_STUB]
#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
[!endif]
    return CWinApp::InitInstance();
}

int [!output SAFE_MFC_APP_NAME]::ExitInstance()
{
    return CWinApp::ExitInstance();
}
[!else]

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HANDLE hInstance, DWORD dwReason, LPVOID lpReserved)
{
[!if MERGE_PROXY_STUB]
#ifdef _MERGE_PROXYSTUB
	// 因为 Windows CE 定义 HINSTANCE 的方式与桌面不同，所以
	// 此 reinterpret_cast 是必需的。
    if (!PrxDllMain(reinterpret_cast<HINSTANCE>(hInstance), dwReason, lpReserved))
        return FALSE;
#endif
[!endif]
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}
[!endif]


// 用于确定 DLL 是否可由 OLE 卸载
STDAPI DllCanUnloadNow(void)
{
[!if MERGE_PROXY_STUB]
#ifdef _MERGE_PROXYSTUB
    HRESULT hr = PrxDllCanUnloadNow();
    if (hr != S_OK)
        return hr;
#endif
[!endif]
[!if SUPPORT_MFC]
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
[!else]
    return _AtlModule.DllCanUnloadNow();
[!endif]
}


// 返回一个类工厂以创建所请求类型的对象
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
[!if MERGE_PROXY_STUB]
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
[!endif]
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - 将项添加到系统注册表
STDAPI DllRegisterServer(void)
{
    // 注册对象、类型库和类型库中的所有接口
    HRESULT hr = _AtlModule.DllRegisterServer();
[!if MERGE_PROXY_STUB]
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
#endif
[!endif]
	return hr;
}


// DllUnregisterServer - 将项从系统注册表中移除
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer(FALSE);
[!if MERGE_PROXY_STUB]
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
    if (FAILED(hr))
        return hr;
    hr = PrxDllUnregisterServer();
#endif
[!endif]
	return hr;
}
[!endif]

[!if EXE_APP]

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

[!endif]
[!if SERVICE_APP]

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

[!endif]
[!endif]
