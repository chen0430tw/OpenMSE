// Copyright (c) Microsoft Corporation. All rights reserved.

var ProjWiz;
ProjWiz = new ActiveXObject("ProjWiz.SDProjWiz2.2");

function OnFinish(selProj, selObj)
{
	try
	{
		var strTemplatePath	= wizard.FindSymbol("TEMPLATES_PATH");
		var strProjectPath	= wizard.FindSymbol("PROJECT_PATH");
		var strProjectName	= wizard.FindSymbol("PROJECT_NAME");
		var bAttributed = wizard.FindSymbol("ATTRIBUTED");
				
		wizard.AddSymbol("SAFE_PROJECT_APPID_NAME", wizard.FindSymbol("SAFE_PROJECT_IDENTIFIER_NAME"));
		wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));
		wizard.AddSymbol("RC2_FILE_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));

		EnsureDevicePlatforms();
		
		// these calls must occur before project creation, because they affect how the templates are generated.
		SetDCOMDeviceProjectSymbols();
		SetDeviceSymbolsForPlatforms();

		selProj = CreateDeviceProject(strProjectName, strProjectPath);
	
		AddConfigurations(selProj, strProjectName);

		SetupFilters(selProj);
		
		//[ISSUE] what is the keyword for? can I change it to something like "AtlSDProj"?
		selProj.Object.keyword = "AtlProj";		
		
		AddFilesToProjectWithInfFile(selProj, strProjectName);

		var L_strGenerated_Text = "生成的文件";
		var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");
		if (!bAttributed)
		{
			var strMIDLHeader = strProjectPath + "\\" + strIdlName + ".h";
			wizard.RenderTemplate(strTemplatePath + "\\root.h", strMIDLHeader, true);
			var strMIDL_IFile = strProjectPath + "\\" + strIdlName + "_i.c";

			wizard.RenderTemplate(strTemplatePath + "\\root_i.c", strMIDL_IFile);

			var oGeneratedFiles = selProj.Object.AddFilter(L_strGenerated_Text);
			if (oGeneratedFiles)
			{
				oGeneratedFiles.SourceControlFiles = false;
				oGeneratedFiles.AddFile(strMIDLHeader);
				oGeneratedFiles.AddFile(strMIDL_IFile);
			}
			else
			{
				selProj.Object.AddFile(strMIDLHeader);
				selProj.Object.AddFile(strMIDL_IFile);
			}
		}

		// [ISSUE] the Pch settings are platform independent.
		SetPchSettings(selProj, strProjectName);

		SetDeviceResourceConfigurations(selProj.Object);

		selProj.Object.Save();

		var bMergeProxy = wizard.FindSymbol("MERGE_PROXY_STUB");
		var bSupportProxy = wizard.FindSymbol("SUPPORT_PROXY_STUB");
		if (bSupportProxy && !bMergeProxy)
		{			
			var strDefFile = strProjectPath + "\\" + strProjectName + "ps.def";		
			var str_PFile;
			var str_IFile;

			if (bAttributed)
			{
				str_PFile = strProjectPath + "\\_" + strIdlName + "_p.c";
				str_IFile = strProjectPath + "\\_" + strIdlName + "_i.c";
			}
			else
			{
				str_PFile = strProjectPath + "\\" + strIdlName + "_p.c";
				str_IFile = strProjectPath + "\\" + strIdlName + "_i.c";
			}

			strProjectName += "PS";
			wizard.AddSymbol("CLOSE_SOLUTION", false);
			
			var oPSProj = CreateDeviceProject(strProjectName, strProjectPath);

			SetPSConfigurations(oPSProj, selProj);

			var strSrcFilter = wizard.FindSymbol("SOURCE_FILTER");
			var L_Source_Text = "源文件";
			var group = oPSProj.Object.AddFilter(L_Source_Text);
			group.Filter = strSrcFilter;

			oPSProj.Object.keyword = "AtlPSProj";

			wizard.RenderTemplate(strTemplatePath + "\\rootps.def", strDefFile);
			oPSProj.Object.AddFile(strDefFile);
			var oGeneratedFiles = oPSProj.Object.AddFilter(L_strGenerated_Text);
			if (oGeneratedFiles)
			{
				oGeneratedFiles.SourceControlFiles = false;
				oGeneratedFiles.AddFile(str_IFile);
				oGeneratedFiles.AddFile(str_PFile);
				oGeneratedFiles.AddFile(strProjectPath + "\\dlldata.c");
			}
			else
			{
				oPSProj.Object.AddFile(str_IFile);
				oPSProj.Object.AddFile(str_PFile);
				oPSProj.Object.AddFile(strProjectPath + "\\dlldata.c");
			}

			oPSProj.Object.Save();
		}

		// expand main project node, highlight it
		//
		strProjectName = wizard.FindSymbol("PROJECT_NAME");
		var oHier = wizard.dte.Windows.Item(vsWindowKindSolutionExplorer).Object;
		var oHISolution = oHier.UIHierarchyItems(1);
		var oHIProjMain;
		for (nHI=1; nHI<=oHISolution.UIHierarchyItems.Count; nHI++)
		{
			if ( oHISolution.UIHierarchyItems(nHI).name == strProjectName )
			{
				oHIProjMain = oHISolution.UIHierarchyItems(nHI);
				break;
			}
		}
		if (oHIProjMain)
		{
			oHIProjMain.UIHierarchyItems.Expanded = true;
			oHIProjMain.Select(vsUISelectionTypeSelect);
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
	return false;
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		var strRC2FileName = wizard.FindSymbol("RC2_FILE_NAME");
		var strTarget = strName;

		if (strName == "readme.txt")
			strTarget = "ReadMe.txt";
		if (strName == "resource.h")
			strTarget = "Resource.h";

		if (strName.substr(0, 4) == "root")
		{
			var strRC2FileName = wizard.FindSymbol("RC2_FILE_NAME");
			if (strName == "root.idl")
			{
				var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");
				strTarget = strIdlName + ".idl";
			}
			else if(strName == "root.rc2")
			{
				strTarget = strRC2FileName + strName.substr(4);
			}
			else
			{
				strTarget = strProjectName + strName.substr(4);
			}
		}
		return strTarget;
	}
	catch(e)
	{
		throw e;
	}
}

function SetPSConfigurations(oProj, oMainProj)
{
	try
	{
		var oConfigs = oProj.Object.Configurations;
		var bSupportComPlus = wizard.FindSymbol("SUPPORT_COMPLUS");

		for (var nCntr = 1; nCntr <= oConfigs.Count; nCntr++)
		{
			var config = oConfigs(nCntr);
			var bDebug = false;
			if (-1 != config.Name.indexOf("Debug"))
				bDebug = true;

			if (config.Platform.Name != "Win32")
			{
				config.DebugSettings.DebuggerFlavor = eDebuggerTypeLast;
			}
			
			config.ConfigurationType = typeDynamicLibrary;

			var sIndex = config.Name.indexOf("(");
			var eIndex = config.Name.indexOf(")");
			var oCLTool;
			var strDefines;
			oCLTool = config.Tools("VCCLCompilerTool");
			
			var instructionSet;
			if ((sIndex != -1) && (eIndex != -1))
			{
				instructionSet = config.Name.substr(sIndex + 1, eIndex - sIndex - 1);
				strDefines = "_WIN32_WCE=$(CEVER);UNDER_CE=$(CEVER);WINCE;REGISTER_PROXY_DLL";
				// PLATFORMDEFINES isn't defined for all platforms
				var PlatformDefines = config.Platform.GetMacroValue("PLATFORMDEFINES");
				if (PlatformDefines != "")
				{
					strDefines += ";$(PLATFORMDEFINES)";
				}
			}
			else
			{		
				strDefines = GetPlatformDefine(config);
				strDefines += "_WIN32_WINNT=0x0400;REGISTER_PROXY_DLL";
			}

			if (bDebug)
			{
				strDefines += ";_DEBUG";
			}
			else
			{
				strDefines += ";NDEBUG";
				oCLTool.Optimization = optimizeMaxSpeed;

			}
			config.IntermediateDirectory = "$(PlatformName)\\$(ConfigurationName)PS";
			config.OutputDirectory = "$(PlatformName)\\$(ConfigurationName)PS";
			oCLTool.PreprocessorDefinitions = strDefines;

			var oLinkTool = config.Tools("VCLinkerTool");
			if (bDebug)		
				oLinkTool.AdditionalDependencies = "kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib atlsd.lib atl.lib atlosapis.lib uuid.lib libcmtd.lib";
			else
				oLinkTool.AdditionalDependencies = "kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib atls.lib atl.lib atlosapis.lib uuid.lib libcmt.lib";


			if (bSupportComPlus)
				oLinkTool.AdditionalDependencies += " ole32.lib advapi32.lib comsvcs.lib";

			oLinkTool.ModuleDefinitionFile = oProj.Name + ".def";

			if (!bDebug)
			{
				oLinkTool.EnableCOMDATFolding = optFolding;
				oLinkTool.OptimizeReferences = optReferences;
			}

			oLinkTool.RegisterOutput = false;
			
			
			if (config.Platform.Name == "Pocket PC 2003 (ARMV4)" ||
				config.Platform.Name == "SmartPhone 2003 (ARMV4)" ||
				config.Platform.Name == "Smartphone 2003 (ARMV4)" )
			{
				oLinkTool.AdditionalOptions = oLinkTool.AdditionalOptions + " /ARMPADCODE";
			}
			
			var oPreBuildTool = config.Tools("VCPreBuildEventTool");
			var strCommand = "if exist dlldata.c goto :END\r\n";
			var L_Echo1_Text = "Echo 错误: MIDL 将不会生成 DLLDATA.C，除非在主项目中至少有 1 个接口。\r\n";
			strCommand += L_Echo1_Text;
			strCommand += "Exit 1\r\n";
			strCommand += ":END\r\n";
			oPreBuildTool.CommandLine = strCommand;
			var L_Echo2_Text = "检查是否有所需的文件";
			oPreBuildTool.Description = L_Echo2_Text;
		}

		// exclude from Solution build
		var oSolBuild = dte.Solution.SolutionBuild;
		var oSolConfigs = oSolBuild.SolutionConfigurations;
		for (var nCntr = 1; nCntr <= oSolConfigs.Count; nCntr++)
		{
			var oSolContexts = oSolConfigs(nCntr).SolutionContexts;
			for (var nCntr2 = 1; nCntr2 <= oSolContexts.Count; nCntr2++)
			{
				var oSolContext = oSolContexts(nCntr2);
				if (oSolContext.ProjectName == oProj.UniqueName)
					oSolContext.ShouldBuild = false;
			}
		}

		// add main project to build dependency list
		oSolBuild.BuildDependencies(oProj.UniqueName).AddProject(oMainProj.UniqueName);
	}
	catch(e)
	{
		throw e;
	}
}

var nNumConfigs = 2;

var astrConfigName = new Array();
astrConfigName[0] = "Debug";
astrConfigName[1] = "Release";

function AddConfigurations(proj, strProjectName)
{
	try
	{
		var bAppTypeDLL = wizard.FindSymbol("DLL_APP");
		var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");

		var oConfigs = proj.Object.Configurations;
		for (nCntr = 1 ; nCntr <= oConfigs.Count ; nCntr++)
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

			// set configuration type
			if (bAppTypeDLL)
				config.ConfigurationType = typeDynamicLibrary;

			config.CharacterSet = charSetUnicode;
			config.ATLMinimizesCRunTimeLibraryUsage = false;
			config.UseOfATL = useATLStatic;

			// Compiler settings
			var CLTool;
			var strDefines = strCommonDefinesForAllTools;

			var instructionSet = GetInstructionSet(config.Platform.Name);
			var strBasePlatform = GetBasePlatformName(config.Platform.Name);

			CLTool = config.Tools("VCCLCompilerTool");
			if (instructionSet != "")
			{

				strDefines += ";WINCE";
			}

			CLTool.UsePrecompiledHeader = pchUseUsingSpecific;
			CLTool.WarningLevel = warningLevel_3;
			CLTool.DebugInformationFormat = debugEnabled;
			if (ProjWiz.GetBaseNativePlatformSupportsDCOM(strBasePlatform))
			{
				strDefines += ";_CE_DCOM";
			}


			if (bDebug)
			{
				CLTool.MinimalRebuild = true;
				CLTool.RuntimeLibrary = rtMultiThreadedDebug;
				CLTool.Optimization = optimizeDisabled;
			}
			else
			{
				CLTool.RuntimeLibrary = rtMultiThreaded;
				CLTool.Optimization = optimizeMaxSpeed;
			}
				
			var bAttributed = wizard.FindSymbol("ATTRIBUTED");
			var bMFC		= wizard.FindSymbol("SUPPORT_MFC");
			var bMergeProxy = wizard.FindSymbol("MERGE_PROXY_STUB");
			var bSupportComPlus	= wizard.FindSymbol("SUPPORT_COMPLUS");
			var bSupportComponentRegistrar = wizard.FindSymbol("SUPPORT_COMPONENT_REGISTRAR");

                        // remaining like _DEBUG or NDEBUG are part of strCommonDefinesForAllTools
			if (bDebug)
				strDefines += ";DEBUG"; 
			
					
			if (bAppTypeDLL)
				strDefines += ";_USRDLL";
			if (bAttributed)
				strDefines += ";_ATL_ATTRIBUTES";
		
			if (bMFC)
			{
			    config.UseOfMFC = useMfcStatic;
			}
			if (bMergeProxy && bSupportComponentRegistrar)
				strDefines += ";_MERGE_PROXYSTUB";

			strDefines += ";$(ARCHFAM);$(_ARCHFAM_);_UNICODE;UNICODE"

			CLTool.PreprocessorDefinitions = strDefines;

			// MIDL settings
			var MidlTool = config.Tools("VCMidlTool");
			MidlTool.MkTypLibCompatible = false;

			// [ISSUE] what should we set at here of all CE platforms?
			if (IsPlatformWin32(config))
				MidlTool.TargetEnvironment = midlTargetWin32;

			MidlTool.PreprocessorDefinitions = strCommonDefinesForAllTools;

			
			MidlTool.HeaderFileName = strIdlName + ".h";
			MidlTool.InterfaceIdentifierFileName = strIdlName + "_i.c";
			MidlTool.ProxyFileName = strIdlName + "_p.c";
			MidlTool.GenerateStublessProxies = true;
			MidlTool.TypeLibraryName = "$(IntDir)/" + strIdlName + ".tlb";
			MidlTool.DLLDataFileName = "";

			// Resource settings
			var RCTool = config.Tools("VCResourceCompilerTool");
			RCTool.Culture = wizard.FindSymbol("LCID");
			RCTool.AdditionalIncludeDirectories = "$(IntDir)";
			RCTool.PreprocessorDefinitions = strCommonDefinesForAllTools;

			// Linker settings
			// [ISSUE] name of the CE VCLinkerTool
			var LinkTool = config.Tools("VCLinkerTool");
			// [ISSUE] need to change to subSystemWindowsCE.
			LinkTool.SubSystem = subSystemNotSet;

			LinkTool.IgnoreImportLibrary = true;
			// [ISSUE] "MachineAM33 (/MACHINE:AM33)"
			//var linkType = ProjWiz.GetLinkTypeFromInstructionSet(instructionSet);
			//LinkTool.TargetMachine = "Machine" + linkType + " (MACHINE:" + linkType + ")";
			//LinkTool.TargetMachine = machineARM;

			if (bAppTypeDLL && !bAttributed)
			{
				var strDefFile = ".\\" + strProjectName + ".def";
				LinkTool.ModuleDefinitionFile = strDefFile;
			}

			if (bAppTypeDLL)
			{
				LinkTool.OutputFile = "$(OutDir)/" + strProjectName + ".dll";
			}
			else
			{
				LinkTool.OutputFile = "$(OutDir)/" + strProjectName + ".exe";
			}

			LinkTool.ImportLibrary = "$(OutDir)/" + strProjectName + ".lib";
			if (bAttributed)
				LinkTool.MergedIDLBaseFileName = "_" + strIdlName + ".idl";
			
			LinkTool.GenerateDebugInformation = true;
			if (bDebug)
			{
				LinkTool.LinkIncremental = linkIncrementalYes;
			}
			else
			{
				LinkTool.LinkIncremental = linkIncrementalNo;
				LinkTool.EnableCOMDATFolding = optFolding;
				LinkTool.OptimizeReferences = optReferences;
			}
			
			if (config.Platform.Name != "Win32")
			{
				SetDeviceLinkLibSettings(config);
			}

			LinkTool.StackReserveSize="65536";
			LinkTool.StackCommitSize="4096";
			
			if (config.Platform.Name == "Pocket PC 2003 (ARMV4)" ||
				config.Platform.Name == "SmartPhone 2003 (ARMV4)" ||
				config.Platform.Name == "Smartphone 2003 (ARMV4)" )
			{
				LinkTool.AdditionalOptions = LinkTool.AdditionalOptions + " /ARMPADCODE";
			}

			LinkTool.RegisterOutput = false;

			// do not delete the access of the deployment tool here, it initializes the device toolbar
			var DeployTool = config.DeploymentTool;
			if (DeployTool)
			{
				if (bAppTypeDLL)
				{
					DeployTool.RegisterOutput = true;
				}
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}

function SetPchSettings(proj, strProjectName)
{
	try
	{
		var files = proj.Object.Files;
		var fStdafx = files("StdAfx.cpp");
		var bAttributed = wizard.FindSymbol("ATTRIBUTED");

		var nCntr;
		var oConfigs = proj.Object.Configurations;
		for (nCntr = 1; nCntr <= oConfigs.Count; nCntr++)
		{
		    var currentconfig = oConfigs(nCntr);
			var config = fStdafx.FileConfigurations(currentconfig.Name);
			config.Tool.UsePrecompiledHeader = pchCreateUsingSpecific;

			if (!bAttributed)
			{
				var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");
				var fProject_i = files(strIdlName + "_i.c");
				config = fProject_i.FileConfigurations(currentconfig.Name);
				config.Tool.UsePrecompiledHeader = pchNone;

				if (wizard.FindSymbol("MERGE_PROXY_STUB"))
				{
					file = files("dlldatax.c");
					config = file.FileConfigurations(currentconfig.Name);
					config.Tool.UsePrecompiledHeader = pchNone;
				}
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}
