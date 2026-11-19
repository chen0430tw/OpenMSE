#include "stdafx.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

// 为了调试 Aggregate 程序，请将下列内容添加到 debug.sql 文件中:
//
// select dbo.MyUDF()
// go


public ref class AddNewUDF 
{    
public:
	[Microsoft::SqlServer::Server::SqlFunction]
	static SqlTypes::SqlString MyUDF()
	{
		// 在此处放置代码
		SqlTypes::SqlString Value(L"Hello");
		return Value;
	}
};
