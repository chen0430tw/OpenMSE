#include "stdafx.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

// 为了调试 Stored Procedure 程序，请将下列内容添加到 debug.sql 文件中:
//
// exec MyStoredProc
// go

public ref class [!output SAFE_ITEM_NAME]
{
public:
	[Microsoft::SqlServer::Server::SqlProcedure]
	static void MyStoredProc()
	{
		// 在此处放置代码
	}
};
