// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");
		if(wizard.FindSymbol("AUTOMATION"))
			wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));
		wizard.AddSymbol("RC2_FILE_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));

		selProj = CreateProject(strProjectName, strProjectPath);

		AddCommonConfig(selProj, strProjectName);
		AddSpecificConfig(selProj, strProjectName);
		selProj.Object.keyword = "MFCDLLProj";

		SetupFilters(selProj);

		AddFilesToProjectWithInfFile(selProj, strProjectName);

		SetCommonPchSettings(selProj);

		selProj.Object.Save();
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
	return false;
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		var strTarget = strName;
		if (strName.substr(0, 4) == "root")
		{
			if (strName == "root.ico")
			{
				strTarget = strResPath + "\\" + strProjectName + strName.substr(4);
			}
			else if (strName == "root.rc2")
			{
				var strRC2FileName = wizard.FindSymbol("RC2_FILE_NAME");
				strTarget = strResPath + "\\" + strRC2FileName + strName.substr(4);
			}
			else if (strName == "root.idl")
			{
				var strProjectName = wizard.FindSymbol("SAFE_IDL_NAME");
				strTarget = strProjectName + ".idl";
			}
			else
				strTarget = strProjectName + strName.substr(4);
			return strTarget;
		}

		switch (strName)
		{
			case "readme.txt":
				strTarget = "ReadMe.txt";
				break;
			case "all.rc":
			case "dlgall.rc":
				strTarget = strProjectName + ".rc";
				break;
			case "dlgres.h":
			case "resource.h":
				strTarget = "Resource.h";
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

function AddSpecificConfig(proj, strProjectName)
{
	try
	{
		var bAutomation = wizard.FindSymbol("AUTOMATION");
		var bRegular = wizard.FindSymbol("DLL_TYPE_REGULAR");
		var bRegularStatic = wizard.FindSymbol("DLL_TYPE_REGULAR_STATIC");

		var oConfigs = proj.Object.Configurations;
	for (var nCntr = 1; nCntr <= oConfigs.Count; nCntr++)
	{
		var config = oConfigs(nCntr);
		var bDebug = false;
		if (-1 != config.Name.indexOf("Debug"))
			bDebug = true;

		config.ConfigurationType = typeDynamicLibrary;
		config.CharacterSet = charSetUNICODE;

		var CLTool = config.Tools("VCCLCompilerTool");
		var strDefines = CLTool.PreprocessorDefinitions;
		if (strDefines != "") strDefines += ";";
		strDefines += GetPlatformDefine(config);
		strDefines += (bDebug ? "_WINDOWS;_DEBUG" : "_WINDOWS;NDEBUG");
		if (bRegular || bRegularStatic)
			strDefines += ";_USRDLL";
		else
			strDefines += ";_AFXEXT";

		if (bRegularStatic)
		{
			config.UseOfMFC = useMfcStatic;
			CLTool.RuntimeLibrary = (bDebug ? rtMultiThreadedDebug : rtMultiThreaded);
		}
		else
		{
			config.UseOfMFC = useMfcDynamic;
			CLTool.RuntimeLibrary = (bDebug ? rtMultiThreadedDebugDLL : rtMultiThreadedDLL);
		}

		CLTool.PreprocessorDefinitions = strDefines;
		CLTool.DebugInformationFormat = (bDebug ? debugEditAndContinue : debugEnabled);

		var MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = (bDebug ? "_DEBUG" : "NDEBUG");
		if (bAutomation)
		{
			var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");
			MidlTool.TypeLibraryName = "$(IntDir)/" + strIdlName +".tlb";
			if (IsPlatformWin32(config))
				MidlTool.TargetEnvironment = midlTargetWin32;
		}

		var RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = wizard.FindSymbol("LCID");
		RCTool.PreprocessorDefinitions = (bDebug ? "_DEBUG" : "NDEBUG");
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";
		
		var LinkTool = config.Tools("VCLinkerTool");
		LinkTool.GenerateDebugInformation = true;
		LinkTool.LinkIncremental = (bDebug ? linkIncrementalYes : linkIncrementalNo);
		var strDefFile = ".\\" + strProjectName + ".def";		
		LinkTool.ModuleDefinitionFile = strDefFile;

		if (bAutomation)
		{
			LinkTool.RegisterOutput = true;
		}
	} //for
	} //try
	catch(e)
	{
		throw e;
	}
}

