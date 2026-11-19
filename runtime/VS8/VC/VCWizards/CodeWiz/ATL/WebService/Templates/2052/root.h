// [!output HEADER_FILE] : 定义 ATL Server 请求处理程序类
//
#pragma once

[!if NO_PCH]
#include <atlsoap.h>
[!endif]


namespace [!output APP_CLASS]Service
{
// webservice 的所有 struct、enum 和 typedef 应进入命名空间

// I[!output APP_CLASS]服务 - Web 服务接口声明
//
[
	uuid("[!output SOAP_INTERFACE_GUID]"), 
	object
]
__interface I[!output APP_CLASS]Service
{
[!if SAMPLE_CODE]
	// HelloWorld 是一个示例 ATL Server Web 服务方法。它显示如何
	// 声明 Web 服务方法及其 in 参数和 out 参数
	[id(1)] HRESULT HelloWorld([in] BSTR bstrInput, [out, retval] BSTR *bstrOutput);
[!endif]

[!if TODO_COMMENTS]
	// TODO: 在此添加其他 Web 服务方法
[!endif]
};


// [!output APP_CLASS]服务 - Web 服务实现
//
[
	request_handler(name="[!output SOAP_HANDLER_NAME]", sdl="Gen[!output SAFE_APP_CLASS]WSDL"),
	soap_handler(
		name="[!output SOAP_HANDLER_NAME]", 
		namespace="urn:[!output SOAP_HANDLER_NAME]",
		protocol="soap"
	)
]
class C[!output APP_CLASS]Service :
	public I[!output APP_CLASS]Service
{
public:
[!if SAMPLE_CODE]
	[ soap_method ]
	HRESULT HelloWorld(/*[in]*/ BSTR bstrInput, /*[out, retval]*/ BSTR *bstrOutput)
	{
		CComBSTR bstrOut(L"Hello ");
		bstrOut += bstrInput;
		bstrOut += L"!";
		*bstrOutput = bstrOut.Detach();
		
		return S_OK;
	}
[!endif]

[!if TODO_COMMENTS]
	// TODO: 在此添加其他 Web 服务方法
[!endif]
}; // 类 C[!output APP_CLASS]服务
} // 命名空间 [!output APP_CLASS]服务
