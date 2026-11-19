========================================================================
ACTIVEX 控件 DLL：[!output PROJECT_NAME] 项目概述
========================================================================

控件向导已为您的 [!output PROJECT_NAME] ActiveX 控件 DLL 创建了
此项目，其中包含一个控件。

此主干项目不仅演示编写 ActiveX 控件的
基本方法，还可作为您编写控件的特定功能的
起点。

此文件概述了组成 [!output PROJECT_NAME] ActiveX 控件 DLL 的
每个文件的内容。

[!output PROJECT_NAME].vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目
文件。它包含生成该文件的 Visual C++ 的版本
信息，以及有关使用应用程序向导选择的平台、
配置和项目功能的信息。

[!output PROJECT_NAME].h
这是 ActiveX 控件 DLL 的主包含文件。它
包括其他项目特定的包含文件。

[!output PROJECT_NAME].cpp
这是包含 DLL 初始化、终止和其他簿记的代码的
主源文件。

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
这是项目使用的所有 Microsoft Windows 资源的清单
。如果 .rc 文件保持不变，数据节中的定义
将保持为它们所定义为的数值的十六进制版本，
而不是定义的友好名称。
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

[!output PROJECT_NAME].def
此文件包含关于在 Microsoft Windows 上运行所必需的
的 ActiveX 控件 DLL 的信息。

[!output SAFE_IDL_NAME].idl
此文件包含控件类型库的
对象描述语言源代码。

[!if !SMARTPHONE2003_UI_MODEL]
[!if ABOUT_BOX]
[!output PROJECT_NAME].ico
此文件包含将出现在“关于”框中的图标。此图标
包含在主资源 (.rc) 文件中

[!endif]
[!endif]
/////////////////////////////////////////////////////////////////////////////
[!output CONTROL_CLASS] 控件：

[!output CONTROL_HEADER]
此文件包含 [!output CONTROL_CLASS] C++ 类的声明。

[!output CONTROL_IMPL]
此文件包含 [!output CONTROL_CLASS] C++ 类的实现。

[!output PROPERTY_PAGE_HEADER]
此文件包含 [!output PROPERTY_PAGE_CLASS] C++ 类的声明。

[!output PROPERTY_PAGE_IMPL]
此文件包含 [!output PROPERTY_PAGE_CLASS] C++ 类的实现。

[!output CONTROL_CLASS].bmp
此文件包含一个位图，当 [!output CONTROL_CLASS] 控件出现在工具调板中时，
容器将使用该位图来表示此控件。此位图
包含在主资源 (.rc) 文件中

[!if HELP_FILES]
/////////////////////////////////////////////////////////////////////////////
帮助支持：

[!output PROJECT_NAME].hpj
此文件是帮助编译器用来创建 ActiveX 控件帮助文件的
帮助项目文件。

*.bmp
这些是 Microsoft 基础类库标准命令的标准帮助文件主题
所需的位图文件。这些文件
位于 HLP 子目录中。

[!output PROJECT_NAME].rtf
此文件包含许多 ActiveX 控件支持的公共属性、事件和方法的
标准帮助主题。您可以编辑
此文件以添加或移除其他控件特定主题。此文件
位于 HLP 子目录中。

[!endif]
[!if RUNTIME_LICENSE]
/////////////////////////////////////////////////////////////////////////////
授权支持：

[!output PROJECT_NAME].lic
这是用户许可证文件。此文件必须与控件的 DLL 位于
同一个目录中，以便在设计时环境中
创建控件的一个实例。您通常会将
此文件与控件一同分发，但您的客户不会将其分发。

[!endif]
/////////////////////////////////////////////////////////////////////////////
其他标准文件：

stdAfx.h，stdAfx.cpp
这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件
和名为 stdafx.obj 的预编译类型 (PCT) 文件。

[!if STANDARDSHELL_UI_MODEL]
resource.h
这是标准头文件，它定义新的资源 ID。
Visual C++ 资源编辑器读取并更新此文件。
[!endif]

[!if POCKETPC2003_UI_MODEL]
resourceppc.h
这是标准头文件，它定义新的资源 ID。
Visual C++ 资源编辑器读取并更新此文件。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
resourcesp.h
这是标准头文件，它定义新的资源 ID。
Visual C++ 资源编辑器读取并更新此文件。
[!endif]

[!if AYGSHELL_UI_MODEL]
resourceayg.h
这是标准头文件，它定义新的资源 ID。
Visual C++ 资源编辑器读取并更新此文件。
[!endif]

/////////////////////////////////////////////////////////////////////////////
其他注释：

控件向导使用“TODO:”指示应添加或自定义的源代码部分。
应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////