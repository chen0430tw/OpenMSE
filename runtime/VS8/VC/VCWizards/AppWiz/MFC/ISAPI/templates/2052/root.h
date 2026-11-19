#pragma once


[!if GENERATE_FILTER]
[!if GENERATE_EXTENSION]
// [!output PROJECT_NAME].h - 用于 Internet Information Server 的头文件
//    [!output FILTER_DESC] 和 [!output EXTENSION_DESC]
[!else]
// [!output PROJECT_NAME].h - 用于 Internet Information Server 的头文件
//    [!output FILTER_DESC]
[!endif]
[!else]
// [!output PROJECT_NAME].h - 用于 Internet Information Server 的头文件
//    [!output EXTENSION_DESC]
[!endif]

#include "resource.h"

[!if GENERATE_EXTENSION]
class [!output EXTENSION_CLASS] : public CHttpServer
{
public:
	[!output EXTENSION_CLASS]();
	~[!output EXTENSION_CLASS]();

	// 重写
public:
	virtual BOOL GetExtensionVersion(HSE_VERSION_INFO* pVer);
	virtual BOOL TerminateExtension(DWORD dwFlags);

	// TODO: 在此添加命令的处理程序。
	// 例如:

	void Default(CHttpServerContext* pCtxt);

	DECLARE_PARSE_MAP()
};
[!endif]
[!if GENERATE_FILTER]
class [!output FILTER_CLASS] : public CHttpFilter
{
public:
	[!output FILTER_CLASS]();
	~[!output FILTER_CLASS]();

// 重写
	public:
	virtual BOOL GetFilterVersion(PHTTP_FILTER_VERSION pVer);
[!if NOTIFY_PREPROC_HEADERS]
	virtual DWORD OnPreprocHeaders(CHttpFilterContext* pCtxt, PHTTP_FILTER_PREPROC_HEADERS pHeaderInfo);
[!endif]
[!if NOTIFY_AUTHENTICATION]
	virtual DWORD OnAuthentication(CHttpFilterContext* pCtxt, PHTTP_FILTER_AUTHENT pAuthent);
[!endif]
[!if NOTIFY_AUTH_COMPLETE]
	virtual DWORD OnAuthComplete(CHttpFilterContext* pCtxt, PHTTP_FILTER_AUTH_COMPLETE_INFO pAuthComplInfo);
[!endif]
[!if NOTIFY_URL_MAP]
	virtual DWORD OnUrlMap(CHttpFilterContext* pCtxt, PHTTP_FILTER_URL_MAP pMapInfo);
[!endif]
[!if NOTIFY_SEND_RAW_DATA]
	virtual DWORD OnSendRawData(CHttpFilterContext* pCtxt, PHTTP_FILTER_RAW_DATA pRawData);
[!endif]
[!if NOTIFY_READ_RAW_DATA]
	virtual DWORD OnReadRawData(CHttpFilterContext* pCtxt, PHTTP_FILTER_RAW_DATA pRawData);
[!endif]
[!if NOTIFY_LOG]
	virtual DWORD OnLog(CHttpFilterContext* pfc, PHTTP_FILTER_LOG pLog);
[!endif]
[!if NOTIFY_END_OF_NET_SESSION]
	virtual DWORD OnEndOfNetSession(CHttpFilterContext* pCtxt);
[!endif]
[!if NOTIFY_END_OF_REQUEST]
	virtual DWORD OnEndOfRequest(CHttpFilterContext* pfc);
[!endif]
[!if NOTIFY_SEND_RESPONSE]
	virtual DWORD OnSendResponse(CHttpFilterContext*, PHTTP_FILTER_SEND_RESPONSE);
[!endif]
[!if NOTIFY_ACCESS_DENIED]
	virtual DWORD OnAccessDenied(CHttpFilterContext*, PHTTP_FILTER_ACCESS_DENIED);
[!endif]
};
[!endif]

