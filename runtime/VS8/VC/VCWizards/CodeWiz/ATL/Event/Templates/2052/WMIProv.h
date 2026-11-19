[!if !EVENT]
//错误: 事件提供程序头模板调用的提供程序类型错误
[!else]
// [!output HEADER_FILE] : Declaration of the [!output CLASS_NAME]

#pragma once
#include "resource.h"       // 主符号
#include <wbemidl.h>
#include <wmiatlprov.h>


extern const WCHAR * s_pMyClassName;    //类名

[!if EXTRINSIC]
[!output EXTR_PROPERTY_DECLARATIONS]
[!endif]

/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

[!if ATTRIBUTED]
[
    coclass,
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

class ATL_NO_VTABLE [!output CLASS_NAME] : 
[!if !ATTRIBUTED]
[!if THREADING_BOTH]
                            public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
[!if THREADING_FREE]
                            public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
                            public CComCoClass<[!output CLASS_NAME], &CLSID_[!output COCLASS]>,
[!endif]
[!if EVENT_SECURITY]
                            public IWbemEventProviderSecurity,
[!endif]
[!if QUERY_SINK]
                            public IWbemEventProviderQuerySink,
[!endif]
                            public IWbemProviderInit,
                            public IWbemEventProvider
{
    CComPtr<IWbemServices>  m_pNamespace;   //缓存的 IWbemServices 指针
    CComPtr<IWbemObjectSink> m_pSink;           //缓存的事件接收器指针

[!if INTRINSIC] 
    CComPtr<IWbemClassObject> m_pDataClass;     //激发事件的类的缓存定义

    CIntrinsicEventProviderHelper * m_pHelper;  //指向帮助器类对象的指针
[!else]
    CComPtr<IWbemClassObject> m_pEventClass;    //缓存的事件类定义指针
    
    CProviderHelper * m_pHelper;
  
    STDMETHODIMP FireEvent();
    [!endif]    

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
        //注意: 接口指针数据成员都是智能型的，可以自动释放
    }

    [!if !ATTRIBUTED]
    DECLARE_REGISTRY_RESOURCEID(IDR_[!output UPPER_SHORT_NAME])

    DECLARE_NOT_AGGREGATABLE([!output CLASS_NAME])

    BEGIN_COM_MAP([!output CLASS_NAME])
        COM_INTERFACE_ENTRY(IWbemEventProvider)
[!if EVENT_SECURITY ]
        COM_INTERFACE_ENTRY(IWbemEventProviderSecurity)
[!endif]
[!if QUERY_SINK ]
        COM_INTERFACE_ENTRY(IWbemEventProviderQuerySink)
[!endif]    
        COM_INTERFACE_ENTRY(IWbemProviderInit)
    END_COM_MAP()
[!endif]


    //IWbemProviderInit
    HRESULT STDMETHODCALLTYPE Initialize( __in_opt LPWSTR pszUser,
                                          LONG lFlags,
                                          __in LPWSTR pszNamespace,
                                          __in_opt LPWSTR pszLocale,
                                          IWbemServices *pNamespace,
                                          IWbemContext *pCtx,
                                          IWbemProviderInitSink *pInitSink);

    //IWbemEventProvider
    HRESULT STDMETHODCALLTYPE ProvideEvents(IWbemObjectSink *pSink,
                                            long lFlags);


[!if EVENT_SECURITY ]
    //IWbemEventProviderSecurity
    HRESULT STDMETHODCALLTYPE AccessCheck(WBEM_CWSTR wszQueryLanguage,
                                          WBEM_CWSTR wszQuery,
                                          long lSidLength,
                                          const BYTE* pSid);
[!endif]

[!if QUERY_SINK ]
    //IWbemEventProviderQuerySink
    HRESULT STDMETHODCALLTYPE CancelQuery(unsigned long dwId);

    HRESULT STDMETHODCALLTYPE NewQuery(unsigned long dwId,
                                       __in WBEM_WSTR wszQueryLanguage,
                                       __in WBEM_WSTR wszQuery);
[!endif]

};

[!if !ATTRIBUTED]
OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output CLASS_NAME])
[!endif]

[!endif]