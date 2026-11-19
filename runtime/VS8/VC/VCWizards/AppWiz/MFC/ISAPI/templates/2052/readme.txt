========================================================================
ISAPI 项目：[!output PROJECT_NAME] 项目概述
========================================================================

[!if GENERATE_FILTER && GENERATE_EXTENSION]
应用程序向导已创建了这个包含 ISAPI 通知和服务器扩展对象的 [!output PROJECT_NAME] 项目。
[!else]
[!if GENERATE_FILTER]
应用程序向导已创建了这个包含 ISAPI 通知对象的 [!output PROJECT_NAME] 项目。
[!endif]
[!if GENERATE_EXTENSION]
应用程序向导已创建了这个包含 ISAPI 服务器扩展对象的 [!output PROJECT_NAME] 项目。
[!endif]
[!endif]

本文件概要介绍组成项目的每个文件的内容。

[!output PROJECT_NAME].vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

[!output PROJECT_NAME].h
此文件包含 C++ 接口定义和用于插入命令处理程序的空间。

[!output PROJECT_NAME].cpp
此文件包含对象映射和筛选器对象通知的实现。此文件包含服务器扩展映射和 CHttpServer 对象的实现。

[!output PROJECT_NAME].rc
这是程序使用的所有 Microsoft Windows 资源的列表。

[!output PROJECT_NAME].rc2
这是项目中包括的 Microsoft Visual C++ 不直接使用的所有资源的清单。这些资源不能使用 VC++ 资源编辑器编辑。

[!output PROJECT_NAME].def
此模块定义文件为链接器提供有关 DLL 所要求的导出的信息。它包含用于以下内容的导出：
[!if GENERATE_FILTER]
        HttpFilterProc
        GetFilterVersion
[!endif] 
[!if GENERATE_EXTENSION]
        HttpExtensionProc
        GetExtensionVersion
        TerminateExtension
[!endif]

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h, StdAfx.cpp
这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

Resource.h
这是用于定义资源 ID 的标准头文件。

/////////////////////////////////////////////////////////////////////////////