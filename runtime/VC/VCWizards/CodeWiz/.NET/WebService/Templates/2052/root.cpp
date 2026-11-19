// [!output FILE_NAME].cpp: 主项目文件。

#include "stdafx.h"
#include "[!output FILE_NAME].h"

using namespace System;
using namespace System::Web;
using namespace System::Web::Services;

[!output SAFE_NAMESPACE_BEGIN]
	// WEB 服务示例
	// 为此类添加您的方法及方法的实现。
	// 若要测试此 Web 服务，请确保部署路径中的 .asmx 文件
	// 在项目属性中设置为“调试 HTTP URL”，然后按 F5。
	// 下面给出了 HelloWorld 示例模板。

    String ^C[!output SAFE_ITEM_NAME]::HelloWorld()
    {

		// TODO: 在此处添加 Web 服务的实现

		return L"Hello World!";
        
    }
[!output SAFE_NAMESPACE_END]
