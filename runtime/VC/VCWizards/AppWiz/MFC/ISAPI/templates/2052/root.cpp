[!if GENERATE_FILTER]
[!if GENERATE_EXTENSION]
// [!output PROJECT_NAME].cpp - ISAPI 的实现文件
//    [!output FILTER_DESC] 和 [!output EXTENSION_DESC]
[!else]
// [!output PROJECT_NAME].cpp - ISAPI 的实现文件
//    [!output FILTER_DESC]
[!endif]
[!else]
// [!output PROJECT_NAME].cpp - ISAPI 的实现文件
//    [!output EXTENSION_DESC]
[!endif]

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"


// 唯一的 CWinApp 对象
// 注意: 如果将项目更改为在 DLL 中不再使用 MFC，
// 则可能可以移除此对象。

CWinApp theApp;

[!if GENERATE_EXTENSION]

// 命令分析映射

BEGIN_PARSE_MAP([!output EXTENSION_CLASS], CHttpServer)
	// TODO: 在此插入 ON_PARSE_COMMAND() 和
	// ON_PARSE_COMMAND_PARAMS() 以将命令挂钩。
	// 例如:

	ON_PARSE_COMMAND(Default, [!output EXTENSION_CLASS], ITS_EMPTY)
	DEFAULT_PARSE_COMMAND(Default, [!output EXTENSION_CLASS])
END_PARSE_MAP([!output EXTENSION_CLASS])



// 唯一的 [!output EXTENSION_CLASS] 对象

[!output EXTENSION_CLASS] theExtension;



// [!output EXTENSION_CLASS] 实现

[!output EXTENSION_CLASS]::[!output EXTENSION_CLASS]()
{
}

[!output EXTENSION_CLASS]::~[!output EXTENSION_CLASS]()
{
}

BOOL [!output EXTENSION_CLASS]::GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	// 调用初始化的默认实现
	CHttpServer::GetExtensionVersion(pVer);

	// 加载说明字符串
	TCHAR sz[HSE_MAX_EXT_DLL_NAME_LEN];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
			IDS_SERVER, sz, HSE_MAX_EXT_DLL_NAME_LEN-1));
	ISAPIVERIFY(SUCCEEDED(StringCbCopy(pVer->lpszExtensionDesc,sizeof(pVer->lpszExtensionDesc),sz)));
	return TRUE;
}

BOOL [!output EXTENSION_CLASS]::TerminateExtension(DWORD dwFlags)
{
	// 扩展正被终止
	//TODO: 清理任何基于实例的资源
	return TRUE;
}


// [!output EXTENSION_CLASS] 命令处理程序

void [!output EXTENSION_CLASS]::Default(CHttpServerContext* pCtxt)
{
	StartContent(pCtxt);
	WriteTitle(pCtxt);

	*pCtxt << _T("此默认消息是由 Internet");
	*pCtxt << _T(" Server DLL 向导产生的。请编辑 [!output EXTENSION_CLASS]::Default()");
	*pCtxt << _T(" 实现以更改它。\r\n");

	EndContent(pCtxt);
}

[!endif]


[!if GENERATE_FILTER]

// 唯一的 [!output FILTER_CLASS] 对象

[!output FILTER_CLASS] theFilter;



// [!output FILTER_CLASS] 实现

[!output FILTER_CLASS]::[!output FILTER_CLASS]()
{
}

[!output FILTER_CLASS]::~[!output FILTER_CLASS]()
{
}

BOOL [!output FILTER_CLASS]::GetFilterVersion(PHTTP_FILTER_VERSION pVer)
{
	// 调用初始化的默认实现
	CHttpFilter::GetFilterVersion(pVer);

	// 清除由基类设置的标志
	pVer->dwFlags &= ~SF_NOTIFY_ORDER_MASK;

	// 设置我们感兴趣的标志
	pVer->dwFlags |= [!output FILTER_FLAGS];

[!if PRIORITY_HIGH]
	// 设置优先级
	pVer->dwFlags |= SF_NOTIFY_ORDER_HIGH;
[!endif]
[!if PRIORITY_MEDIUM]
	// 设置优先级
	pVer->dwFlags |= SF_NOTIFY_ORDER_MEDIUM;
[!endif]
[!if PRIORITY_LOW]
	// 设置优先级
	pVer->dwFlags |= SF_NOTIFY_ORDER_LOW;
[!endif]

	// 加载说明字符串
	TCHAR sz[SF_MAX_FILTER_DESC_LEN];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
			IDS_FILTER, sz, SF_MAX_FILTER_DESC_LEN-1));
	ISAPIVERIFY(SUCCEEDED(StringCbCopy(pVer->lpszFilterDesc, sizeof(pVer->lpszFilterDesc), sz)));
	return TRUE;
}
[!if NOTIFY_PREPROC_HEADERS]

DWORD [!output FILTER_CLASS]::OnPreprocHeaders(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_PREPROC_HEADERS pHeaderInfo)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_AUTHENTICATION]

DWORD [!output FILTER_CLASS]::OnAuthentication(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_AUTHENT pAuthent)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_AUTH_COMPLETE]

DWORD [!output FILTER_CLASS]::OnAuthComplete(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_AUTH_COMPLETE_INFO pAuthCompInfo)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_URL_MAP]

DWORD [!output FILTER_CLASS]::OnUrlMap(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_URL_MAP pMapInfo)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_SEND_RAW_DATA]

DWORD [!output FILTER_CLASS]::OnSendRawData(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_RAW_DATA pRawData)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_READ_RAW_DATA]

DWORD [!output FILTER_CLASS]::OnReadRawData(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_RAW_DATA pRawData)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_LOG]

DWORD [!output FILTER_CLASS]::OnLog(CHttpFilterContext *pCtxt, PHTTP_FILTER_LOG pLog)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_END_OF_NET_SESSION]

DWORD [!output FILTER_CLASS]::OnEndOfNetSession(CHttpFilterContext* pCtxt)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_END_OF_REQUEST]

DWORD [!output FILTER_CLASS]::OnEndOfRequest(CHttpFilterContext* pfc)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_SEND_RESPONSE]

DWORD [!output FILTER_CLASS]::OnSendResponse(CHttpFilterContext*, PHTTP_FILTER_SEND_RESPONSE)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!if NOTIFY_ACCESS_DENIED]

DWORD [!output FILTER_CLASS]::OnAccessDenied(CHttpFilterContext*, PHTTP_FILTER_ACCESS_DENIED)
{
	// TODO: 对此通知做出相应响应，并且
	// 返回适当的状态代码
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}
[!endif]
[!endif]


// 如果您的扩展不使用 MFC，您将需要此代码，以确保
// 扩展对象可以找到模块的资源句柄。
// 如果将扩展转换为不依赖于 MFC，
// 请移除以下 AfxGetResourceHandle()
// 和 DllMain() 函数周围的注释，以及 g_hInstance 全局变量。

/****

static HINSTANCE g_hInstance;

HINSTANCE AFXISAPI AfxGetResourceHandle()
{
	return g_hInstance;
}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ulReason,
					LPVOID lpReserved)
{
	if (ulReason == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInst;
	}

	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


****/
