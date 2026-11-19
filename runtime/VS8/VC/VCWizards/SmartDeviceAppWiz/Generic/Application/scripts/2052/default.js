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

		EnsureDevicePlatforms();

		// this call must occur before we create the project, because otherwise the templates are already generated.
		SetDeviceSymbolsForPlatforms();

		selProj = CreateDeviceProject(strProjectName, strProjectPath);
		selProj.Object.Keyword = "Win32Proj";
		
		var strAppType = GetAppType();
		var bEmptyProject = wizard.FindSymbol("EMPTY_PROJECT");
		
		AddConfigurations(selProj, strProjectName, bEmptyProject, strAppType);

		SetupFilters(selProj);

		if (!bEmptyProject)
		{
			if (IsDBCSCharSet(wizard))
				wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "9"); //VS-supported codedepages that require FontSize 9
			else
				wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "8"); //all the rest and unsupported codepages: Fontsize 8
			
			var Pch = wizard.FindSymbol("PRE_COMPILED_HEADER");

			if ((strAppType == "LIB" || ((strAppType == "CONSOLE") &&
		                wizard.FindSymbol("SUPPORT_MFC"))) && !Pch)
			{
				AddFilesToProjectWithInfFile(selProj, strProjectName);
				SetNoPchSettings(selProj);
			}
			else
			{
				AddFilesToProjectWithInfFile(selProj, strProjectName);
				SetCommonPchSettings(selProj);
			}
		}
		
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

		if (strName == "readme.txt")
			strTarget = "ReadMe.txt";
		if (strName == "resource.h")
			strTarget = "Resource.h";

		if (strName == "small.ico")
		{
			strTarget = "small.ico";
		}
		if (strName.substr(0, 4) == "root")
		{
			if (strName == "root.rc2" || strName == "rootayg.rc2" || strName == "rootppc.rc2" || strName == "rootsp.rc2")
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

function GetAppType()
{
	try
	{
		var strAppType = "";
		if (wizard.FindSymbol("CONSOLE_APP"))
			strAppType = "CONSOLE";
		else
		{
			if (wizard.FindSymbol("WIN_APP"))
				strAppType = "WIN";
			else
			{
				if (wizard.FindSymbol("DLL_APP"))
					strAppType = "DLL";
				else
				{
					if (wizard.FindSymbol("LIB_APP"))
						strAppType = "LIB";
				}
			}
		}
		return strAppType;
	}
	catch(e)
	{
		throw e;
	}
}

function AddConfigurations(oProj, strProjectName, bEmptyProject, strAppType)
{
	try
	{
		var bMFC = wizard.FindSymbol("SUPPORT_MFC");
		var bATL = wizard.FindSymbol("SUPPORT_ATL");

		var oConfigs = oProj.Object.Configurations;
		for (var nCntr = 1 ; nCntr <= oConfigs.Count ; nCntr++)
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

			if (config.Platform.Name != "Win32")
			{
				config.DebugSettings.DebuggerFlavor = eDebuggerTypeLast;
			}	
			
			// set output directories
			config.IntermediateDirectory = "$(PlatformName)\\$(ConfigurationName)";
			config.OutputDirectory = "$(PlatformName)\\$(ConfigurationName)";
			
			if (strAppType == "LIB")
				config.ConfigurationType = typeStaticLibrary;
			else if (strAppType == "DLL")
				config.ConfigurationType = typeDynamicLibrary;

			config.CharacterSet = charSetUnicode;

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
					
			if (ProjWiz.GetBaseNativePlatformSupportsDCOM(strBasePlatform))
		        {
                		strDefines += ";_CE_DCOM";
		        }
            
			CLTool.WarningLevel = warningLevel_3;
			CLTool.DebugInformationFormat = debugEnabled;
			
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

			if (bDebug)
			{
				if (strAppType != "DLL" && bMFC)
				{
					// REVIEW: RuntimeLibrary option previously present here, left in because we may need to specialize this case for some CE devices
				}
				else
				{
					// REVIEW: RuntimeLibrary option previously present here, left in because we may need to specialize this case for some CE devices
					
				}
			}
			else
			{
				if (strAppType != "DLL" && bMFC)
				{
					// REVIEW: RuntimeLibrary option previously present here, left in because we may need to specialize this case for some CE devices
				}
			}
			if (strAppType == "DLL" && bMFC)
			{
				// set /EHsc in this case
				CLTool.ExceptionHandling = cppExceptionHandlingYes;
			}

			if (bEmptyProject || (strAppType == "LIB"))
				CLTool.UsePrecompiledHeader = pchNone;

			if (bDebug)
				strDefines += ";DEBUG"; // remaining like _DEBUG or NDEBUG are part of strCommonDefinesForAllTools

			switch(strAppType)
			{
				case "CONSOLE":
					strDefines += ";_CONSOLE";
					if (bMFC)
						config.UseOfMFC = useMfcStatic;
					if (bATL)
						config.UseOfATL = useATLStatic;
					break;
				case "LIB":
					strDefines += ";_LIB";
					if (bMFC)
						config.UseOfMFC = useMfcStatic;
					break;
				case "DLL":
					strDefines += ";_WINDOWS;_USRDLL;";
					var strExports = wizard.FindSymbol("UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME") + "_EXPORTS";
					strDefines += strExports;
					if (bMFC)
						config.UseOfMFC = useMfcStatic;
					if (bATL)
						config.UseOfATL = useATLStatic;
					break;
				case "WIN":
					strDefines += ";_WINDOWS";
					if (bMFC)
						config.UseOfMFC = useMfcStatic;
					if (bATL)
						config.UseOfATL = useATLStatic;
					break;
				default:
					break;
			}

			strDefines += ";$(ARCHFAM);$(_ARCHFAM_);_UNICODE;UNICODE"
			CLTool.PreprocessorDefinitions = strDefines;

		
			// Resource settings
			var RCTool = config.Tools("VCResourceCompilerTool");
			RCTool.Culture = wizard.FindSymbol("LCID");
			RCTool.AdditionalIncludeDirectories = "$(IntDir)";
			
			RCTool.PreprocessorDefinitions = strCommonDefinesForAllTools;

			if (strAppType == "LIB")
			{
				var LibTool = config.Tools("VCLibrarianTool");
				LibTool.OutputFile = "$(OutDir)/" + strProjectName + ".lib";
				SetDeviceLinkLibSettings(config);
			}
			else
			{
				var LinkTool = config.Tools("VCLinkerTool");					
				LinkTool.SubSystem = subSystemNotSet;
				LinkTool.ProgramDatabaseFile = "$(OutDir)/" + strProjectName + ".pdb";
				LinkTool.GenerateDebugInformation = true;

				if (bDebug)
					LinkTool.LinkIncremental = linkIncrementalYes;
				else
				{
					LinkTool.LinkIncremental = linkIncrementalNo;
					LinkTool.EnableCOMDATFolding = optFolding;
					LinkTool.OptimizeReferences = optReferences;
				}
				if (strAppType == "DLL")
				{
					LinkTool.ImportLibrary = "$(OutDir)/" + strProjectName + ".lib";
					LinkTool.OutputFile = "$(OutDir)/" + strProjectName + ".dll";
				}
				else
				{
					LinkTool.OutputFile = "$(OutDir)/" + strProjectName + ".exe";
				}

				LinkTool.StackReserveSize="65536";
				LinkTool.StackCommitSize="4096";

				if (strAppType == "CONSOLE")
				{
					LinkTool.EntryPointSymbol = "mainWCRTStartup";
				}

				if (config.Platform.Name != "Win32")
				{
					SetDeviceLinkLibSettings(config);
				}
				
				if (config.Platform.Name == "Pocket PC 2003 (ARMV4)" ||
					config.Platform.Name == "SmartPhone 2003 (ARMV4)" ||
					config.Platform.Name == "Smartphone 2003 (ARMV4)" )
				{
					LinkTool.AdditionalOptions += " /ARMPADCODE";
				}
				
				var symbol = ProjWiz.GetBaseNativePlatformProperty(config.Platform.Name, "UISymbol");	

				LinkTool.DelayLoadDLLs = "$(NOINHERIT)";
				
				// do not delete the access of the deployment tool here, it initializes the device toolbar
				var DeployTool = config.DeploymentTool;
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}
