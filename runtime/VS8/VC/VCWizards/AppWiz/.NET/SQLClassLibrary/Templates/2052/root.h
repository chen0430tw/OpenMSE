// [!output PROJECT_NAME].h

#pragma once

using namespace System;
using namespace System::Data;
using namespace System::Data::Sql;
using namespace System::Data::SqlTypes;
using namespace Microsoft::SqlServer::Server;

[!output SAFE_NAMESPACE_BEGIN]
	public ref class [!output SAFE_NAMESPACE_NAME]Class
	{
	public:
		[Microsoft::SqlServer::Server::SqlProcedure] static void MyStoredProc();
	};
[!output SAFE_NAMESPACE_END]
