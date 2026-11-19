// [!output PROJECT_NAME].cpp: 主 Windows 服务项目文件。

#include "stdafx.h"
#include <string.h>
#include "[!output PROJECT_NAME]WinService.h"

using namespace [!output SAFE_NAMESPACE_NAME];
using namespace System::Text;
using namespace System::Security::Policy;
using namespace System::Reflection;

//要安装/卸载服务，请键入:“[!output PROJECT_NAME].exe -Install [-u]”
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc >= 2)
	{
		if (argv[1][0] == _T('/'))
		{
			argv[1][0] = _T('-');
		}

		if (_tcsicmp(argv[1], _T("-Install")) == 0)
		{
			array<String^>^ myargs = System::Environment::GetCommandLineArgs();
			array<String^>^ args = gcnew array<String^>(myargs->Length - 1);

			// 用到达程序集的完整路径设置 args[0]，
			Assembly^ assem = Assembly::GetExecutingAssembly();
			args[0] = assem->Location;

			Array::Copy(myargs, 2, args, 1, args->Length - 1);
			AppDomain^ dom = AppDomain::CreateDomain(L"execDom");
			Type^ type = System::Object::typeid;
			String^ path = type->Assembly->Location;
			StringBuilder^ sb = gcnew StringBuilder(path->Substring(0, path->LastIndexOf(L"\\")));
			sb->Append(L"\\InstallUtil.exe");
			Evidence^ evidence = gcnew Evidence();
			dom->ExecuteAssembly(sb->ToString(), evidence, args);
		}
	}
	else
	{
		ServiceBase::Run(gcnew [!output SAFE_NAMESPACE_NAME]WinService());
	}
}
