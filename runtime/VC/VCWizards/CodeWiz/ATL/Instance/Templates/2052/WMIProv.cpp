// [!output IMPL_FILE] : [!output CLASS_NAME] 的实现
#include "stdafx.h"
[!if !ATTRIBUTED]
#include "[!output PROJECT_NAME].h"
[!endif]
#include "[!output HEADER_FILE]"


/////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]

//////////////////////////////////////////////////////////////////////////////
// 类名、属性名和方法名

// 重要事项: 以下定义的字符串不可本地化

[!if CLASS_SPECIFIED]
const static WCHAR * s_pMyClassName = L"[!output WMICLASSNAME]"; //类名

//属性:
[!output PROPERTY_DEFINITIONS]

//方法:
[!output METHOD_DEFINITIONS]

[!else]
//TODO: 定义提供的类名，例如:
const static WCHAR * s_pMyClassName = L"MyClassName"; 

//TODO: 定义提供的类的属性名，例如:
//const static WCHAR * pMyProperty = L"MyProperty";

//TODO: 定义提供的类的方法名，例如:
//const static WCHAR * pMyMethod = L"MyMethod";
[!endif]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::Initialize
//请参阅有关 IWbemProviderInit::Initialize() 的 MSDN 文档
//获取有关实现此方法的详细信息

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
    

    //缓存 IWbemServices 指针 
    //注意: m_pNamespace 是智能指针，因此它能自动执行 AddRef()
    m_pNamespace = pNamespace;
                
    //缓存提供的类定义
    //注意: 以下的代码假定提供程序运行期间您的类定义
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
                                 pCtx,   //传递 IWbemContext 指针以防止死锁
                                 &m_pClass, 
                                 NULL);
    SysFreeString(bstrClassName);
    bstrClassName = NULL;
    if(FAILED(hr))
    {
        return hr;
    }
    
    //缓存路径分析器类工厂
    hr = CoGetClassObject(CLSID_WbemDefPath, 
                            CLSCTX_INPROC_SERVER, 
                            NULL,
                            IID_IClassFactory,
                            (void **) &m_pPathFactory);
    if (FAILED(hr))
    {
        return hr;
    }

    //创建帮助器对象
    m_pHelper = new CInstanceProviderHelper(pNamespace, pCtx);

    if(m_pHelper == NULL)
    {
        return WBEM_E_OUT_OF_MEMORY;
    }

    //注意: 要将错误或状态的详细信息报告给 WMI，可以在提供程序中的任意位置 
    //调用 m_pHelper 上的 ConstructErrorObject()

    return pInitSink->SetStatus(WBEM_S_INITIALIZED,0);
}

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::GetObjectAsync
// 请参阅有关 IWbemServices::GetObjectAsync() 的 MSDN 文档
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::GetObjectAsync( 
                             const BSTR bstrObjectPath,
                             long lFlags,
                             IWbemContext  *pCtx,
                             IWbemObjectSink  *pResponseHandler)
{

[!if SUPPORT_GET ]
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

    if (NULL == pResponseHandler)
    {
        return WBEM_E_INVALID_PARAMETER;
    }

    CComPtr<IWbemClassObject> pInstance;
    hr = GetInstanceByPath(bstrObjectPath, &pInstance);
    if (FAILED (hr))
    {
        return hr;
    }
            
    //如果全部正常，将对象返回 WMI 并指示成功:
    hr = pResponseHandler->Indicate(1, &(pInstance.p)); 
    return pResponseHandler->SetStatus(0, hr, NULL, NULL);
[!else]
    return WBEM_E_PROVIDER_NOT_CAPABLE;
[!endif]}

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::PutInstanceAsync()
// 请参阅 IWbemServices::PutInstanceAsync() 的 MSDN 文档
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::PutInstanceAsync( 
                            IWbemClassObject  *pInst,
                            long lFlags,
                            IWbemContext  *pCtx,
                            IWbemObjectSink  *pResponseHandler)
{
   
[!if SUPPORT_PUT ]
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

    if ( NULL == pResponseHandler || NULL == pInst)
    {
        return WBEM_E_INVALID_PARAMETER;
    }
    //TODO: 检查可能的标志值: WBEM_FLAG_UPDATE_ONLY、 
    //WBEM_FLAG_CREATE_ONLY 和 WBEM_FLAG_CREATE_OR_UPDATE
    //并选择您需要的支持级别，对于不支持的标志值，
    //返回 WBEM_E_PROVIDER_NOT_CAPABLE

    //TODO: 如果计划支持部分更新，请检查 
    //“__PUT_EXTENSIONS”的 pCtx 以及其他相关系统上下文值，
    //并相应更新实例数据
    
    //TODO: 请在此处理实例更新或创建         
    
    return pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
[!else]
    return WBEM_E_PROVIDER_NOT_CAPABLE;
[!endif]
}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::DeleteInstanceAsync()
// 请参阅有关 IWbemServices::DeleteInstanceAsync() 的 MSDN 帮助
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::DeleteInstanceAsync( 
                            const BSTR ObjectPath,
                            long lFlags,
                            IWbemContext  *pCtx,
                            IWbemObjectSink  *pResponseHandler)
{
[!if SUPPORT_DELETE ]
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

    //为实现此方法，实例提供程序会分析 strObjectPath 参数中 
    //指定的对象路径字符串，试图查找相应的实例 
    //并将其删除。

    if (NULL == pResponseHandler)
    {
        return WBEM_E_INVALID_PARAMETER;
    }

    CComPtr<IWbemClassObject> pInstance;
    hr = GetInstanceByPath(ObjectPath, &pInstance);
    if (FAILED (hr))
    {
        return hr;
    }

    //TODO: 遍历数据源以查找匹配对象并将其删除。
    //注意: 如果找不到与路径中指示的键值匹配的对象，
    //请取消下列行的注释，以便将此信息传递回 WMI:
    //return WBEM_E_NOT_FOUND;
    
    return pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL); 
    [!else]
    return WBEM_E_PROVIDER_NOT_CAPABLE;
    [!endif]

}


//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::CreateInstanceEnumAsync()
// 请参阅有关 IWbemServices::CreateInstanceEnumAsync() 的 MSDN 文档
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::CreateInstanceEnumAsync( 
                            const BSTR Class,
                            long lFlags,
                            IWbemContext  *pCtx,
                            IWbemObjectSink  *pResponseHandler)
{
    
[!if SUPPORT_ENUMERATE ]
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

    if (NULL == pResponseHandler)
    {
        return WBEM_E_INVALID_PARAMETER;
    }

    hr = WBEM_S_NO_ERROR;
    
[!if IS_SINGLETON]
    // 准备一个空对象，以接收实例数据
    CComPtr<IWbemClassObject> pNewInst;
    hr = m_pClass->SpawnInstance(0, &pNewInst);
    
    VARIANT var;
    VariantInit(&var);
[!if CLASS_SPECIFIED]
    [!output POPULATE_INSTANCE]
[!else]
    //TODO: 使用属性填充实例，例如:
    //var.vt = VT_BSTR;
    //var.bstrVal = <value>;  //在此处输入适当的值
    //BSTR bstrPropertyName=SysAllocString(pMyProperty);
    //if (NULL == bstrPropertyName)
    //{
    //    // 根据值的创建方式，可能需要在此处对其进行清理
    //    hr = WBEM_E_OUT_OF_MEMORY;
    //    return hr;
    //}
    //hr = pNewInst->Put(bstrPropertyName, 0, &var, 0);
    //SysFreeString(bstrPropertyName);
    //bstrPropertyName = NULL;
    //// 根据值的创建方式，可能需要在此处对其进行清理
    //if(FAILED(hr))
    //{
    //  return hr;
    //}
[!endif]
    
    // 将类传送给 WMI。
    hr = pResponseHandler->Indicate(1, &(pNewInst.p));
[!else]
    // 当(<有多个实例>)时，在专用源之间循环
    //并创建每个实例
    {
        // 准备一个空对象，以接收类定义。
        CComPtr<IWbemClassObject> pNewInst;
        hr = m_pClass->SpawnInstance(0, &pNewInst);
        if (FAILED(hr))
        {   
            //TODO: 如果符合循环条件，取消下列行的注释
            //break;
        }
        
        VARIANT var;
        VariantInit(&var);
[!if CLASS_SPECIFIED]
[!output POPULATE_INSTANCE]
[!else]
        //TODO: 使用属性填充实例，例如:
        //var.vt = VT_BSTR;
        //var.bstrVal = <value>;  //在此处输入适当的值
        //BSTR bstrPropertyName=SysAllocString(pMyProperty);
        //if (NULL == bstrPropertyName)
        //{
        //    // 根据值的创建方式，可能需要在此处对其进行清理
        //    hr = WBEM_E_OUT_OF_MEMORY;
        //    return hr;
        //}
        //hr = pNewInst->Put(bstrPropertyName, 0, &var, 0);
        //SysFreeString(bstrPropertyName);
        //bstrPropertyName = NULL;
        //// 根据值的创建方式，可能需要在此处对其进行清理
        //if(FAILED(hr))
        //{
        //  return hr;
        //}
        [!endif]
        
        // 将类传送给 WMI。
        hr = pResponseHandler->Indicate(1, &(pNewInst.p));
    }
[!endif]
    return pResponseHandler->SetStatus(0, hr, NULL, NULL);
[!else]
    return WBEM_E_PROVIDER_NOT_CAPABLE;
[!endif]
}


//////////////////////////////////////////////////////////////////////////////
//[!output CLASS_NAME]::ExecQueryAsync()
// 请参阅有关 IWbemServices::ExecQueryAsync() 的 MSDN 文档
// 获取有关实现此方法的详细信息
[!if SUPPORTS_QUERY]
// 请参阅有关使用 IWbemQuery 接口进行查询分析的 MSDN 文档
[!endif]
STDMETHODIMP [!output CLASS_NAME]::ExecQueryAsync( 
                            const BSTR QueryLanguage,
                            const BSTR Query,
                            long lFlags,
                            IWbemContext  *pCtx,
                            IWbemObjectSink  *pResponseHandler)
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

    
[!if SUPPORTS_QUERY]
    // 创建 WMI 查询分析器对象来分析给定的查询
    if(m_pQuery == NULL)
    {
        hr = CoCreateInstance(CLSID_WbemQuery,
                        0,
                        CLSCTX_INPROC_SERVER,
                        IID_IWbemQuery,
                        (LPVOID *)&m_pQuery);
    }
    else
    {
        hr = WBEM_S_NO_ERROR;
    }
[!else]
    hr = WBEM_E_NOT_SUPPORTED;
[!endif]
    
    if(SUCCEEDED(hr))
    {
        // 实例提供程序可以选择支持查询处理或依赖 WMI 
        // 提供该服务。要支持查询，实例提供程序必须能够分析 
        // 简单的结构化查询语言(SQL)语句、执行请求的查询 
        // 并将结果集对象传送到请求方接收器。 
        
[!if SUPPORTS_QUERY]
        // m_pQuery 是一个 IWbemQuery 接口指针，可以用于分析查询
        // 取消以下部分的注释以分析查询
        //hr = m_pQuery->Parse(QueryLanguage,Query,0);
        //if (FAILED(hr) and (提供程序不支持提供程序定义的查询))
        //{
        //      // 对提供程序定义的查询进行分析
        //}
        
        // if(SUCCEEDED(hr)
        // {
        //      // 请在此处输入查询处理代码
        // }
[!else]
        //TODO: 请在此处输入查询处理代码

[!endif]

        //hr = pResponseHandler->SetStatus(0, WBEM_S_NO_ERROR, NULL, NULL);
    }
    return hr;
    
}

[!if PROVIDE_METHODS ]

//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::ExecMethodAsync()
// 请参阅有关 IWbemServices::ExecMethodAsync() 的 MSDN 文档
// 获取有关实现此方法的详细信息

STDMETHODIMP [!output CLASS_NAME]::ExecMethodAsync( 
                            const BSTR strObjectPath,
                            const BSTR strMethodName,
                            long lFlags,
                            IWbemContext  *pCtx,
                            IWbemClassObject  *pInParams,
                            IWbemObjectSink  *pResponseHandler)
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

[!if CLASS_SPECIFIED]
[!if  !HAS_IMPL_METHODS]    
    //[!output WMICLASSNAME] 未实现方法 
    return WBEM_E_NOT_SUPPORTED;
[!else]
    // don't need to check inParams because if the method has params it has already been checked for NULL
    if (NULL == pResponseHandler)
    {
        return WBEM_E_INVALID_PARAMETER;
    }   
[!output EXEC_METHOD_BODY]
[!endif]
[!else]

    ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_IS_CLASS_REF;


    //请检查路径中的类名是否正确。
    BSTR bstrClassName=SysAllocString(s_pMyClassName);
    if (NULL == bstrClassName)
    {
        hr = WBEM_E_OUT_OF_MEMORY;
        return hr;
    }
    hr = m_pHelper->CheckInstancePath(m_pPathFactory,
                                      strObjectPath,
                                      bstrClassName,
                                      ulPathTest);
    SysFreeString(bstrClassName);
    bstrClassName = NULL;

    if(FAILED(hr))
    {
        //路径中有语法错误，或提供的类的路径不正确
        return hr; 
    }           

    VARIANT var;
    VariantInit(&var);

    //获取输入参数:
    //TODO: 如果方法有输入参数，那么这些参数将作为
    //pInParams 对象的属性进行传递。以下所注释的行演示了如何提取这些:
    //BSTR bstrInputArg=SysAllocString(L"InputArgument1");
    //if (NULL == bstrInputArg)
    //{
    //    hr = WBEM_E_OUT_OF_MEMORY;
    //    return hr;
    //}
    //hr = pInParams->Get(bstrInputArg, 0, &var, NULL, NULL);
    //SysFreeString(bstrInputArg);
    //bstrInputArg = NULL;
    //if (FAILED(hr))
    //{  
    //    return hr;
    //}
    //TODO: 保存输入参数值
    VariantClear(&var);        
    
    //分析路径以查找执行方法的实例: 对于非静态方法
    CComPtr<IWbemClassObject> pInstance;
    hr = GetInstanceByPath(strObjectPath,&pInstance);
    if(FAILED(hr))
    {
        return hr;
    }

    
    //TODO: 在此处添加执行方法的代码

    //获取输出参数类，以便将方法执行结果加载到
    CComPtr<IWbemClassObject> pOutClass;
    hr = m_pClass->GetMethod(strMethodName, 0, NULL, &pOutClass);
    if(SUCCEEDED(hr) && pOutClass != NULL)
    {
        CComPtr<IWbemClassObject> pOutParams;
        pOutClass->SpawnInstance(0, &pOutParams);
        //TODO: 通过填充 pOutParams 类的属性创建
        //输出参数。例如:

        //var.vt = VT_BSTR;
        //var.bstrVal = <value> // 使用适当的值填充变量
        //BSTR bstrOutputParam=SysAllocString(L"MyOutputParameter");
        //if (NULL == bstrOutputParam)
        //{
        //    // depending on how value was created you may need to clean it up here
        //    hr = WBEM_E_OUT_OF_MEMORY;
        //    return hr;
        //}
        //hr = pOutParams->Put(bstrOutputParam, 0, &var, 0);
        //SysFreeString(bstrOutputParam);
        //bstrOutputParam = NULL;
        //// 根据值的创建方式，可能需要在此处对其进行清理
        //if(FAILED(hr))
        //{
        //  return hr;
        //}
        //var.vt = VT_I2;
        //var.iVal = <value> //使用适当的值填充变量
        //BSTR bstrReturnValue=SysAllocString(L"ReturnValue");
        //if (NULL == bstrReturnValue)
        //{
        //    // 根据值的创建方式，可能需要在此处对其进行清理
        //    hr = WBEM_E_OUT_OF_MEMORY;
        //    return hr;
        //}
        //hr = pOutParams->Put(bstrReturnValue, 0, &var, 0);
        //SysFreeString(bstrReturnValue);
        //bstrReturnValue = NULL;
        //// 根据值的创建方式，可能需要在此处对其进行清理
        //if(FAILED(hr))
        //{
        //  return hr;
        //}
        // 通过接收器将输出对象发送回客户端
        hr = pResponseHandler->Indicate(1, &(pOutParams.p));
        hr = pResponseHandler->SetStatus(WBEM_STATUS_COMPLETE, hr, NULL, NULL);
    }

    return hr;
[!endif]

}
[!endif]



//////////////////////////////////////////////////////////////////////////////
// [!output CLASS_NAME]::GetInstanceByPath() 分析路径以确定要求的键值，
// 然后在内部存储区搜索带有匹配键值的对象。如果
// 找到了那样的对象，则方法将生成新的实例，填充所有属性并
// 将其返回 ppInstance。如果没有找到，则方法将返回 WBEM_E_NOT_FOUND。

STDMETHODIMP [!output CLASS_NAME]::GetInstanceByPath (
                                    /*in*/BSTR bstrPath,
                                    /*out*/IWbemClassObject ** ppInstance )
                                                    
{
    HRESULT hr = WBEM_E_FAILED;               
        
[!if IS_COMPOUND_KEY]
    ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_IS_COMPOUND;
[!else]
[!if IS_SINGLETON]
    ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF | WBEMPATH_INFO_CONTAINS_SINGLETON;
[!else]
    ULONG ulPathTest = WBEMPATH_INFO_IS_INST_REF;
[!endif]
[!endif]
    BSTR bstrClassName=SysAllocString(s_pMyClassName);
    if (NULL == bstrClassName)
    {
        hr = WBEM_E_OUT_OF_MEMORY;
        return hr;
    }        
    hr = m_pHelper->CheckInstancePath(m_pPathFactory,
                                      bstrPath,
                                      bstrClassName,
                                      ulPathTest);
    SysFreeString(bstrClassName);
    bstrClassName = NULL;
    if (FAILED(hr))
    {
        //路径中有语法错误，或提供的类的路径不正确
        return WBEM_E_INVALID_PARAMETER; 
    }           
        
[!if IS_SINGLETON]
//[!output WMICLASSNAME] 是一个 singleton 对象。无需标识实例
    //注意: 如果没有实例，请取消下列行的注释，以便将此信息传送回 WMI:
    //return WBEM_E_NOT_FOUND;
[!else]
    //获取路径分析器对象:
    CComPtr<IWbemPath>pPath;
    hr = m_pPathFactory->CreateInstance(NULL,
                                        IID_IWbemPath,
                                        (void **) &pPath);
    if (FAILED(hr))
    {
        return hr;
    }
    
    hr = pPath->SetText(WBEMPATH_CREATE_ACCEPT_ALL,bstrPath);
    if (FAILED(hr))
    {
        return hr;
    }

    CComPtr<IWbemPathKeyList> pIKeyList;
    hr = pPath->GetKeyList(&pIKeyList);
    if (FAILED(hr))
    {
        return hr;
    }
    unsigned long ulNumKeys;
    hr = pIKeyList->GetCount(&ulNumKeys);
    if(FAILED(hr))
    {
        return hr;
    }


    //获取关键属性的值
    BSTR bstrPathName=SysAllocString(bstrPath);
    if (NULL == bstrPathName)
    {
        hr = WBEM_E_OUT_OF_MEMORY;
        return hr;
    }
    unsigned int uKeyNameBufferSize = SysStringLen(bstrPathName);
    SysFreeString(bstrPathName);
    bstrPathName = NULL;
    if (uKeyNameBufferSize >= (unsigned long)(-1)) return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
    WCHAR  * wKeyName = new WCHAR[uKeyNameBufferSize+1];
    if (NULL == wKeyName)
    {
        return WBEM_E_OUT_OF_MEMORY;
    }

    unsigned long ulKeyNameBufferSize = (unsigned long)uKeyNameBufferSize+1;
    VARIANT vValue;
    VariantInit(&vValue);     
    unsigned long ulApparentCimType;
    for (unsigned long i = 0; i < ulNumKeys; i++)
    {           
        hr = pIKeyList->GetKey2(i, 0L, &ulKeyNameBufferSize, 
                                wKeyName, &vValue, &ulApparentCimType); 
        if (FAILED(hr))
        {
             // save failed hr's and continue or just return an errror on the first failure
             return hr;
        }        
        //TODO: 保存 vValue 以备后用
        VariantClear(&vValue);
    }

    delete[] wKeyName;


    //TODO: 搜索内部数据源以查找匹配对象。
    //如果找不到任何带有要求的键值的对象， 
    //将返回 WBEM_E_NOT_FOUND。
    [!endif]

    //生成新实例
    CComPtr<IWbemClassObject> pNewInst;
    hr = m_pClass->SpawnInstance(0, &pNewInst);
    if(FAILED(hr))
    {
        return hr;
    }
    VARIANT var;
    VariantInit(&var);
    //TODO: 使用匹配的内部对象填充新实例的属性
[!if CLASS_SPECIFIED]
[!output GET_INSTANCEBYPATH]
[!else]
    //示例:
    //var.vt = VT_BSTR;
    //var.bstrVal = <value>; //在此处输入适当的值
    //BSTR bstrPropertyName=SysAllocString(pMyProperty);
    //if (NULL == bstrPropertyName)
    //{
    //    // 根据值的创建方式，可能需要在此处对其进行清理
    //    hr = WBEM_E_OUT_OF_MEMORY;
    //    return hr;
    //}
    //hr = pNewInst->Put(bstrPropertyName, 0, &var, 0);
    //SysFreeString(bstrPropertyName);
    //bstrPropertyName = NULL;
    //// 根据值的创建方式，可能需要在此处对其进行清理
    //if(FAILED(hr))
    //{
    //    return hr;
    //}
  
[!endif]

    // use the CComPtrBase.CopyTo method to copy pointer to the output parameter
    hr = pNewInst.CopyTo(ppInstance);

    return hr;
}

