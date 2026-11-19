========================================================================
MICROSOFT 基础类库：[!output PROJECT_NAME] 项目概述
========================================================================


应用程序向导已为您创建了此 [!output PROJECT_NAME] DLL。此 DLL 不仅
演示了 Microsoft 基础类的基本使用方法，还可
作为您编写 DLL 的起点。

此文件概述了组成 [!output PROJECT_NAME] DLL 的每个文件
的内容。

[!output PROJECT_NAME].vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含有关生成该文件所使用的 Visual C++ 版本的信息，以及
有关在应用程序向导中选择的平台、配置和项目功能
的信息。

_[!if DLL_TYPE_EXTENSION]
[!output PROJECT_NAME].cpp
这是包含 DllMain() 定义的主 DLL 源
文件。
[!else]
[!output PROJECT_NAME].h
这是 DLL 的主头文件。它声明
[!output APP_CLASS] 类。

[!output PROJECT_NAME].cpp
这是主 DLL 源文件。它包含类 [!output APP_CLASS]。
[!if AUTOMATION]
它还包含需要 inproc 服务器具有的 OLE 入口点。
[!endif]
[!endif]
[!if AUTOMATION]

[!output PROJECT_NAME].idl
此文件包含 DLL 类型库的
对象描述语言源代码。
[!endif]

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
这是项目使用的所有 Microsoft Windows 资源的清单
。它包括存储在 RES 子目录中的图标、位图
和光标。如果 .rc 文件保持不变，数据节中的
定义将保持为它们所定义为的数值的十六进制版本，
而不是定义的友好名称。
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
这是项目使用的所有 Microsoft Windows 资源的清单
（当针对 Pocket PC 平台或
支持相同的用户界面模型的平台进行编译时）。它包括
存储在 RES 子目录中的图标、位图和光标。如果
.rc 文件保持不变，数据节中的
将作为定义将保持为它们所定义为的数值的十六进制版本，
而不是定义的友好名称。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
这是项目使用的所有 Microsoft Windows 资源的清单
（当针对 Smartphone 平台或
支持相同的用户界面模型的平台进行编译时）。它包括
存储在 RES 子目录中的图标、位图和光标。如果
.rc 文件保持不变，数据节中的
将作为定义将保持为它们所定义为的数值的十六进制版本，
而不是定义的友好名称。
[!endif]

[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
这是项目使用的所有 Microsoft Windows 资源的清单
（当针支持 AYGShell 用户界面模型的平台
进行编译时）。它包括 RES 子目录中存储的
图标、位图和光标。如果 .rc 文件保持不变，
数据节中的定义将保持为它们
所定义为的数值的十六进制版本，而不是定义的
友好名称。
[!endif]

res\[!output RC2_FILE_NAME].rc2
此文件包含不由 Microsoft 
Visual C++ 编辑的资源。您应将所有无法由资源编辑器编辑的资源
放置到此文件中。

[!output PROJECT_NAME].def
此文件包含关于在 Microsoft Windows 上运行所必需
的 DLL 的信息。它定义此 DLL 的
名称和说明等参数。它还从
此 DLL 导出函数。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件
和名为 StdAfx.obj 的预编译类型文件。

[!if STANDARDSHELL_UI_MODEL]
resource.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!if POCKETPC2003_UI_MODEL]
resourceppc.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
resourcesp.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!if AYGSHELL_UI_MODEL]
resourceayg.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。
[!endif]

/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”指示应添加到或自定义的源代码部分。
应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////