#include "stdafx.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

// 为了调试用户定义的类型(UDT)，请将下列内容添加到 debug.sql 文件中:
//
//	if object_id ('test_table') is not null
//		DROP TABLE test_table

//	CREATE TABLE test_table (col1 Type1)
//	go

//	INSERT INTO test_table VALUES (convert(Type1, 'Instantiation String 1'))
//	INSERT INTO test_table VALUES (convert(Type1, 'Instantiation String 2'))
//	INSERT INTO test_table VALUES (convert(Type1, 'Instantiation String 3'))

//	select col1.Method1() from test_table
//	select col1.var1 from test_table
//	select Type1::Method2()

[Serializable]
[Microsoft::SqlServer::Server::SqlUserDefinedType(Format::Native)]
public value struct Type1 : public SqlTypes::INullable
{
public:
	virtual String ^ToString() override
	{
		// TODO: 在此处放置代码
		return "";
	}
	property bool IsNull 
	{
		virtual bool get()
		{
			// TODO: 在此处放置代码
			return m_Null;
		}
	}
	property static Type1 Null
	{
		Type1 get()
		{
			Type1 ^h = gcnew Type1();
            h->m_Null = true;
			return *h;
		}
	}

	static Type1 Parse(SqlTypes::SqlString s)
	{
		if (s.IsNull)
			return Type1::Null;
		Type1 ^u = gcnew Type1();
		// TODO: 在此处放置代码
		return *u;
	}

	// 这是占位符方法
	System::String ^Method1()
	{
		// TODO: 在此处插入方法代码
		return L"Hello";
	}

	// 这是占位符静态方法
	static SqlTypes::SqlString Method2()
	{
		// TODO: 在此处插入方法代码
		SqlTypes::SqlString Value(L"Hello");
		return Value;
	}

	// 这是占位符字段成员
	int var1;

// 私有成员
private:
	bool m_Null;

};
