// dlldata.c 的包装

#ifdef _MERGE_PROXYSTUB // 合并代理存根(stub) DLL

#define REGISTER_PROXY_DLL //DllRegisterServer 等

#ifndef _WIN32_WCE
#define _WIN32_WINNT 0x0400	//对于 WinNT 4.0 或安装了 DCOM 的 Win95
#else
#define WIN32
#endif
#define USE_STUBLESS_PROXY	//仅当使用 MIDL 开关 /Oicf 时定义

#ifndef _WIN32_WCE
#pragma comment(lib, "rpcns4.lib")
#endif
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx

#include "dlldata.c"
#include "[!output SAFE_IDL_NAME]_p.c"

#endif //_MERGE_PROXYSTUB
