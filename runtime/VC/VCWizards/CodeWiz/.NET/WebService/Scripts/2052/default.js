// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
	wizard.AddSymbol("NATIVE_PROJECT",IsNativeProject(selProj));
	try
	{
		if (!IsMCppProject(selProj))
			return VS_E_WIZARDBACKBUTTONPRESS;
		if(!PrepareToAddManagedClass(selProj))
			return VS_E_WIZARDBACKBUTTONPRESS;
		if(!IsWebProject(selProj))
		{
			// Error already shown in IsWebProject
			return VS_E_WIZARDBACKBUTTONPRESS;
		}

		CheckAddtoProject(selProj);

		PatchWizardSymbols();
		var strSafeNamespaceName = wizard.FindSymbol("SAFE_NAMESPACE_NAME");
		var oElement = selProj.CodeModel.Namespaces.Find(strSafeNamespaceName);
		if (oElement)
		{
			var strItemName = wizard.FindSymbol("ITEM_NAME");
			var L_ObjectExist_Text = " 已在该项目中找到。请选择另一个名称。";
			wizard.ReportError(strItemName + L_ObjectExist_Text);
			return VS_E_WIZARDBACKBUTTONPRESS;
		}
		AddReferencesForWebService(selProj);
		AddFilesToProjectWithInfFile(selProj, wizard.FindSymbol("PROJECT_NAME"), selObj);
	}
	catch(e)
	{
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}
function SetFileProperties(projfile, strName)
{
	if (strName == "root.asmx")
	{
		projfile.Object.DeploymentContent = true;
	}
	else if (strName == "root.h")
	{
		projfile.Object.FileType = eFileTypeCppWebService;
	} else if (strName == "root.cpp") {
		if (wizard.FindSymbol("NATIVE_PROJECT")) {
			SetFileLevelClr(projfile);
		}
	}

	return false;
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		var strTarget = strName;
		if (strName.substr(0, 4) == "root")
		{
			strTarget = wizard.FindSymbol("FILE_NAME") + strName.substr(4);
		}
		return strTarget;
	}
	catch(e)
	{
		throw e;
	}
}

function StripPath(strFullPath)
{
	try
	{
		var nIndex = strFullPath.lastIndexOf("\\", strFullPath.length);
		if (nIndex != -1)
		{
			return strFullPath.substring(nIndex+1, strFullPath.length);
		}

		return strFullPath;
	}
	catch (e)
	{
		throw e;
	}
}

function CheckAddtoProject(oProj)
{
	wizard.AddSymbol("COMMENTS_MANAGED_SWITCHES","");
	try
	{
		var nTotal = oProj.Object.Configurations.Count;
		for (var nCntr = 1; nCntr <= nTotal; nCntr++)
		{
			var oConfig = oProj.Object.Configurations(nCntr);
			if (oConfig.ManagedExtensions == false)
			{
				var L_MANAGEDSETTINGS_TEXT = "/*\n对包括此文件的文件检查以下属性设置。\n";
				var L_MANAGEDSETTINGS1_TEXT = "1. \"调试信息格式\"应设置为 \"Zi\"\n";
				L_MANAGEDSETTINGS_TEXT = L_MANAGEDSETTINGS_TEXT + L_MANAGEDSETTINGS1_TEXT;
				L_MANAGEDSETTINGS2_TEXT = "2. \"编译为托管\"应设置为 \"程序集(\\CLR)\"\n";
				L_MANAGEDSETTINGS_TEXT = L_MANAGEDSETTINGS_TEXT + L_MANAGEDSETTINGS2_TEXT;
				L_MANAGEDSETTINGS3_TEXT = "3. \"启用最小重新生成\"应设置为 \"No\"\n";
				L_MANAGEDSETTINGS_TEXT = L_MANAGEDSETTINGS_TEXT + L_MANAGEDSETTINGS3_TEXT;
				L_MANAGEDSETTINGS4_TEXT = "4. \"基本运行时检查\"应设置为 \"默认\"\n";
				L_MANAGEDSETTINGS_TEXT = L_MANAGEDSETTINGS_TEXT + L_MANAGEDSETTINGS4_TEXT;
				L_MANAGEDSETTINGS5_TEXT = "5. \"运行时库\"应设置为 \"多线程 DLL (\\MD)\"\n";
				L_MANAGEDSETTINGS_TEXT = L_MANAGEDSETTINGS_TEXT + L_MANAGEDSETTINGS5_TEXT;
				L_MANAGEDSETTINGS6_TEXT = "6. 可能必须对预编译头设置进行某些修改。\n*/\n";
				L_MANAGEDSETTINGS_TEXT = L_MANAGEDSETTINGS_TEXT + L_MANAGEDSETTINGS6_TEXT;
				wizard.AddSymbol("COMMENTS_MANAGED_SWITCHES", L_MANAGEDSETTINGS_TEXT);

				return false;
			}
		}
		return true;

	}
	catch (e)
	{
		return false;
	}
}

function PatchWizardSymbols()
{
	try
	{
		var strProjectName		= wizard.FindSymbol("PROJECT_NAME");

		var strProjectPath		= wizard.FindSymbol("PROJECT_PATH");
		var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");

		var strSafeItemName     = CreateIdentifierSafeName(StripPath(wizard.FindSymbol("ITEM_NAME")));
		var strFileName		= StripPath(GetUniqueFileName(strProjectPath, strSafeItemName));

		wizard.AddSymbol("SAFE_ITEM_NAME", strSafeItemName);
		wizard.AddSymbol("SAFE_NAMESPACE_NAME", CreateCPPName(strSafeItemName));
		wizard.AddSymbol("FILE_NAME", strFileName);
	}
	catch (e)
	{
		throw e;
	}
}

function IsWebProject(selProj)
{
	try
	{
		var L_NotManaged_Text = "托管 Web 服务只能添加到托管 Web 项目中";

		var config = selProj.Object.Configurations(1);
		if(!config) {
			wizard.ReportError (L_NotManaged_Text);
			return false;
		}
		var WebDeployConfig = config.Tools("VCWebDeploymentTool");
		if(!WebDeployConfig) {
			wizard.ReportError (L_NotManaged_Text);
			return false;
		}
		//web projects must have their deployment tool included in the build
		if (WebDeployConfig.ExcludedFromBuild) {
		        var L_QUESTION_Text = "要将 Web 服务添加到此项目中，必须启用 Web 部署。要这样做吗?";
                if (wizard.YesNoAlert(L_QUESTION_Text))
				{
				    WebDeployConfig.ExcludedFromBuild = false;
					return true;
				}
				// No error message -- the dialog above was enough
				return false;
        }
        return true;
	}
	catch (e)
	{
		throw e;
	}
}
