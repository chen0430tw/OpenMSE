// Copyright (c) Microsoft Corporation. All rights reserved.

var ProjWiz;
ProjWiz = new ActiveXObject("ProjWiz.SDProjWiz2.2");


function OnFinish(selProj, selObj)
{
	try
	{
		wizard.AddSymbol("RC2_FILE_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");
		wizard.AddSymbol("RC2_FILE_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));

		EnsureDevicePlatforms();

		// this call must occur before we create the project, because otherwise the templates are already generated.
		SetDeviceSymbolsForPlatforms();

		selProj = CreateDeviceProject(strProjectName, strProjectPath);

		AddConfigurations(selProj, strProjectName);

		selProj.Object.keyword = "MFCDLLProj";

		SetupFilters(selProj);

		AddFilesToProjectWithInfFile(selProj, strProjectName);

		SetCommonPchSettings(selProj);

		SetDeviceResourceConfigurations(selProj.Object);

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
		var strRC2FileName = wizard.FindSymbol("RC2_FILE_NAME");
		var strTarget = strName;
		var strRC2FileName = wizard.FindSymbol("RC2_FILE_NAME");
		if (strName.substr(0, 4) == "root")
		{
			if (strName == "root.ico") 
			{
				strTarget = strResPath + "\\" + strProjectName + strName.substr(4);
			}
			else if(strName == "root.rc2" || strName == "rootppc.rc2" || strName == "rootsp.rc2" || strName == "rootayg.rc2")
			{
				strTarget = strResPath + "\\" + strRC2FileName + strName.substr(4);
			}
			else
			{
				strTarget = strProjectName + strName.substr(4);
			}
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

function AddConfigurations(proj, strProjectName)
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

		//Put all common definitions in strCommonDefinesForAllTools 
		var strCommonDefinesForAllTools = "";
		if (bDebug)
		{	
			strCommonDefinesForAllTools = "_DEBUG";
		}
		else
		{
			strCommonDefinesForAllTools = "NDEBUG";
		}
		strCommonDefinesForAllTools += ";_WIN32_WCE=$(CEVER);UNDER_CE"

		var PlatformDefines = "";
		PlatformDefines = config.Platform.GetMacroValue("PLATFORMDEFINES");
		if (PlatformDefines != "")
                {
			strCommonDefinesForAllTools += ";$(PLATFORMDEFINES)";
	        }

		config.DebugSettings.DebuggerFlavor = eDebuggerTypeLast;
		
		// set output directories
		config.IntermediateDirectory = "$(PlatformName)\\$(ConfigurationName)";
		config.OutputDirectory = "$(PlatformName)\\$(ConfigurationName)";

		config.ConfigurationType = typeDynamicLibrary;
		config.CharacterSet = charSetUnicode;

		var CLTool;
		var strDefines = strCommonDefinesForAllTools;
		strDefines += GetPlatformDefine(config);
		
		var instructionSet = GetInstructionSet(config.Platform.Name);
		var strBasePlatform = GetBasePlatformName(config.Platform.Name);

		CLTool = config.Tools("VCCLCompilerTool");
		if (instructionSet != "")
		{
			strDefines += ";WINCE";
		}

		if (ProjWiz.GetBaseNativePlatformSupportsDCOM(strBasePlatform))
		{
			strDefines += ";_CE_DCOM";
		}

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

		strDefines += ";$(ARCHFAM);$(_ARCHFAM_)"
		CLTool.PreprocessorDefinitions = strDefines;
		CLTool.DebugInformationFormat = debugEnabled;
		CLTool.WarningLevel = warningLevel_3;
		
		if(bDebug)
		{
			if (bRegularStatic)
			{
				CLTool.RuntimeLibrary = rtMultiThreadedDebug;
			}
			else
			{
				CLTool.RuntimeLibrary = rtMultiThreadedDebugDLL;
			}
		}
		else
		{
			if (bRegularStatic)
			{
				CLTool.RuntimeLibrary = rtMultiThreaded;
			}
			else
			{
				CLTool.RuntimeLibrary = rtMultiThreadedDLL;
			}
		}

		var MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = strCommonDefinesForAllTools;
		if (bAutomation)
		{
			MidlTool.TypeLibraryName = "$(IntDir)/" + "$(ProjectName).tlb";
			if (IsPlatformWin32(config))
				MidlTool.TargetEnvironment = midlTargetWin32;
		}

		var RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = wizard.FindSymbol("LCID");
		RCTool.PreprocessorDefinitions = strCommonDefinesForAllTools;
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";
		
		var LinkTool = config.Tools("VCLinkerTool");
		LinkTool.GenerateDebugInformation = true;
		if (bDebug)
		{
			LinkTool.LinkIncremental = linkIncrementalYes;
		}
		else
		{
			LinkTool.LinkIncremental = linkIncrementalNo;
		}
		var strDefFile = ".\\" + strProjectName + ".def";		
		LinkTool.ModuleDefinitionFile = strDefFile;
		LinkTool.OutputFile = "$(OutDir)/" + strProjectName + ".dll";
		LinkTool.ImportLibrary = "$(OutDir)/" + strProjectName + ".lib";

		LinkTool.StackReserveSize="65536";
		LinkTool.StackCommitSize="4096";

		if (config.Platform.Name != "Win32")
		{
			SetDeviceLinkLibSettings(config);
		}	
		
		if (config.Platform.Name == "Pocket PC 2003 (ARMV4)" ||
			config.Platform.Name == "SmartPhone 2003 (ARMV4)" ||
			config.Platform.Name == "Smartphone 2003 (ARMV4)" )
		{
			LinkTool.AdditionalOptions = LinkTool.AdditionalOptions + " /ARMPADCODE";
		}
		
		// do not delete the access of the deployment tool here, it initializes the device toolbar
		var DeployTool = config.DeploymentTool;
		if (DeployTool)
		{
			if (!bRegularStatic)
			{
				DeployTool.AdditionalFiles += "msvcr80.dll|$(BINDIR)\\$(INSTRUCTIONSET)\\|%CSIDL_PROGRAM_FILES%\\$(ProjectName)|0;";
				DeployTool.AdditionalFiles += "atl80.dll|$(BINDIR)\\$(INSTRUCTIONSET)\\|%CSIDL_PROGRAM_FILES%\\$(ProjectName)|0;";
				if (bDebug)
				{
					DeployTool.AdditionalFiles += "msvcr80d.dll|$(BINDIR)\\$(INSTRUCTIONSET)\\|%CSIDL_PROGRAM_FILES%\\$(ProjectName)|0;";
					DeployTool.AdditionalFiles += "MFC80UD.dll|$(BINDIR)\\$(INSTRUCTIONSET)\\|%CSIDL_PROGRAM_FILES%\\$(ProjectName)|0;";
				}
				else
				{
					DeployTool.AdditionalFiles += "MFC80U.dll|$(BINDIR)\\$(INSTRUCTIONSET)\\|%CSIDL_PROGRAM_FILES%\\$(ProjectName)|0;";
				}
			}
			if (bAutomation)
			{
				DeployTool.RegisterOutput = true;
			}
		}
	} //for
	} //try
	catch(e)
	{
		throw e;
	}
}

