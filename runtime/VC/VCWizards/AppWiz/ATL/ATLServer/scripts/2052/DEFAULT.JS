// Copyright (c) Microsoft Corporation. All rights reserved.
var strDBString;

function CheckProjectName(selProj, selObj)
{
	if (wizard.FindSymbol("SOAP_WIZ") != "DoSoapWiz") return true;
	if (IsUnicodeString(wizard.FindSymbol("PROJECT_NAME")))
	{
		var L_NameNotAllowed_Text = "This project name is not allowed. Please chose another one."
		wizard.ReportError(L_NameNotAllowed_Text);
		return false;
	}
	return true;
}

function OnFinish(selProj, selObj)
{
	try
	{
		PatchWizardSymbols();
		var bGenIsapi = wizard.FindSymbol("GENERATE_ISAPI");
		var bGenApp = wizard.FindSymbol("GENERATE_APP");
		var bCombine = wizard.FindSymbol("COMBINE_PROJECTS");
		var bStencil = wizard.FindSymbol("STENCIL");
		
		var CurrentDate = new Date();
		var strYear = CurrentDate.getYear();
		wizard.AddSymbol("PROJECT_YEAR", strYear.toString());
		wizard.AddSymbol("DEPLOYMENT_PROJECT_NAME", wizard.FindSymbol("PROJECT_NAME")+"Deploy");

		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");

		wizard.AddSymbol("SAFE_PROJECT_HTTP_NAME", CreateSafeName(strProjectName));

		// before we do anything, check that the name given us isn't too long
		//
		var strTestPath = strProjectPath + "\\" + strProjectName;

		// 240 is MAX_PATH - space enough for ext and string that get appended
		// ("Deploy" or "Isapi")
		//
		if( strTestPath.length > 240 )
		{
			var L_ErrMsgPath_Text = "错误: 路径太长。请使用短一些的项目名称。";
			var oError = new Error(L_ErrMsgPath_Text);
			SetErrorInfo(oError);
			return;
		}
		
		if (bGenApp)
		{
			wizard.AddSymbol("GENERATING_APP", true);
			if (bCombine)
				wizard.AddSymbol("GENERATING_ISAPI", true);
			
			var strDllName;
			if (wizard.FindSymbol("SOAP"))
				strDllName = CreateSafeName(strProjectName);
			else
				strDllName = strProjectName;
		
			wizard.AddSymbol("DLL_NAME", strDllName);
			selProj = CreateProject(strProjectName, strProjectPath);        	
			
			AddCommonConfig(selProj, strProjectName);
			AddSpecificConfigs(selProj, strProjectName);
			
			SetupFilters(selProj);
			
			selProj.Object.keyword = "AtlServerProj";

			AddFilesToProjectWithInfFile(selProj, strProjectName);
			SetCommonPchSettings(selProj);
			selProj.Object.Save();
			wizard.AddSymbol("GENERATING_APP", false);
		}
	
		if (bGenIsapi && !bCombine)
		{
			wizard.AddSymbol("GENERATING_ISAPI", true);
			//save original project name and path
			wizard.AddSymbol("_PROJECT_NAME", strProjectName);
			wizard.AddSymbol("_PROJECT_PATH", strProjectPath);
			//adjust project name and path
			strProjectName = wizard.FindSymbol("ISAPI_PROJECT");
			strProjectPath = wizard.FindSymbol("ISAPI_DIRECTORY") +"\\" + strProjectName;

			wizard.AddSymbol("PROJECT_NAME", strProjectName);
			wizard.AddSymbol("PROJECT_PATH", strProjectPath);

			wizard.AddSymbol("DLL_NAME", strProjectName);
			
			//disable closing solution as we're adding an extra project
			var bCloseSolution = wizard.FindSymbol("CLOSE_SOLUTION");
			wizard.AddSymbol("CLOSE_SOLUTION", false);
			selProj = CreateProject(strProjectName, strProjectPath);
			//restore closing
			wizard.AddSymbol("CLOSE_SOLUTION", bCloseSolution);

			AddCommonConfig(selProj, strProjectName);
			AddSpecificConfigs(selProj, strProjectName);
			SetupFilters(selProj);

			AddFilesToProjectWithInfFile(selProj, strProjectName);
			
			SetCommonPchSettings(selProj);
			selProj.Object.Save();

			//restore original projectr name and path
			wizard.AddSymbol("PROJECT_NAME", wizard.FindSymbol("_PROJECT_NAME"));
			wizard.AddSymbol("PROJECT_PATH", wizard.FindSymbol("_PROJECT_PATH"));

			wizard.AddSymbol("GENERATING_ISAPI", false);
		}
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
	//UNDONE put_DeploymentContent() does not work (is not implemented on VCFile.Object)
	return; //so disable it for now, as Deployment is M2 workitem in projbld
	if (strName == "rootapp.srf")
	{
		projfile.DeploymentContent = true;
	}
	else if (strName == "root.disco")
	{
		projfile.DeploymentContent = true;
	}
	else if (strName == "root.htm")
	{
		projfile.DeploymentContent = true;
	}
}

function GetTargetName(strName, strProjectName)
{
	try
	{
		var strTarget = strName;
		switch (strName)
		{
			case "readme.txt":
				strTarget = "ReadMe.txt";
				break;
			case "root.cpp":
				strTarget = wizard.FindSymbol("PROJECT_NAME")+".cpp";
				break;
			case "rootapp.srf":
				strTarget = wizard.FindSymbol("PROJECT_NAME")+".srf";
				break;
			case "rootisapi.def":
				strTarget = wizard.FindSymbol("PROJECT_NAME")+".def";
				break;
			case "root.disco":
				strTarget = wizard.FindSymbol("PROJECT_NAME")+".disco";
				break;
			case "root.htm":
				strTarget = wizard.FindSymbol("SAFE_PROJECT_HTTP_NAME")+".htm";
				break;
			case "rootisapiextension.h":
				strTarget = wizard.FindSymbol("EXTENSION_FILE");
				break;
			case "root.rc":
				strTarget = wizard.FindSymbol("PROJECT_NAME")+".rc";
				break;
			case "root.h":
				strTarget = wizard.FindSymbol("APP_FILE");
				break;
			default:
				break;
		}
		return strTarget; 
	}
	catch(e)
	{
		throw e;
	}
}

function SetSpecificConfig(config, strProjectName, bIsDebug)
{
	var bAttributed = wizard.FindSymbol("ATTRIBUTED");
	var bSoap = wizard.FindSymbol("SOAP");
	var bStencil = wizard.FindSymbol("STENCIL");
   	var strSafeProjectHTTPName = wizard.FindSymbol("SAFE_PROJECT_HTTP_NAME");

	config.CharacterSet = charSetUNICODE;
	config.ConfigurationType = typeDynamicLibrary;
	config.UseOfATL = useATLDynamic;

	var CLTool = config.Tools("VCCLCompilerTool");

	var strDefines = CLTool.PreprocessorDefinitions;
	if (strDefines != "") strDefines += ";";
	strDefines += GetPlatformDefine(config);
	if(bIsDebug)
		strDefines += "_WINDOWS;_DEBUG;_USRDLL";
	else
		strDefines += "_WINDOWS;NDEBUG;_USRDLL";
	if (bAttributed || bSoap)
		strDefines += ";_ATL_ATTRIBUTES";
	CLTool.PreprocessorDefinitions = strDefines;

	var MidlTool = config.Tools("VCMidlTool");
	MidlTool.MkTypLibCompatible = false;

	var RCTool = config.Tools("VCResourceCompilerTool");
	RCTool.Culture = wizard.FindSymbol("LCID");

	var LinkTool = config.Tools("VCLinkerTool");
	LinkTool.GenerateDebugInformation = true;

	if(bIsDebug)
	{
		CLTool.RuntimeLibrary = rtMultiThreadedDebugDLL;
		CLTool.DebugInformationFormat = debugEditAndContinue;
		MidlTool.PreprocessorDefinitions = "_DEBUG";
		RCTool.PreprocessorDefinitions = "_DEBUG";
		LinkTool.LinkIncremental = linkIncrementalYes;
	}
	else
	{
		CLTool.RuntimeLibrary = rtMultiThreadedDLL;
		CLTool.DebugInformationFormat = debugEnabled;
		MidlTool.PreprocessorDefinitions = "NDEBUG";
		RCTool.PreprocessorDefinitions = "NDEBUG";
		LinkTool.LinkIncremental = linkIncrementalNo;
	}

	if (wizard.FindSymbol("GENERATING_ISAPI") || wizard.FindSymbol("COMBINE_PROJECTS"))
	{
		var strDefFile = ".\\" + strProjectName + ".def";
		LinkTool.ModuleDefinitionFile = strDefFile;
	}
	var strDllName = wizard.FindSymbol("DLL_NAME");
	LinkTool.OutputFile = "$(OutDir)\\" + strDllName + ".dll";
	if (wizard.FindSymbol("SESSION_DB") == true)
	{
		LinkTool.AdditionalDependencies = "msdasc.lib";
	}
	if (bSoap)
	{
		LinkTool.MergedIDLBaseFileName = "_"+strProjectName+".idl";
	}
	if (wizard.FindSymbol("GENERATE_DEPLOYMENT")
	&& (wizard.FindSymbol("GENERATING_APP") || wizard.FindSymbol("COMBINE_PROJECTS")))
	{
		// set debug settings
		
		var dbg = config.DebugSettings;
		dbg.DebuggerFlavor = eWebSvcDebugger;
		dbg.Remote = 0;
		var strUrl = "http://localhost/" + wizard.FindSymbol("ISAPI_VROOT") + "/";
		
		if (bSoap)
		{
			strUrl += strDllName+".dll?Handler=Gen" + strSafeProjectHTTPName + "WSDL";
		}
		else if (wizard.FindSymbol("STENCIL"))
		{
			strUrl += strProjectName + ".srf";
		}
		else
		{
			strUrl += strDllName+".dll?Handler=Default";
		}		
		dbg.HttpUrl = strUrl;
	}
	if (wizard.FindSymbol("GENERATE_DEPLOYMENT"))
	{
		var DeploymentTool = config.Tools("VCWebDeploymentTool");
		DeploymentTool.ExcludedFromBuild = false;
		DeploymentTool.RelativePath = "";
		DeploymentTool.VirtualDirectoryName = wizard.FindSymbol("ISAPI_VROOT");
		DeploymentTool.ApplicationProtection = 0; // eAppProtectLow
		DeploymentTool.UnloadBeforeCopy = true;
		
		if (wizard.FindSymbol("GENERATING_ISAPI") || wizard.FindSymbol("COMBINE_PROJECTS"))
		{
			DeploymentTool.ApplicationMappings = ".srf;.dll";
			if (wizard.FindSymbol("PERFMON"))
			{
				DeploymentTool.RegisterOutput = true;
			}
		}
		
	}
}

function AddSpecificConfigs(proj, strProjectName)
{
	try
	{
		var configs = proj.Object.Configurations;
		
		var config = configs.Item("Debug");
		SetSpecificConfig(config, strProjectName, true);
		config = configs.Item("Release");
		SetSpecificConfig(config, strProjectName, false);
		
	}
	catch(e)
	{
		throw e;
	}
}

function PatchWizardSymbols()
{
	try
	{
		// for keeping track of debugging comments in the templates
		wizard.AddSymbol("__dbgcmt", false);
		
		if (wizard.FindSymbol("COMBINE_PROJECTS"))
		{
			wizard.AddSymbol("GENERATE_ISAPI", true);
			wizard.AddSymbol("GENERATE_APP", true);
		}
		
		if (wizard.FindSymbol("SOAP"))
		{
			wizard.AddSymbol("STENCIL", false);
			wizard.AddSymbol("VALIDATION", false);

			var strGuid = wizard.CreateGuid();
			wizard.AddSymbol("SOAP_INTERFACE_GUID", wizard.FormatGuid(strGuid, Format1));

			strGuid = wizard.CreateGuid();
			wizard.AddSymbol("SOAP_COCLASS_GUID", wizard.FormatGuid(strGuid, Format1));

			wizard.AddSymbol("ATTRIBUTED", true);
		}
		
		if (wizard.FindSymbol("SOME_SESSION") == false)
		{
			wizard.AddSymbol("SESSION_DB", false);
			wizard.AddSymbol("SESSION_MEM", false);
		}
		
		if (wizard.FindSymbol("PERFMON") == true && wizard.FindSymbol("GENERATE_ISAPI") == false)
		{
			wizard.AddSymbol("PERFMON", false);
		}
		
		if (wizard.FindSymbol("SESSION_DB") == true)
		{
			var strConnectString = wizard.FindSymbol("DB_CONNECTION_STRING");
			if (strConnectString.length == 0)
				return;

			GetDBString(strConnectString);
		}
		
		if (!wizard.FindSymbol("SESSION_MEM") && !wizard.FindSymbol("SESSION_DB") &&
			!wizard.FindSymbol("FILE_CACHE") && !wizard.FindSymbol("BLOB_CACHE") &&
			!wizard.FindSymbol("DATASOURCE_CACHE") && !wizard.FindSymbol("BROWSCAPS"))
		{
			wizard.AddSymbol("EXTENSION_CLASS", "CIsapiExtension")
		}
		
		if (wizard.FindSymbol("GENERATE_ISAPI"))
		{
			var strVal = wizard.FindSymbol("ISAPI_DIRECTORY");
			var strNewVal = TrimStr(strVal);
			wizard.AddSymbol("ISAPI_DIRECTORY", strNewVal);
			
			strVal = wizard.FindSymbol("ISAPI_PROJECT");
			strNewVal = TrimStr(strVal);
			wizard.AddSymbol("ISAPI_PROJECT", strNewVal);
		}
		
		wizard.AddSymbol("GENERATING_APP", false);
		wizard.AddSymbol("GENERATING_ISAPI", false);
		var strRemoteMachine = "localhost";
		try
		{
			strRemoteMachine = wizard.GetComputerName();
		}
		catch (e)
		{
			strRemoteMachine = "localhost";
		}
		
		wizard.AddSymbol("REMOTE_MACHINE", strRemoteMachine);
	}
	catch(e)
	{
		throw e;
	}
}

function GetDBString(str)
{
	try
	{
		var i = 0;
		strDBString = "";
		for (i = 0; i < str.length; i++)
		{
			if (str.charAt(i) == '\"' || str.charAt(i) == '\\')
			{
				strDBString+= '\\';
			}
			strDBString+= str.charAt(i);
		}
		wizard.AddSymbol("DB_CONNECTION_STRING", strDBString);
	}
	catch(e)
	{
		throw e;
	}
}

