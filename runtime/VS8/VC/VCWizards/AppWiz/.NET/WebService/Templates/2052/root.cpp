// [!output PROJECT_NAME].cpp: 主项目文件。

#include "stdafx.h"
#include "[!output PROJECT_NAME]Class.h"
#include "Global.asax.h"


[!output SAFE_NAMESPACE_BEGIN]
	// WEB 服务示例
	// HelloWorld() 示例服务返回字符串“Hello, World!”。
	// 若要测试此 Web 服务，请确保部署路径中的 .asmx 文件
	// 在项目属性中设置为“调试 HTTP URL”。
	// 然后按 F5。
    
    String^ [!output SAFE_NAMESPACE_NAME]Class::HelloWorld()
    {

		// TODO: 在此处添加 Web 服务的实现

		return L"Hello World!";
        
    }
[!output SAFE_NAMESPACE_END]
