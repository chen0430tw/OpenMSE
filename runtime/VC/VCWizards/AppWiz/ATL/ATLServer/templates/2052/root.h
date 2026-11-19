// [!output APP_FILE] : 定义 ATL Server 请求处理程序类
//
#pragma once

[!if __dbgcmt]
// 仅当在选中了 Web 服务复选框的情况下创建
// 应用程序 DLL 时才应生成如下内容
[!endif]
[!if GENERATING_APP && SOAP]
namespace [!output APP_CLASS]
{
// webservice 的所有 struct、enum 和 typedef 应进入命名空间

// I[!output APP_CLASS] - Web 服务接口声明
//
[
	uuid("[!output SOAP_INTERFACE_GUID]"), 
	object
]
__interface I[!output APP_CLASS]
{
[!if TODO]
	// HelloWorld 是一个示例 ATL Server Web 服务方法。它显示如何
	// 声明 Web 服务方法及其 in 参数和 out 参数
[!endif]
	[id(1)] HRESULT HelloWorld([in] BSTR bstrInput, [out, retval] BSTR *bstrOutput);
[!if TODO]
	// TODO: 在此添加其他 Web 服务方法
[!endif]
};


// [!output APP_CLASS] - Web 服务实现
//
[
	request_handler(name="Default", sdl="Gen[!output SAFE_PROJECT_HTTP_NAME]WSDL"),
	soap_handler(
		name="[!output APP_CLASS]", 
		namespace="urn:[!output APP_CLASS]",
		protocol="soap"
	)
]
class C[!output APP_CLASS] :
	public I[!output APP_CLASS]
{
public:
[!if SESSION_MEM || SESSION_DB || FILE_CACHE || BLOB_CACHE || BROWSCAPS]
	// 如果想要使用
	// 通过您的 ISAPI 扩展生成的服务，请取消注释服务声明
[!endif]
[!if SESSION_MEM || SESSION_DB]
//	CComPtr<ISessionStateService> m_spSessionSvc;
//	CComPtr<ISession> m_spSession;
[!endif]
[!if FILE_CACHE]
//	CComPtr<IFileCache> m_spFileCache;
[!endif]
[!if BLOB_CACHE]
//	CComPtr<IMemoryCache> m_spBlobCache;
[!endif]
[!if BROWSCAPS]
//	CComPtr<IBrowserCapsSvc> m_spBrowserCaps;
[!endif]
[!if SESSION_MEM || SESSION_DB || FILE_CACHE || BLOB_CACHE || DATASOURCE_CACHE]
	HTTP_CODE InitializeHandler(AtlServerRequest *pRequestInfo, IServiceProvider *pProvider)
	{
		if (HTTP_SUCCESS != CSoapHandler<C[!output APP_CLASS]>::InitializeHandler(pRequestInfo, pProvider))
			return HTTP_FAIL;
[!if SESSION_MEM || SESSION_DB]

		// 从 ISAPI 扩展中获取 ISessionStateService
//		if (FAILED(pProvider->QueryService(__uuidof(ISessionStateService), 
//						&m_spSessionSvc)))
//			return HTTP_FAIL;
[!endif]
[!if FILE_CACHE]

		// 从 ISAPI 扩展中获取 IFileCache 服务
//		if (FAILED(pProvider->QueryService(__uuidof(IFileCache), 
//						&m_spFileCache)))
//			return HTTP_FAIL;
[!endif]
[!if BLOB_CACHE]

		// 从 ISAPI 扩展中获取 IMemoryCache 服务
//		if (FAILED(pProvider->QueryService(__uuidof(IMemoryCache), 
//						&m_spBlobCache)))
//			return HTTP_FAIL;
[!endif]
[!if DATASOURCE_CACHE]

		// 取消注释下列代码以
		// 从数据源缓存中检索数据源连接。将 connection_name 替换为
		// 用于标识该连接的字符串，
		// 并将 connection_string 替换为对数据源有效的 OLEDB
		// 连接字符串。此代码假定 
		// 由 m_spServiceProvider 指向的服务提供程序
		// 可以提供指向数据源 
		// 缓存服务(通常在 ISAPI 扩展中实现)的 IDataSourceCache 指针。
//		CDataConnection dc;
//		if (S_OK != GetDataSource(  pProvider, 
//									_T("connection_name"),
//									L"connection_string", 
//									&dc ))
//			return HTTP_FAIL;
[!endif]
[!if BROWSCAPS]

		// 从 ISAPI 扩展中获取 IBrowserCapsSvc 服务
//		if (FAILED(pProvider->QueryService(__uuidof(IBrowserCapsSvc), 
//						&m_spBrowserCaps)))
//			return HTTP_FAIL;
[!endif]
		return HTTP_SUCCESS;
	}	
[!endif]	
[!if TODO]
	// 这是一个示例 Web 服务方法，它显示如何使用 
	// soap_method 属性将方法公开为 Web 方法
[!endif]
	[ soap_method ]
	HRESULT HelloWorld(/*[in]*/ BSTR bstrInput, /*[out, retval]*/ BSTR *bstrOutput)
	{
		CComBSTR bstrOut(L"Hello ");
		bstrOut += bstrInput;
		bstrOut += L"!";
		*bstrOutput = bstrOut.Detach();
		
		return S_OK;
	}
[!if TODO]
	// TODO: 在此添加其他 Web 服务方法
[!endif]
}; // 类 C[!output APP_CLASS]
[!endif]
[!if __dbgcmt]
// 仅当在选中了 Web 服务复选框的情况下创建
// 应用程序 DLL 时才应生成如下内容
[!endif]
[!if GENERATING_APP && !SOAP]
[!if ATTRIBUTED]
[ request_handler("Default") ]
[!endif]
class [!output APP_CLASS]
[!if !ATTRIBUTED]
[!if STENCIL || VALIDATION]
	: public CRequestHandlerT<[!output APP_CLASS]>
[!else]
	: public IRequestHandlerImpl<[!output APP_CLASS]>, public CComObjectRootEx<CComSingleThreadModel>
[!endif]
[!endif]
{
private:
[!if TODO]
	// 在此放置私有成员
[!if SESSION_MEM || SESSION_DB || FILE_CACHE || BLOB_CACHE || BROWSCAPS]
	// 如果想要使用
	// 通过您的 ISAPI 扩展生成的服务，请取消注释服务声明
[!endif]
[!if SESSION_MEM || SESSION_DB]

	// 会话服务支持
//	CComPtr<ISessionStateService> m_spSessionSvc;
//	CComPtr<ISession> m_spSession;
[!endif]
[!if FILE_CACHE]

	// 文件缓存支持
//	CComPtr<IFileCache> m_spFileCache;
[!endif]
[!if BLOB_CACHE]

	// Blob 缓存支持
//	CComPtr<IMemoryCache> m_spBlobCache;
[!endif]
[!if BROWSCAPS]

	// 数据源缓存支持
//	CComPtr<IBrowserCapsSvc> m_spBrowserCaps;
[!endif]
[!endif]

protected:
[!if TODO]
	// 在此放置保护成员

[!endif]	
public:
[!if TODO]
	// 在此放置公共成员

[!endif]
[!if !STENCIL && !VALIDATION && !ATTRIBUTED]

	BEGIN_COM_MAP([!output APP_CLASS])
		COM_INTERFACE_ENTRY(IRequestHandler)
	END_COM_MAP()
	
[!endif]
[!if STENCIL && !ATTRIBUTED]
[!if TODO]
	// TODO: 向替换方法映射添加其他标记
[!endif]
	BEGIN_REPLACEMENT_METHOD_MAP([!output APP_CLASS])
		REPLACEMENT_METHOD_ENTRY("Hello", OnHello)
	END_REPLACEMENT_METHOD_MAP()

[!endif]
[!if VALIDATION]
	HTTP_CODE ValidateAndExchange()
	{
[!if TODO]
		// TODO: 在此放置所有初始化代码和验证代码
		
[!endif]
[!if TODO]
		// 设置内容类型
[!endif]		
		m_HttpResponse.SetContentType("text/html");
[!if TODO]		
[!if SESSION_MEM || SESSION_DB || FILE_CACHE || BLOB_CACHE || DATASOURCE_CACHE]

		// 如果想要使用
		// 通过您的 ISAPI 扩展生成的服务，请取消注释服务声明
		
[!endif]
[!if SESSION_MEM || SESSION_DB]

		// 从 ISAPI 扩展中获取 ISessionStateService
//		if (FAILED(m_spServiceProvider->QueryService(__uuidof(ISessionStateService), 
//						&m_spSessionSvc)))
//			return HTTP_FAIL;
[!endif]
[!if FILE_CACHE]

		// 从 ISAPI 扩展中获取 IFileCache 服务
//		if (FAILED(m_spServiceProvider->QueryService(__uuidof(IFileCache), 
//						&m_spFileCache)))
//			return HTTP_FAIL;
[!endif]
[!if BLOB_CACHE]

		// 从 ISAPI 扩展中获取 IMemoryCache 服务
//		if (FAILED(m_spServiceProvider->QueryService(__uuidof(IMemoryCache), 
//						&m_spBlobCache)))
//			return HTTP_FAIL;
[!endif]
[!if DATASOURCE_CACHE]
		// 取消注释下列代码以
		// 从数据源缓存中检索数据源连接。将 connection_name 替换为
		// 用于标识该连接的字符串，
		// 并将 connection_string 替换为对数据源有效的 OLEDB
		// 连接字符串。此代码假定 
		// 由 m_spServiceProvider 指向的服务提供程序
		// 可以提供指向数据源 
		// 缓存服务(通常在 ISAPI 扩展中实现)的 IDataSourceCache 指针。
//		CDataConnection dc;
//		if (S_OK != GetDataSource(  m_spServiceProvider, 
//									_T("connection_name"),
//									L"connection_string", 
//									&dc ))
//			return HTTP_FAIL;
[!endif]
[!if BROWSCAPS]

		// 从 ISAPI 扩展中获取 IBrowserCapsSvc 服务
//		if (FAILED(m_spServiceProvider->QueryService(__uuidof(IBrowserCapsSvc), 
//						&m_spBrowserCaps)))
//			return HTTP_FAIL;
[!endif]
[!endif]
		
		return HTTP_SUCCESS;
	}
[!endif]
[!if STENCIL]
 
protected:
[!if TODO]
	// 下面是一个有关如何将替换标记用于模具(Stencil)处理器的示例
[!endif]
[!if ATTRIBUTED]
	[ tag_name(name="Hello") ]
[!endif]	
	HTTP_CODE OnHello(void)
	{
		m_HttpResponse << "Hello World!";
		return HTTP_SUCCESS;
	}
[!endif]
[!if !STENCIL && !VALIDATION]
public:
	HTTP_CODE HandleRequest(AtlServerRequest *pRequest, IServiceProvider *pProvider)
	{
[!if TODO]	
		// 初始化 CHttpResponse
[!endif]		
		CHttpResponse Response;
		BOOL bRet = Response.Initialize(pRequest->pServerContext);
		if (!bRet)
			return HTTP_FAIL;

[!if TODO]		
		// 设置内容类型
[!endif]		
		Response.SetContentType("text/html");
[!if TODO]

		// TODO: 在此添加处理程序代码
[!endif]

[!if TODO]
		// 编写响应
[!endif]		
		Response << "<html><body><H1>Hello World!</H1></body></html>";
		return HTTP_SUCCESS;
	}
[!endif]
}; // 类 [!output APP_CLASS]
[!endif]
[!if GENERATING_APP && SOAP]

} // 命名空间 [!output APP_CLASS]
[!endif]