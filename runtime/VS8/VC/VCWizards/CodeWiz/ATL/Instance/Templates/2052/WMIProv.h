[!if  !INSTANCE]
//错误: 实例提供程序头模板调用的提供程序类型错误
[!else]
// [!output HEADER_FILE] : [!output CLASS_NAME] 的声明

#pragma once
#include "resource.h"       // 主符号

#include <wbemidl.h>
#include <wmiatlprov.h>
#include <wmiutils.h>   //路径分析器

//////////////////////////////////////////////////////////////////////////////
// 类名、属性名和方法名: 在 [!output IMPL_FILE] 中进行定义

extern const WCHAR * s_pMyClassName;    //类名

[!if CLASS_SPECIFIED]
//属性:
[!output PROPERTY_DECLARATIONS]

//方法:
[!output METHOD_DECLARATIONS]
[!else]
//TODO: 声明提供的类的属性名，例如:
//extern const WCHAR * pMyProperty;

//TODO: 声明提供的类的方法名，例如:
//extern const WCHAR * pMyMethod;
[!endif]

[!if ATTRIBUTED]
[
    coclass,
[!if THREADING_APARTMENT]
    threading(apartment),
[!endif]
[!if THREADING_BOTH]
    threading(both),
[!endif]
[!if THREADING_FREE]
    threading(free),
[!endif]
    aggregatable(never),
    vi_progid("[!output VERSION_INDEPENDENT_PROGID]"),
    progid("[!output PROGID]"),
    version(1.0),
    uuid("[!output CLSID_REGISTRY_FORMAT]"),
    helpstring("[!output TYPE_NAME]")
]
[!endif]
/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

class ATL_NO_VTABLE [!output CLASS_NAME] : 
[!if !ATTRIBUTED]
[!if THREADING_APARTMENT]
                            public CComObjectRootEx<CComSingleThreadModel>,
[!endif]
[!if THREADING_BOTH]
                            public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
[!if THREADING_FREE]
                            public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
                            public CComCoClass<[!output CLASS_NAME], &CLSID_[!output COCLASS]>,
[!endif]
                            public IWbemInstProviderImpl
{
    
    CComPtr<IWbemServices>  m_pNamespace;   //缓存的 IWbemServices 指针
    CComPtr<IWbemClassObject> m_pClass;     //缓存的提供的类定义      
    CComPtr<IWbemClassObject> m_pErrorObject;//缓存的错误对象类定义指针
    CComPtr<IClassFactory> m_pPathFactory;   //缓存的指向路径分析器的类工厂的指针

[!if SUPPORTS_QUERY]
    CComPtr<IWbemQuery> m_pQuery;               // WBEM 查询分析器对象
[!endif]
    CInstanceProviderHelper * m_pHelper;
    
    STDMETHODIMP GetInstanceByPath (BSTR bstrPath,
                                    IWbemClassObject ** ppInstance );

  public:
    [!output CLASS_NAME]()
    {
        m_pHelper = NULL;                   
    }

    ~[!output CLASS_NAME]()
    {
        if (NULL != m_pHelper)
        {
            delete m_pHelper;
        }
        //接口指针数据成员都是智能型的，可以自动释放
    }
    
    [!if !ATTRIBUTED]
    DECLARE_REGISTRY_RESOURCEID(IDR_[!output UPPER_SHORT_NAME])

    DECLARE_NOT_AGGREGATABLE([!output CLASS_NAME])

    BEGIN_COM_MAP([!output CLASS_NAME])
        COM_INTERFACE_ENTRY(IWbemServices)
        COM_INTERFACE_ENTRY(IWbemProviderInit)
    END_COM_MAP()
    [!endif]

    //IWbemProviderInit
    HRESULT STDMETHODCALLTYPE Initialize( 
                                         __in_opt LPWSTR pszUser,
                                         LONG lFlags,
                                         __in LPWSTR pszNamespace,
                                         __in_opt LPWSTR pszLocale,
                                         IWbemServices *pNamespace,
                                         IWbemContext *pCtx,
                                         IWbemProviderInitSink *pInitSink);

    //IWbemServices
    HRESULT STDMETHODCALLTYPE GetObjectAsync(
                                             const BSTR ObjectPath,
                                             long lFlags,
                                             IWbemContext __RPC_FAR *pCtx,
                                             IWbemObjectSink __RPC_FAR *pResponseHandler);


    HRESULT STDMETHODCALLTYPE DeleteInstanceAsync(
                                                  const BSTR ObjectPath,
                                                  long lFlags,
                                                  IWbemContext __RPC_FAR *pCtx,
                                                  IWbemObjectSink __RPC_FAR *pResponseHandler);


    HRESULT STDMETHODCALLTYPE PutInstanceAsync(
                                               IWbemClassObject __RPC_FAR *pInst,
                                               long lFlags,
                                               IWbemContext __RPC_FAR *pCtx,
                                               IWbemObjectSink __RPC_FAR *pResponseHandler);


    HRESULT STDMETHODCALLTYPE CreateInstanceEnumAsync( 
                                                      const BSTR Class,
                                                      long lFlags,
                                                      IWbemContext __RPC_FAR *pCtx,
                                                      IWbemObjectSink __RPC_FAR *pResponseHandler);

    HRESULT STDMETHODCALLTYPE ExecQueryAsync( 
                                             const BSTR QueryLanguage,
                                             const BSTR Query,
                                             long lFlags,
                                             IWbemContext __RPC_FAR *pCtx,
                                             IWbemObjectSink __RPC_FAR *pResponseHandler);
[!if PROVIDE_METHODS]
    HRESULT STDMETHODCALLTYPE ExecMethodAsync( 
                                              const BSTR strObjectPath,
                                              const BSTR strMethodName,
                                              long lFlags,
                                              IWbemContext __RPC_FAR *pCtx,
                                              IWbemClassObject __RPC_FAR *pInParams,
                                              IWbemObjectSink __RPC_FAR *pResponseHandler);
[!else]
    HRESULT STDMETHODCALLTYPE ExecMethodAsync( 
                                              const BSTR strObjectPath,
                                              const BSTR strMethodName,
                                              long lFlags,
                                              IWbemContext __RPC_FAR *pCtx,
                                              IWbemClassObject __RPC_FAR *pInParams,
                                              IWbemObjectSink __RPC_FAR *pResponseHandler) 
    {   
        //注意: 如果决定要提供方法，您应该 
        //创建 ExecMethodAsync() 的实现
        return WBEM_E_NOT_SUPPORTED;
    }
[!endif]

};

[!if !ATTRIBUTED]
OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output CLASS_NAME])
[!endif]

[!endif]