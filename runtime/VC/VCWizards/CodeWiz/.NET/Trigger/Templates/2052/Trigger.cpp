#include "stdafx.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

// 为了调试 Trigger 程序，请将 t-sql 代码添加到 
// 将使 Trigger 激活的 debug.sql 文件中


public ref class AddNewTrigger
{
public:	
	// 为目标输入现有表或视图并取消对属性行的注释
	// [Microsoft::SqlServer::Server::SqlTrigger (Name="Trigger1", Target="Table1", Event="FOR UPDATE")]
    static void MyTrigger()
    {
        // 在此处放置代码
    }
};
