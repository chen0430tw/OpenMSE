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
		wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));

		EnsureDevicePlatforms()

		// this call must occur before we create the project, because otherwise the templates are already generated.
		SetDeviceSymbolsForPlatforms();

		selProj = CreateDeviceProject(strProjectName, strProjectPath);

		AddConfigurations(selProj, strProjectName);

		selProj.Object.keyword = "MFCActiveXProj";

		SetupFilters(selProj);

		if (IsDBCSCharSet(wizard))
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "9"); //VS-supported codedepages that require FontSize 9
		else
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "8"); //all the rest and unsupported codepages: Fontsize 8

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
			if (strName == "root.idl")
			{
				var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");
				strTarget = strIdlName + ".idl";
			}
			else if (strName == "root.rc2")
			{
				strTarget = strRC2FileName + strName.substr(4);
			}
			else
			{
			    strTarget = strProjectName + strName.substr(4);
			    return strTarget;
			}
		}

		switch (strName)
		{
			case "readme.txt":
				strTarget = "ReadMe.txt";
				break;
			case "resource.h":
				strTarget = "Resource.h";
				break;
			case "ctl.h":
				strTarget = wizard.FindSymbol("CONTROL_HEADER");
				break;
			case "ctl.cpp":
				strTarget = wizard.FindSymbol("CONTROL_IMPL");
				break;
			case "ppg.h":
				strTarget = wizard.FindSymbol("PROPERTY_PAGE_HEADER");
				break;
			case "ppg.cpp":
				strTarget = wizard.FindSymbol("PROPERTY_PAGE_IMPL");
				break;
			case "ctl.bmp":
				var strControlName = wizard.FindSymbol("SHORT_NAME");
				strTarget =  strControlName + "Ctrl.bmp";
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

		var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");

		config.DebugSettings.DebuggerFlavor = eDebuggerTypeLast;

		config.ConfigurationType = typeDynamicLibrary;
		config.UseOfMFC = useMfcStatic;
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

		config.IntermediateDirectory = "$(PlatformName)\\$(ConfigurationName)";
		config.OutputDirectory = "$(PlatformName)\\$(ConfigurationName)";
		if(bDebug)
		{
			strDefines += "_WINDOWS;_USRDLL"; //_DEBUG or NDEBUG are pade part of strCommonDefinesForAllTools
			CLTool.RuntimeLibrary = rtMultiThreadedDebug;
			CLTool.Optimization = optimizeDisabled;
		}			
		else
		{
			strDefines += "_WINDOWS;_USRDLL";	
			CLTool.MinimalRebuild = false;
			CLTool.RuntimeLibrary = rtMultiThreaded;
			CLTool.Optimization = optimizeMaxSpeed;
		}

		strDefines += ";$(ARCHFAM);$(_ARCHFAM_)"

		CLTool.DebugInformationFormat = debugEnabled;
		CLTool.PreprocessorDefinitions= strDefines;
		CLTool.WarningLevel = warningLevel_3;

		var MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = strCommonDefinesForAllTools;
		MidlTool.TypeLibraryName = "$(IntDir)/" + strIdlName + ".tlb";
		MidlTool.HeaderFileName = strIdlName + "idl.h";

		var RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = wizard.FindSymbol("LCID");
		RCTool.PreprocessorDefinitions = strCommonDefinesForAllTools;
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";
		
		var LinkTool = config.Tools("VCLinkerTool");
		
		if (bDebug)
		{
			LinkTool.LinkIncremental = linkIncrementalYes;
		}
		else
		{
			LinkTool.LinkIncremental = linkIncrementalNo;
		}

		// [ISSUE] need to change to subSystemWindowsCE.
		LinkTool.SubSystem = subSystemNotSet;
		
		LinkTool.GenerateDebugInformation = true;
		var strDefFile = ".\\" + strProjectName + ".def";
		LinkTool.ModuleDefinitionFile = strDefFile;
		LinkTool.OutputFile = "$(OutDir)/" + strProjectName + ".ocx";
		LinkTool.ImportLibrary = "$(OutDir)/" + strProjectName + ".lib";
		LinkTool.RegisterOutput = false;

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
			DeployTool.RegisterOutput = true;
		}

		if (wizard.FindSymbol("RUNTIME_LICENSE"))
		{
			var PostBuildTool = config.Tools("VCPostBuildEventTool");
			var L_CopyingRuntimeLicense1_Text = "正在复制运行库许可证";
			PostBuildTool.Description = L_CopyingRuntimeLicense1_Text;
			PostBuildTool.CommandLine = 'copy "' + strProjectName + '.lic" "$(OutDir)"';
		}
	} //for
	} //try
	catch(e)
	{
		throw e;
	}
}
