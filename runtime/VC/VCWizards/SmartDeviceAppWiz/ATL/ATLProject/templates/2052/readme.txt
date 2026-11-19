========================================================================
    活动模板库：[!output PROJECT_NAME] 项目概述
========================================================================

[!if DLL_APP]
应用程序向导已为您创建了此 [!output PROJECT_NAME] 项目，您可以在此基础之上
编写自己的动态链接库 (DLL)。
[!endif]
[!if EXE_APP]
应用程序向导已为您创建了此 [!output PROJECT_NAME] 项目，您可以在此基础之上
编写可执行程序 (EXE)。
[!endif]
[!if SERVICE_APP]
应用程序向导已为您创建了此 [!output PROJECT_NAME] 项目，您可以在此基础之上
编写服务 (EXE)。
[!endif]

[!if ATTRIBUTED]
此项目使用 Visual C++ 属性实现。
[!endif]

此文件概述了组成项目的每个文件
的内容。

[!output PROJECT_NAME].vcproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
    它包含有关生成该文件所使用的 Visual C++ 版本的信息，以及
    有关在应用程序向导中选择的平台、配置和项目功能
    的信息。

[!if ATTRIBUTED]
_[!output SAFE_IDL_NAME].idl
    此文件将在生成项目时由编译器生成。它将包含在您的项目中定义的
    类型库、接口和 coclass 的 IDL 定义。
    此文件将由 MIDL 编译器处理以生成：
        C++ 接口定义和 GUID 声明                 (_[!output SAFE_IDL_NAME].h)
        GUID 定义                                (_[!output SAFE_IDL_NAME]_i.c)
        一个类型库                               (_[!output SAFE_IDL_NAME].tlb)
        封送代码                                 (_[!output SAFE_IDL_NAME]_p.c 和 dlldata.c)
[!else]
[!output SAFE_IDL_NAME].idl
    此文件包含在您的项目中定义的类型库、接口
    和 coclass 的 IDL 定义。
    此文件将由 MIDL 编译器处理以生成：
        C++ 接口定义和 GUID 声明                 ([!output SAFE_IDL_NAME].h)
        GUID 定义                                ([!output SAFE_IDL_NAME]_i.c)
        一个类型库                               ([!output SAFE_IDL_NAME].tlb)
        封送代码                                 ([!output SAFE_IDL_NAME]_p.c 和 dlldata.c)

[!output SAFE_IDL_NAME].h
    此文件包含在 [!output SAFE_IDL_NAME].idl 中定义的项的 C++ 接口定义
    和 GUID 声明。它将在编译过程中由 MIDL 重新生成。
[!endif]

[!output PROJECT_NAME].cpp
[!if DLL_APP]
    此文件包含对象映射和 DLL 导出的实现。
[!endif]
[!if EXE_APP]
    此文件包含对象映射和 WinMain 的实现。
[!endif]
[!if SERVICE_APP]
    此文件包含对象映射以及 WinMain、ServiceMain 和
    服务管理函数的实现。
[!endif]

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    这是项目使用的所有 Microsoft Windows 资源的清单。 
    如果 .rc 文件保持不变，数据节中的定义将保持为
    它们所定义为的数值的十六进制版本，而不是
    定义的友好名称。
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    这是项目使用的所有 Microsoft Windows 资源的清单
    （当针对 Pocket PC 平台或
    支持相同的用户界面模型的平台进行编译时）。如果 .rc 文件保持不变，
    数据节中的定义将保持为它们所定义为的数值的十六进制版本，
    而不是定义的友好名称。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    这是项目使用的所有 Microsoft Windows 资源的清单
    （当针对 Smartphone 平台或
    支持相同的用户界面模型的平台进行编译时）。如果 .rc 文件保持不变，
    数据节中的定义将保持为它们所定义为的数值的十六进制版本，
    而不是定义的友好名称。
[!endif]

[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    这是项目使用的所有 Microsoft Windows 资源的清单
    （当针对支持 AYGShell 用户界面模型的平台
    进行编译时）。如果 .rc 文件保持不变，数据节中的定义
    将保持为它们所定义为的数值的十六进制版本，
    而不是定义的友好名称。
[!endif]

[!output RC2_FILE_NAME].rc2
    此文件包含不由 Microsoft 
    Visual C++ 编辑的资源。您应将所有无法由资源编辑器编辑的资源
    放置到此文件中。

[!if DLL_APP]
[!output PROJECT_NAME].def
    此模块定义文件为链接器提供有关 DLL 所要求的
    导出的信息。它包含用于以下内容的导出：
        DllGetClassObject  
        DllCanUnloadNow    
        DllRegisterServer	
        DllUnregisterServer
[!endif]

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件
    和名为 StdAfx.obj 的预编译类型文件。

[!if STANDARDSHELL_UI_MODEL]
Resource.h
    这是用于定义资源 ID 的标准头文件。
[!endif]

[!if POCKETPC2003_UI_MODEL]
Resourceppc.h
    这是用于定义资源 ID 的标准头文件。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
Resourcesp.h
    这是定义资源 ID 的标准头文件。
[!endif]

[!if AYGSHELL_UI_MODEL]
Resourceayg.h
    这是用于定义资源 ID 的标准头文件。
[!endif]

[!if !MERGE_PROXY_STUB]
/////////////////////////////////////////////////////////////////////////////
代理/存根 (stub) DLL 项目和模块定义文件：

[!output PROJECT_NAME]ps.vcproj
    此文件是用于生成代理/存根 (stub) DLL 的项目文件（若有必要）。
	主项目中的 IDL 文件必须至少包含一个接口，并且
	在生成代理/存根 (stub) DLL 之前必须先编译 IDL 文件。	此进程生成
	dlldata.c、[!output PROJECT_NAME]_i.c 和 [!output PROJECT_NAME]_p.c，它们是
	是生成代理/存根 (stub) DLL 所必需的。

[!output PROJECT_NAME]ps.def
    此模块定义文件为链接器提供有关代理/存根 (stub) 所要求的
    导出的信息。它包含用于以下内容的导出：
        DllGetClassObject  
        DllCanUnloadNow    
        GetProxyDllInfo    
        DllRegisterServer	
        DllUnregisterServer
[!endif]

[!if SUPPORT_COMPLUS || SUPPORT_MFC]
/////////////////////////////////////////////////////////////////////////////
其他注释：

[!if SUPPORT_COMPLUS]
	“COM+ 1.0 支持”选项可用于将 COM+ 1.0 库构建到您的主干应用程序中，
	从而让您能够使用 COM+ 1.0 类、对象和函数。
[!endif]

[!if SUPPORT_MFC]
	“MFC 支持”选项可用于将 Microsoft 基础类库构建到您的主干应用程序中，
	从而让您能够使用 MFC 类、对象和函数。
[!endif]
[!endif]
/////////////////////////////////////////////////////////////////////////////