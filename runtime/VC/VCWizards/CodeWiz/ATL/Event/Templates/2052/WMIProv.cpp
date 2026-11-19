// [!output IMPL_FILE] : [!output CLASS_NAME] 的实现
#include "stdafx.h"
[!if !ATTRIBUTED]
#include "[!output PROJECT_NAME].h"
[!endif]
#include "[!output HEADER_FILE]"


/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

// 重要事项: 以下定义的字符串不可本地化

// 类名
[!if CLASS_SPECIFIED]
const static WCHAR * s_pMyClassName = L"[!output WMICLASSNAME]"; 
[!if EXTRINSIC]
//[!output WMICLASSNAME] 事件类的属性:
[!output EXTR_PROPERTY_DEFINITIONS]
[!endif]
[!else]
//TODO: 定义提供的类名，例如: 
const static WCHAR * s_pMyClassName = L"MyClassName"; 
[!endif]


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::Initialize()
// 请参阅有关 IWbemProviderInit::Initialize() 的 MSDN 文档
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::Initialize(__in_opt LPWSTR pszUser,   
                        LONG lFlags,
                        __in LPWSTR pszNamespace,
                        __in_opt LPWSTR pszLocale,
                        IWbemServices *pNamespace,
                        IWbemContext *pCtx,
                        IWbemProviderInitSink *pInitSink) 
{
    
    CImpersonateClientHelper impersonateClient;
    HRESULT hr = WBEM_S_NO_ERROR;
    DWORD dwImpersonationLevel = 0;

    if(SUCCEEDED(hr = impersonateClient.GetCurrentImpersonationLevel(dwImpersonationLevel)))
    {
        // 如果当前线程将模拟级别设置为 RPC_C_IMP_LEVEL_IDENTIFY 或 RPC_C_IMP_LEVEL_ANONYMOUS
        if(dwImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
        {
            hr = WBEM_E_ACCESS_DENIED;
            // 根据提供程序的安全配置自行对访问的资源进行访问检查，
            // 并更改相应返回代码 hr
        }
        else
        {
            hr = impersonateClient.ImpersonateClient();
        }
    }

    if(FAILED(hr))
    {
        return hr;
    }

    if ( NULL == pNamespace || NULL == pInitSink) 
    {
        return WBEM_E_INVALID_PARAMETER;
    }

    hr = WBEM_E_FAILED;

    //缓存 IWbemServices 指针
    //注意: m_pNamespace 是智能指针，因此它能自动执行 AddRef()
    m_pNamespace = pNamespace;
                    
    [!if INTRINSIC]
    //获取帮助器对象
    m_pHelper = new CIntrinsicEventProviderHelper(pNamespace, pCtx);
    if(m_pHelper == NULL)
    {
        return WBEM_E_OUT_OF_MEMORY;
    }

    [!endif]
    
    [!if EXTRINSIC]
    //存储事件类定义              
    //注意: 以下的代码假定提供程序运行期间您的事件类定义
    //不会发生更改。如果不为 true，您应为类修改和类删除事件
    //实现使用程序。有关事件使用程序的信息，
    //请参阅 MSDN 上的 WMI 文档。
    BSTR bstrClassName=SysAllocString(s_pMyClassName);
    if (NULL == bstrClassName)
    {
        hr = WBEM_E_OUT_OF_MEMORY;
        return hr;
    }
    hr = m_pNamespace->GetObject(bstrClassName, 
                                0, 
                                pCtx, //传递 IWbemContext 指针以防止死锁
                                &m_pEventClass, 
                                NULL);
    SysFreeString(bstrClassName);
    bstrClassName = NULL;

    if(FAILED(hr))
    {
        return hr;
    }

    //获取帮助器对象
    m_pHelper = new CProviderHelper(pNamespace, pCtx);
    if(m_pHelper == NULL)
    {
        return WBEM_E_OUT_OF_MEMORY;
    }

    [!endif]
    [!if INTRINSIC]
    //存储目标类定义
    BSTR bstrClassName=SysAllocString(s_pMyClassName);
    if (NULL == bstrClassName)
    {
        hr = WBEM_E_OUT_OF_MEMORY;
        return hr;
    }
    hr = m_pNamespace->GetObject(bstrClassName, 
                                 0, 
                                 pCtx, //传递 IWbemContext 指针以防止死锁
                                 &m_pDataClass, 
                                 NULL);
    SysFreeString(bstrClassName);
    bstrClassName = NULL;
    if(FAILED(hr))
    {
        return hr;
    }
    [!endif]
        
    return pInitSink->SetStatus(WBEM_S_INITIALIZED,0);
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::ProvideEvents()
// 请参阅有关 IWbemEventProvider::ProvideEvents() 的 MSDN 文档
// 获取有关实现此方法的详细信息
    
STDMETHODIMP [!output CLASS_NAME]::ProvideEvents(
                        IWbemObjectSink *pSink,
                        long lFlags)
{
    //  WMI 调用此方法以激活事件提供程序。
    //  TODO: 从此调用返回后，开始传送此事件，
    //  因为它们可能会提供接收器接口。您可能希望创建独立线程
    //  来处理事件传送。
    CImpersonateClientHelper impersonateClient;
    HRESULT hr = WBEM_S_NO_ERROR;
    DWORD dwImpersonationLevel = 0;

    if(SUCCEEDED(hr = impersonateClient.GetCurrentImpersonationLevel(dwImpersonationLevel)))
    {
        // 如果当前线程将模拟级别设置为 RPC_C_IMP_LEVEL_IDENTIFY 或 RPC_C_IMP_LEVEL_ANONYMOUS
        if(dwImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
        {
            hr = WBEM_E_ACCESS_DENIED;
            // 根据提供程序的安全配置自行对访问的资源进行访问检查，
            // 并更改相应返回代码 hr
        }
        else
        {
            hr = impersonateClient.ImpersonateClient();
        }
    }

    if(FAILED(hr))
    {
        return hr;
    }

[!if INTRINSIC]
    //  要传送内部事件，请调用 m_pHelper 上的 FireCreationEvent()、FireDeletionEvent() 和 
    //  FireModificationEvent() 方法。
[!endif]
    //  要将错误或状态的详细信息报告给 WMI，可以调用 m_pHelper 上的 ConstructErrorObject()。
    
    //  重要事项: 不要阻塞此调用达数秒钟之久。

    if ( NULL == pSink )
    {
        return WBEM_E_INVALID_PARAMETER;
    }

    //缓存接收器指针
    //注意: m_pSink 是智能指针，因此它能自动执行 AddRef()
    m_pSink = pSink;
    
    return WBEM_S_NO_ERROR;
}

[!if EVENT_SECURITY ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::AccessCheck()
// 请参阅有关 IWbemEventProviderSecurity::AccessCheck() 的 MSDN 文档
// 获取有关实现此方法的详细信息
// 请参阅有关使用 IWbemQuery 分析查询的 MSDN 文档

STDMETHODIMP [!output CLASS_NAME]::AccessCheck(
                            WBEM_CWSTR wszQueryLanguage,
                            WBEM_CWSTR wszQuery,
                            long lSidLength,
                            const BYTE* pSid)
{
    CImpersonateClientHelper impersonateClient;
    HRESULT hr = WBEM_S_NO_ERROR;
    DWORD dwImpersonationLevel = 0;

    if(SUCCEEDED(hr = impersonateClient.GetCurrentImpersonationLevel(dwImpersonationLevel)))
    {
        // 如果当前线程将模拟级别设置为 RPC_C_IMP_LEVEL_IDENTIFY 或 RPC_C_IMP_LEVEL_ANONYMOUS
        if(dwImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
        {
            hr = WBEM_E_ACCESS_DENIED;
            // 根据提供程序的安全配置自行对访问的资源进行访问检查，
            // 并更改相应返回代码 hr
        }
        else
        {
            hr = impersonateClient.ImpersonateClient();
        }
    }

    if(FAILED(hr))
    {
        return hr;
    }


    // 创建 WMI 查询分析器对象来分析给定的查询

    CComPtr<IWbemQuery> pQuery;               // WBEM 查询分析器对象

    pQuery = NULL;

    hr = CoCreateInstance(CLSID_WbemQuery,
                          0,
                          CLSCTX_INPROC_SERVER,
                          IID_IWbemQuery,
                          (LPVOID *)&pQuery);


    if(SUCCEEDED(hr))
    {
        // TODO: 分析查询并根据查询进行访问检查
    }
    
    // 当前返回失败，这样在可以使用此类之前即可实现上面的 TODO
    // 写入代码以便在所分析的查询上执行访问检查时，删除下面的 hr 设置 
    hr = WBEM_E_ACCESS_DENIED;
    return hr;
}
[!endif]

[!if QUERY_SINK ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::CancelQuery()
// 请参阅有关 IWbemEventProviderQuerySink::CancelQuery() 的 MSDN 文档
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::CancelQuery(
              unsigned long dwId)
{
    CImpersonateClientHelper impersonateClient;
    HRESULT hr = WBEM_S_NO_ERROR;
    DWORD dwImpersonationLevel = 0;

    if(SUCCEEDED(hr = impersonateClient.GetCurrentImpersonationLevel(dwImpersonationLevel)))
    {
        // 如果当前线程将模拟级别设置为 RPC_C_IMP_LEVEL_IDENTIFY 或 RPC_C_IMP_LEVEL_ANONYMOUS
        if(dwImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
        {
            hr = WBEM_E_ACCESS_DENIED;
            // 根据提供程序的安全配置自行对访问的资源进行访问检查，
            // 并更改相应返回代码 hr
        }
        else
        {
            hr = impersonateClient.ImpersonateClient();
        }
    }

    // TODO: 实现取消

    return hr;
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::NewQuery()
// 请参阅有关 IWbemEventProviderQuerySink::NewQuery() 的 MSDN 文档
// 获取有关实现此方法的详细信息
// 请参阅有关使用 IWbemQuery 分析查询的 MSDN 文档

STDMETHODIMP [!output CLASS_NAME]::NewQuery(
              unsigned long dwId,
              __in WBEM_WSTR wszQueryLanguage,
              __in WBEM_WSTR wszQuery)
{
    CImpersonateClientHelper impersonateClient;
    HRESULT hr = WBEM_S_NO_ERROR;
    DWORD dwImpersonationLevel = 0;

    if(SUCCEEDED(hr = impersonateClient.GetCurrentImpersonationLevel(dwImpersonationLevel)))
    {
        // 如果当前线程将模拟级别设置为 RPC_C_IMP_LEVEL_IDENTIFY 或 RPC_C_IMP_LEVEL_ANONYMOUS
        if(dwImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE)
        {
            hr = WBEM_E_ACCESS_DENIED;
            // 根据提供程序的安全配置自行对访问的资源进行访问检查，
            // 并更改相应返回代码 hr
        }
        else
        {
            hr = impersonateClient.ImpersonateClient();
        }
    }

    if(FAILED(hr))
    {
        return hr;
    }


    // 创建 WMI 查询分析器对象来分析给定的查询

    CComPtr<IWbemQuery> pQuery;               // WBEM 查询分析器对象

    pQuery = NULL;
    hr = CoCreateInstance(CLSID_WbemQuery,
                          0,
                          CLSCTX_INPROC_SERVER,
                          IID_IWbemQuery,
                          (LPVOID *)&pQuery);

    if(SUCCEEDED(hr))
    {
        // TODO: 分析查询并进行基于查询的优化
    }

    return hr;
}
[!endif]

[!if EXTRINSIC] 
//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::FireEvent()

STDMETHODIMP [!output CLASS_NAME]::FireEvent()
{
    HRESULT hr = WBEM_S_NO_ERROR;


    ATLASSERT(m_pEventClass);

    CComPtr<IWbemClassObject> pEvtInstance;
    hr = m_pEventClass->SpawnInstance(0, &pEvtInstance);
    if(FAILED(hr))
    {
        return hr;
    }

    //填充事件对象的属性值:
[!if CLASS_SPECIFIED]
    [!output EXTRINSIC_PUT_BLOCK]
    [!else]
    //TODO: 修改以下的注释代码以填充事件属性值和变量类型
    //VARIANT var;
    //VariantInit(&var);
    //var.vt = VT_BSTR;       //在此处输入适当的变量类型
    //var.bstrVal = <value>;  //在此处输入适当的值
    //BSTR bstrEventProperty=SysAllocString(L"EventProperty1");
    //if (NULL == bstrEventProperty)
    //{
    //    // 根据值的创建方式，可能需要在此处对其进行清理
    //    hr = WBEM_E_OUT_OF_MEMORY;
    //    return hr;
    //}
    //hr = pEvtInstance->Put(bstrEventProperty, 0, &var, 0);
    //SysFreeString(bstrEventProperty);
    //bstrEventProperty=NULL;
    //// depending on how the value was created you may need to clean it up here
    //if(FAILED(hr))
    //{
    //  return hr;
    //}
    [!endif]

    return m_pSink->Indicate(1, &(pEvtInstance.p) );
}
[!endif]    
