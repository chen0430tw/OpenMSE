[!if CONSOLE_APP]
========================================================================
    控制台应用程序：[!output PROJECT_NAME] 项目概述
========================================================================

应用程序向导已为您创建了此 [!output PROJECT_NAME] 应用程序。  
[!endif]
[!if DLL_APP]
========================================================================
    动态链接库：[!output PROJECT_NAME] 项目概述
========================================================================

应用程序向导已为您创建了此 [!output PROJECT_NAME] DLL。  
[!endif]
[!if LIB_APP]
========================================================================
    静态库：[!output PROJECT_NAME] 项目概述
========================================================================

应用程序向导已为您创建了此 [!output PROJECT_NAME] 库项目。 
[!endif]
[!if WIN_APP]
========================================================================
    WIN32 应用程序：[!output PROJECT_NAME] 项目概述
========================================================================

应用程序向导已为您创建了此 [!output PROJECT_NAME] 应用程序。  
[!endif]

[!if !LIB_APP]
此文件概述了组成 [!output PROJECT_NAME] 应用程序的每个文件
的内容。
[!else]
[!if PRE_COMPILED_HEADER]
此文件概述了组成 [!output PROJECT_NAME] 应用程序的每个文件
的内容。
[!else]
没有为此项目创建源文件。
[!endif]
[!endif]


[!output PROJECT_NAME].vcproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
    它包含有关生成该文件所使用的 Visual C++ 版本的信息，以及
    有关在应用程序向导中选择的平台、配置和项目功能
    的信息。

[!if !EMPTY_PROJECT && !LIB_APP]
[!output PROJECT_NAME].cpp
[!if DLL_APP]
    这是主 DLL 源文件。
[!else]
    这是主应用程序源文件。
[!endif]
[!if DLL_APP && !EXPORT_SYMBOLS]

	此 DLL 在创建时不导出任何符号。因此，当它
	生成时不会产生 .lib 文件。如果希望此项目
	成为其他某个项目的项目依赖项，需要
	添加代码以从 DLL 导出某些符号，
	以便产生导出库，您也可以在项目“属性页”对话框中的
	“链接器”文件夹中，将“常规”属性页上的
	“忽略输入库”属性设置为“是”。
[!endif]
[!if WIN_APP || SUPPORT_MFC]

/////////////////////////////////////////////////////////////////////////////
应用程序向导创建了下列资源：

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    这是程序使用的所有 Microsoft Windows 资源的清单
    （当针对使用标准 Windows CE 外壳程序的平台
    进行编译时）。它包括存储在 RES 子目录中的图标、位图
    和光标。此文件可以直接在 Microsoft Visual C++
    中进行编辑。如果 .rc 文件保持不变，数据节中的定义
    将保持为它们所定义为的数值的十六进制版本，
    而不是定义的友好名称。

[!output RC2_FILE_NAME].rc2
    此文件包含不由 Microsoft 
    Visual C++ 编辑的资源。您应将所有无法由资源编辑器编辑的资源
    放置到此文件中。

Resource.h
    这是标准头文件，它定义新的资源 ID。
    Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    这是程序使用的所有 Microsoft Windows 资源的清单
    （当针对 Pocket PC 2003 平台或
    支持相同的用户界面模型的平台进行编译时）。它包括 RES 子目录中
    存储的图标、位图和光标。此文件可以
    直接在 Microsoft Visual C++ 中进行编辑。如果 .rc 文件保持不变，
    数据节中的定义将保持为
    它们所定义为的数值的十六进制版本，而不是定义的
    友好名称。

[!output RC2_FILE_NAME]ppc.rc2
    此文件包含不由 Microsoft 
    Visual C++ 编辑的资源。您应将所有无法由资源编辑器编辑的资源
    放置到此文件中。

Resourceppc.h
    这是标准头文件，它定义新的资源 ID。
    Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    这是程序使用的所有 Microsoft Windows 资源的清单
    （当针对 Smartphone 2003 平台或支持
    相同用户界面模型的平台进行编译时）。它
    包括 RES 子目录中存储的图标、位图和光标。  
    此文件可以直接在 Microsoft Visual C++ 中进行编辑。如果
    .rc 文件保持不变，数据节中的
    定义将保持为它们所定义为的数值的十六进制版本，
    而不是定义的友好名称。

[!output RC2_FILE_NAME]sp.rc2
    此文件包含不由 Microsoft 
    Visual C++ 编辑的资源。您应该将不可由资源编辑器编辑的所有资源
    放在此文件中。

Resourcesp.h
    这是标准头文件，它定义新的资源 ID。
    Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    这是程序使用的所有 Microsoft Windows 资源的清单
    （当针对支持 AYG 外壳程序的平台进行编译时）。
    它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。
    如果 .rc 文件保持不变，数据节中的定义
    将保持为它们所定义为的数值的十六进制版本，
    而不是定义的友好名称。

[!output RC2_FILE_NAME]ayg.rc2
    此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。

Resourceayg.h
    这是标准头文件，它定义新的资源 ID。
    Microsoft Visual C++ 读取并更新此文件。
[!endif]

[!endif]
[!if WIN_APP]

[!output PROJECT_NAME].ico
    这是用作应用程序图标 (32x32) 的图标文件。
    此图标包括在主资源文件 [!output PROJECT_NAME].rc 中。
[!endif]

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件
    和名为 StdAfx.obj 的预编译类型文件。

[!endif]
[!if LIB_APP]
[!if PRE_COMPILED_HEADER]

/////////////////////////////////////////////////////////////////////////////

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 [!output PROJECT_NAME].pch 的预编译头 (PCH) 文件
    和名为 StdAfx.obj 的预编译类型文件。

[!endif]
[!if SUPPORT_MFC]
/////////////////////////////////////////////////////////////////////////////
编译器和链接器开关已经过修改以支持 MFC。在此项目中使用 MFC ClassWizard 需要将几个文件添加到
项目中，包括“resource.h”、“[!output PROJECT_NAME].rc”和包含 resource.h 的
“[!output PROJECT_NAME].h”。如果将 rc 文件添加到静态库，您可能
会遇到困难，因为 Dll 或 Exe 中可能只存在
一个 rc 文件。解决此问题的方法可以是将库的 .rc 文件包含到
父项目的 .rc 文件中。

[!endif]
[!endif]
/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”注释指示
应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////s