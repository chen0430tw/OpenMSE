================================================================================
MICROSOFT 基础类库: [!output PROJECT_NAME] 项目概述
===============================================================================

应用程序向导已为您创建了这个 [!output PROJECT_NAME] 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 [!output PROJECT_NAME] 应用程序的每个文件的内容。

[!output PROJECT_NAME].vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

[!output APP_HEADER]
这是应用程序的主要头文件。它包括其他项目特定的头文件(包括 Resource.h)，并声明 [!output APP_CLASS] 应用程序类。

[!output APP_IMPL]
这是包含应用程序类 [!output APP_CLASS] 的主要应用程序源文件。

[!output PROJECT_NAME].rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 [!output LCID] 中。

res\[!output PROJECT_NAME].ico
这是用作应用程序图标的图标文件。此图标包括在主要资源文件 [!output PROJECT_NAME].rc 中。

res\[!output RC2_FILE_NAME].rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
这是一个示例 .reg 文件，它显示了框架将为您设置的注册设置的种类。可以将它用作
[!if APP_TYPE_DLG]
将与您的应用程序一起使用的 .reg 文件。
[!else]
将与您的应用程序一起使用的 .reg 文件，或者索性将它删除，而采用默认的 RegisterShellFileTypes 注册。
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
此文件包含应用程序类型库的接口描述语言源代码。
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

对于主框架窗口:
[!if PROJECT_STYLE_EXPLORER]
Windows 资源管理器样式: 项目将包含 Windows 资源管理器样式的界面，带有两个框架。
[!else]
项目包含标准 MFC 界面。
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
这些文件包含框架类 [!output MAIN_FRAME_CLASS]，该类派生自
[!if APP_TYPE_MDI]
CMDIFrameWnd 并控制所有 MDI 框架功能。
[!else]
CFrameWnd 并控制所有 SDI 框架功能。
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
这些文件包含左框架类 [!output TREE_VIEW_CLASS]，该类派生自
CTreeView。
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
此位图文件用于为工具栏创建平铺图像。
初始工具栏和状态栏在 [!output MAIN_FRAME_CLASS] 类中构造。使用资源编辑器编辑此工具栏位图，并更新 [!output PROJECT_NAME].rc 中的 IDR_MAINFRAME TOOLBAR 数组以添加工具栏按钮。
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
当服务器应用程序在另一个容器内部就地激活时，此位图文件用于为工具栏创建平铺图像。此工具栏在 [!output INPLACE_FRAME_CLASS] 类中构造。此位图类似于 res\Toolbar.bmp 中的位图，不同之处是它移除了许多非服务器命令。

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

对于子框架窗口:

ChildFrm.h，ChildFrm.cpp
这些文件定义并实现 [!output CHILD_FRAME_CLASS] 类，该类支持 MDI 应用程序中的子窗口。

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
应用程序向导创建一个文档类型和一个视图:

[!output DOC_HEADER]，[!output DOC_IMPL] - 文档
这些文件包含 [!output DOC_CLASS] 类。编辑这些文件可以添加特殊文档数据并可实现文件保存和加载(通过 [!output DOC_CLASS]::Serialize)。
[!if HAS_SUFFIX && !HTML_EDITVIEW]
文档将具有下列字符串:
文件扩展名:      [!output FILE_EXTENSION]
文件类型 ID:        [!output FILE_TYPE_ID]
主框架标题:  [!output MAIN_FRAME_CAPTION]
文档类型名称:       [!output DOC_TYPE_NAME]
筛选器名:         [!output FILTER_NAME]
文件的新简称: [!output FILE_NEW_NAME_SHORT]
文件类型全名: [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
应用程序向导创建一个视图:
[!endif]

[!output VIEW_HEADER]，[!output VIEW_IMPL] - 文档的视图
这些文件包含 [!output VIEW_CLASS] 类。
[!if !DB_VIEW_NO_FILE]
[!output VIEW_CLASS] 对象用于查看 [!output DOC_CLASS] 对象。
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
这是图标文件，它用作 [!output DOC_CLASS] 类的 MDI 子窗口的图标。此图标包括在主要资源文件 [!output PROJECT_NAME].rc 中。
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

数据库支持:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
这些文件包含 [!output ROWSET_CLASS] 类。此类用于访问您在向导中选择的数据源。
[!if DB_VIEW_NO_FILE]
将不添加序列化支持。
[!else]
已添加序列化支持。
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

应用程序向导还创建了特定于 OLE 的类

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
这些文件包含 [!output CONTAINER_ITEM_CLASS] 类。此类用于操作 OLE 对象。OLE 对象通常由 [!output VIEW_CLASS] 类显示并作为 [!output DOC_CLASS] 类的一部分序列化。
[!if ACTIVE_DOC_CONTAINER]
程序支持在其框架中包含活动文档。
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
这些文件包含 [!output SERVER_ITEM_CLASS]。此类用于将 [!output DOC_CLASS] 类连接到 OLE 系统，并可以选择提供指向文档的链接。
[!if ACTIVE_DOC_SERVER]
项目支持创建和管理活动文档。
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
这些文件包含 [!output INPLACE_FRAME_CLASS]。此类派生自 COleIPFrameWnd，用于在就地激活过程中控制所有框架功能。
[!endif]

[!if SUPPORT_COMPOUND_FILES]
项目支持复合文件。复合文件格式将包含一个或多个自动化对象的文档存储到一个文件中，同时仍然允许访问其中的单个对象。
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
应用程序向导创建一个对话框类和自动化代理类:
[!else]
应用程序向导创建一个对话框类:
[!endif]

[!output DIALOG_HEADER]，[!output DIALOG_IMPL] - 对话框
这些文件包含 [!output DIALOG_CLASS] 类。该类定义应用程序主对话框的行为。该对话框的模板位于 [!output PROJECT_NAME].rc 中，该文件可以在 Microsoft Visual C++ 中进行编辑。
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER]，[!output DIALOG_AUTO_PROXY_IMPL] - 自动化对象
这些文件包含 [!output DIALOG_AUTO_PROXY_CLASS] 类。此类称为对话框的自动化代理类，这是因为它负责公开自动化控制器可以用来访问对话框的自动化方法和属性。这些方法和属性不是从对话框类直接公开的，因为在基于模式对话框的 MFC 应用程序中，能够更清楚、更容易地将自动化对象与用户界面分离。
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

帮助支持:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
此文件是帮助项目文件。它包含将帮助文件编译为 .chm 文件所需的数据。

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
此文件列出帮助项目的内容。

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
此文件包含帮助主题的索引。

hlp\afxcore.htm
此文件包含标准 MFC 命令和屏幕对象的标准帮助主题。将您自己的帮助主题添加到此文件中。

[!if PRINTING]
hlp\afxprint.htm
此文件包含打印命令的帮助主题。

[!endif]
makehtmlhelp.bat
此文件由生成系统用来编译帮助文件。

hlp\Images\*.gif
这些是 Microsoft 基础类库标准命令的标准帮助文件主题所需的位图文件。

[!else]
hlp\[!output PROJECT_NAME].hpj
此文件是帮助编译器用来创建应用程序的帮助文件的帮助项目文件。

hlp\*.bmp
这些是 Microsoft 基础类库标准命令的标准帮助文件主题所需的位图文件。

hlp\*.rtf
这些文件包含标准 MFC 命令和屏幕对象的标准帮助主题。
[!endif]
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

其他功能:
[!if ACTIVEX_CONTROLS]

ActiveX 控件
应用程序包括对使用 ActiveX 控件的支持。
[!endif]
[!if PRINTING]

打印及打印预览支持
应用程序向导已通过从 MFC 库调用 CView 类中的成员函数，生成了用于处理打印、打印设置和打印预览命令的代码。
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

数据库支持
应用程序向导已为程序添加了基础级别的数据库支持。 
只包括了所需的文件。
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

拆分窗口
应用程序向导已为应用程序文档添加了拆分窗口支持。
[!endif]
[!if MAPI]

MAPI 支持
生成的项目包含创建、操作、传输和存储邮件所需的代码。
[!endif]
[!if SOCKETS]

Windows Sockets
应用程序支持通过 TCP/IP 网络建立通信。
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

其他标准文件:

StdAfx.h，StdAfx.cpp
这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

Resource.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	应用程序清单文件供 Windows XP 用来描述应用程序
	对特定版本并行程序集的依赖性。加载程序使用此
	信息从程序集缓存加载适当的程序集或
	从应用程序加载私有信息。应用程序清单可能为了重新分发而作为
	与应用程序可执行文件安装在相同文件夹中的外部 .manifest 文件包括，
	也可能以资源的形式包括在该可执行文件中。 
[!endif]
/////////////////////////////////////////////////////////////////////////////

其他注释:

应用程序向导使用“TODO:”指示应添加或自定义的源代码部分。
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

如果应用程序在共享的 DLL 中使用 MFC，则需要重新发布这些 MFC DLL；如果应用程序所用的语言与操作系统的当前区域设置不同，则还需要重新发布对应的本地化资源 MFC80XXX.DLL。有关这两个主题的更多信息，请参见 MSDN 文档中有关 Redistributing Visual C++ applications (重新发布 Visual C++ 应用程序)的章节。
[!endif]

/////////////////////////////////////////////////////////////////////////////
