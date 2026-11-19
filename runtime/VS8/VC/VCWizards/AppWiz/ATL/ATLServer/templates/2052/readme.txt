========================================================================
ATL SERVER：[!output PROJECT_NAME] 项目概述
========================================================================

应用程序向导已为您创建了此 [!output PROJECT_NAME] 应用程序。此应用程序不仅演示 ATL Server 类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 [!output PROJECT_NAME] 应用程序的每个文件的内容。

[!output PROJECT_NAME].vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。
[!if GENERATING_ISAPI]
 
[!output PROJECT_NAME].cpp
此文件是根 ISAPI 扩展文件。其中包含 ISAPI 扩展代码和其他 ATL Server 代码。可以根据项目的特定需求自定义 ATL Server 代码。
[!endif]
[!if GENERATING_APP]
 
[!output PROJECT_NAME].h
此文件包含根据您在 ATL Server 向导中选择的选项自定义的 ATL Server 请求处理程序类。
[!endif]
[!if GENERATING_ISAPI]
 
[!output PROJECT_NAME].def
此文件包含将从 ISAPI 扩展 DLL 导出的函数。  
其中包括 ISAPI 扩展函数 HttpExtensionProc、GetExtensionVersion 和 TerminateExtension。这些函数将委托给 [!output EXTENSION_CLASS] 类的实例。
[!if PERFMON]
此 DLL 还会导出性能监视支持所需的函数，不过这些函数不显示在 [!output PROJECT_NAME].def 文件中。
[!endif]
 
[!output PROJECT_NAME].rc
这是程序使用的所有 Microsoft Windows 资源的列表。其中包含 ISAPI DLL 的版本资源，并且还包含您在向导中所指定语言的 atlsrv.rc 文件。  
此文件可以直接在 Microsoft 开发环境中进行编辑。
[!if BLOB_CACHE || FILE_CACHE || SESSION_MEM || SESSION_DB || DATASOURCE_CACHE || BROWSCAPS]
 
[!output EXTENSION_FILE]
此文件包含 CIsapiExtension 派生的 ISAPI 扩展类：
[!output EXTENSION_CLASS]，该类是根据您在应用程序向导中选择的选项自定义的。
[!endif]
[!endif]
[!if GENERATING_APP && STENCIL && !SOAP]
 
[!output SRF_FILE]
此文件包含应用程序的起始 SRF（服务器响应文件）。
其中显示了与 [!output APP_CLASS] 类中 OnHello 方法关联的替换标记 {{Hello}} 的示例用法。
[!endif]
[!if GENERATING_APP && SOAP]
 
[!output PROJECT_NAME].disco
此文件包含能使 Web 服务通过支持 DISCO 协议的 Web 服务器公开的信息。
	
[!output PROJECT_NAME].htm
这是一个包含 Web 服务说明的网页。在 [!output PROJECT_NAME].disco 中引用它。在 Web 服务中添加或移除方法后应更新此文件。
[!endif]

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h, StdAfx.cpp
这些文件用于生成预编译头 (PCH) 文件和预编译类型文件。

/////////////////////////////////////////////////////////////////////////////

其他注释：
[!if TODO]
 
应用程序向导使用“TODO:”指示应添加或自定义的源代码部分。
[!endif]

/////////////////////////////////////////////////////////////////////////////