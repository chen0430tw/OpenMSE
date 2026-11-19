#include "stdafx.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

// 为了调试 Aggregate 程序，请将下列内容添加到 debug.sql 文件中:
//
// if object_id ('test_table') is not null
//	DROP TABLE test_table

// CREATE TABLE test_table ( Column1 nvarchar(max))
// INSERT INTO test_table 值(N“a”)

// 从 test_table 中选择 dbo.Aggregate1(Column1)

[Serializable]
[Microsoft::SqlServer::Server::SqlUserDefinedAggregate(Format::Native)]
public value struct Aggregate1 
{    
public:
	void Init()
	{
		// TODO: 在此处放置代码
	}
	
	void Accumulate(SqlTypes::SqlString Value)
	{
		// TODO: 在此处放置代码
	}

	void Merge(Aggregate1 Group)
	{
		// TODO: 在此处放置代码
	}

	SqlTypes::SqlString Terminate()
	{
		SqlTypes::SqlString Value(L"");
		return Value;
	}

private:
    // 这是占位符成员
    int var1;
};
