================================================================================
MICROSOFT 基础类库: [!output PROJECT_NAME] 项目概述
===============================================================================

应用程序向导已为您创建了这个 [!output PROJECT_NAME] 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 [!output PROJECT_NAME] 应用程序的每个文件的内容。

[!output PROJECT_NAME].vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

[!output APP_HEADER]
这是应用程序的主要头文件。它包括其他项目特定的头文件并声明 [!output APP_CLASS] 应用程序类。

[!output APP_IMPL]
这是包含应用程序类 [!output APP_CLASS] 的主要应用程序源文件。

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
这是项目的主要资源文件，列出项目使用的所有 Microsoft Windows 资源。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 [!output LCID] 中。
如果 .rc 文件保留不变，数据节中的定义将保留为它们所定义为的数值的十六进制版本，而不是定义的友好名称。

res\[!output RC2_FILE_NAME].rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
这是项目的主要资源文件，列出项目在针对 Pocket PC 平台或支持相同用户界面模型的平台进行编译时使用的所有 Microsoft Windows 资源。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 [!output LCID] 中。如果 .rc 文件保留不变，数据节中的定义将保留为它们所定义为的数值的十六进制版本，而不是定义的友好名称。

res\[!output RC2_FILE_NAME]ppc.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
这是项目的主要资源文件，列出项目在针对 Smartphone 平台或支持相同用户界面模型的平台进行编译时使用的所有 Microsoft Windows 资源。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 [!output LCID] 中。如果 .rc 文件保留不变，数据节中的定义将保留为它们所定义为的数值的十六进制版本，而不是定义的友好名称。

res\[!output RC2_FILE_NAME]sp.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
这是项目的主要资源文件，列出项目在针对支持 AYGShell 用户界面模型的平台进行编译时使用的所有 Microsoft Windows 资源。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 [!output LCID] 中。
如果 .rc 文件保留不变，数据节中的定义将保留为它们所定义为的数值的十六进制版本，而不是定义的友好名称。

res\[!output RC2_FILE_NAME]ayg.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。
[!endif]

res\[!output PROJECT_NAME].ico
这是用作应用程序图标的图标文件。此图标包括在主要资源文件中。

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
这是一个示例 .reg 文件，它显示了框架将为您设置的注册设置的种类。可以将它用作
[!if APP_TYPE_DLG]
将与您的应用程序一起使用的 .reg 文件。
[!else]
将与您的应用程序一起使用的 .reg 文件，或者索性将它删除，而采用默认的 RegisterShellFileTypes 注册。
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

对于主框架窗口:
项目包含标准 MFC 界面。

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
这些文件包含框架类 [!output MAIN_FRAME_CLASS]，该类是从 CFrameWnd 派生的，它控制所有 SDI 框架功能。

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
此位图文件用于为命令栏创建平铺图像。
初始命令栏是在 [!output MAIN_FRAME_CLASS] 类中构造的。 
将此 cmdbar 位图与 [!output MAIN_FRAME_IMPL] 中的数组一起编辑可以添加更多命令栏按钮。
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
应用程序向导创建一个文档类型和一个视图:

[!output DOC_HEADER]，[!output DOC_IMPL] - 文档
这些文件包含 [!output DOC_CLASS] 类。编辑这些文件可以添加特殊文档数据并可实现文件保存和加载(通过 [!output DOC_CLASS]::Serialize)。
[!if HAS_SUFFIX]
文档将具有下列字符串:
文件扩展名:      [!output FILE_EXTENSION]
文件类型 ID:        [!output FILE_TYPE_ID]
主框架标题:  [!output MAIN_FRAME_CAPTION]
文档类型名称:       [!output DOC_TYPE_NAME]
筛选器名:         [!output FILTER_NAME]
文件的新简称: [!output FILE_NEW_NAME_SHORT]
文件类型全名: [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER]，[!output VIEW_IMPL] - 文档的视图
这些文件包含 [!output VIEW_CLASS] 类。
[!output VIEW_CLASS] 对象用于查看 [!output DOC_CLASS] 对象。

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

应用程序向导创建一个对话框类:

[!output DIALOG_HEADER]，[!output DIALOG_IMPL] - 对话框
这些文件包含 [!output DIALOG_CLASS] 类。该类定义应用程序主对话框的行为。该对话框的模板位于主要资源文件中，该文件可以在 Microsoft Visual C++ 中进行编辑。
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

帮助支持:

hlp\[!output PROJECT_NAME].hpj
此文件是帮助编译器用来创建应用程序的帮助文件的帮助项目文件。

hlp\*.bmp
这些是 Microsoft 基础类库标准命令的标准帮助文件主题所需的位图文件。

hlp\*.rtf
这些文件包含标准 MFC 命令和屏幕对象的标准帮助主题。
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

其他功能:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

具有菜单和修饰的主框架中的命令栏
[!else]
[!if CMD_BAR_MENUNBUTTON]
具有菜单、按钮和修饰的主框架中的菜单栏
[!endif]
[!endif]
[!if STATUS_BAR]
主框架中的初始状态栏
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

Windows CE 帮助支持
[!endif]
[!if DYNAMIC_MFC]

使用共享 DLL 实现
[!endif]
[!if ACTIVEX_CONTROLS]

ActiveX 控件
应用程序包括对使用 ActiveX 控件的支持。
[!endif]
[!if PRINTING]

打印及打印预览支持
应用程序向导已通过从 MFC 库调用 CView 类中的成员函数，生成了用于处理打印、打印设置和打印预览命令的代码。
[!endif]
[!if SOCKETS]

Windows Sockets
应用程序支持通过 TCP/IP 网络建立通信。
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

其他标准文件:

StdAfx.h, StdAfx.cpp
这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

[!if STANDARDSHELL_UI_MODEL]
Resource.h
[!endif]
[!if POCKETPC2003_UI_MODEL]
Resourceppc.h
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
Resourcesp.h
[!endif]
[!if AYGSHELL_UI_MODEL]
Resourceayg.h
[!endif]
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。

/////////////////////////////////////////////////////////////////////////////

其他注释:

应用程序向导使用“TODO:”来指示
应添加或自定义的源代码部分。
[!if APP_TYPE_SDI || APP_TYPE_DLG]

如果应用程序使用共享 DLL 中的 MFC，并且它使用的是与操作系统的当前语言不同的语言，那么您需要将对应的本地化资源 MFC80XXX.DLL 复制到应用程序目录中(“XXX”表示语言缩写。例如，MFC80DEU.DLL 包含翻译为德语的资源。)如果您不执行此操作，则应用程序的某些用户界面元素将保持与操作系统相同的语言。
[!endif]

/////////////////////////////////////////////////////////////////////////////
