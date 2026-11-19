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

		if (wizard.FindSymbol("APP_TYPE_SDI_DOCLIST"))
		{
			wizard.AddSymbol("DOCVIEW", true);
		}

		selProj = CreateDeviceProject(strProjectName, strProjectPath);

		AddConfigurations(selProj);

		SetupFilters(selProj);

		var strDocTypeName = wizard.FindSymbol("DOC_TYPE_NAME"); 		
		if (strDocTypeName != "" )
		{
			wizard.AddSymbol("SAFE_DOC_TYPE_NAME", CreateSafeName(strDocTypeName));
		}
		
		selProj.Object.keyword = "MFCProj";

		if (IsDBCSCharSet(wizard))
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "9"); //VS-supported codedepages that require FontSize 9
		else
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "8"); //all the rest and unsupported codepages: Fontsize 8

		AddFilesToProjectWithInfFile(selProj, strProjectName);
		SetCommonPchSettings(selProj);

		SetDeviceResourceConfigurations(selProj.Object);
		selProj.Object.Save();

		if (wizard.FindSymbol("APP_TYPE_DLG"))
		{
			var configs = selProj.Object.Configurations;
			var strDialogId = "IDD_" + wizard.FindSymbol("UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME") + "_DIALOG";
			var oResHelper = wizard.ResourceHelper;

			var symbol = ProjWiz.GetBaseNativePlatformProperty(configs.Item(1).Platform.Name, "UISymbol");
			if (symbol == "POCKETPC2003_UI_MODEL")
				oResHelper.OpenResourceFile(strProjectPath + "\\" + strProjectName + "ppc.rc");
			else if (symbol == "SMARTPHONE2003_UI_MODEL")
				oResHelper.OpenResourceFile(strProjectPath + "\\" + strProjectName + "sp.rc");
			else
				oResHelper.OpenResourceFile(strProjectPath + "\\" + strProjectName + ".rc");
			oResHelper.OpenResourceInEditor("DIALOG", strDialogId);
			oResHelper.CloseResourceFile();
		}

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

function AddConfigurations(proj)
{
	try
	{
		var bDynamicMFC = wizard.FindSymbol("DYNAMIC_MFC");

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

			config.CharacterSet = charSetUnicode;

			config.IntermediateDirectory = "$(PlatformName)\\$(ConfigurationName)";
			config.OutputDirectory = "$(PlatformName)\\$(ConfigurationName)";

			var CLTool;
			var strDefines = strCommonDefinesForAllTools;		
			var instructionSet = GetInstructionSet(config.Platform.Name);
			var strBasePlatform = GetBasePlatformName(config.Platform.Name);

			CLTool = config.Tools("VCCLCompilerTool");
			
			if (bDynamicMFC)
			{
				config.UseOfMFC = useMfcDynamic;	
				CLTool.RuntimeLibrary = (bDebug ? rtMultiThreadedDebugDLL : rtMultiThreadedDLL);
			}
			else
			{
				config.UseOfMFC = useMfcStatic;
				CLTool.RuntimeLibrary = (bDebug ? rtMultiThreadedDebug : rtMultiThreaded);
			}

			if (instructionSet != "")
			{
				strDefines += GetPlatformDefine(config);
				strDefines += ";WINCE";
			}
									
			if (ProjWiz.GetBaseNativePlatformSupportsDCOM(strBasePlatform))
			{
				strDefines += ";_CE_DCOM";
			}
			
			CLTool.DebugInformationFormat = debugEnabled;

			if(bDebug)
			{
				strDefines += ";DEBUG"; //_DEBUG is already part of strCommonDefinesForAllTools 
				CLTool.MinimalRebuild = true;
				if (bDynamicMFC)
				{
					CLTool.RuntimeLibrary = rtMultiThreadedDebugDLL;
				}
				else
				{
					CLTool.RuntimeLibrary = rtMultiThreadedDebug;
				}
				CLTool.Optimization = optimizeDisabled;
			}
			else
			{
				CLTool.MinimalRebuild = false;
				if (bDynamicMFC)
				{
					CLTool.RuntimeLibrary = rtMultiThreadedDLL;
				}
				else
				{
					CLTool.RuntimeLibrary = rtMultiThreaded;
				}
				CLTool.Optimization = optimizeMaxSpeed;
			}

			strDefines += ";$(ARCHFAM);$(_ARCHFAM_)"
			CLTool.PreprocessorDefinitions = strDefines;
			CLTool.WarningLevel = warningLevel_3;

			// Linker settings
			var LinkTool = config.Tools("VCLinkerTool");
			LinkTool.GenerateDebugInformation = true;
			// [ISSUE] the following two properties' settings for non-win32 platforms need to be added into the project system
			//LinkTool.SubSystem = subSystemWindows;
			//LinkTool.TargetMachine = machineX86;

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
				LinkTool.EntryPointSymbol = "WinMainCRTStartup";
			}

			LinkTool.StackReserveSize="65536";
			LinkTool.StackCommitSize="4096";

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
				if (bDynamicMFC)
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
			}

			// [ISSUE]
			var MidlTool = config.Tools("VCMidlTool");
			MidlTool.MkTypLibCompatible = false;
			MidlTool.PreprocessorDefinitions = strCommonDefinesForAllTools;

			var RCTool = config.Tools("VCResourceCompilerTool");
			RCTool.Culture = wizard.FindSymbol("LCID");
			RCTool.PreprocessorDefinitions = strCommonDefinesForAllTools;
			RCTool.AdditionalIncludeDirectories = "$(IntDir)";
		}
	}
	catch(e)
	{
		throw e;
	}
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
		if (strName.substr(0, 7) == "dlgroot")
		{
			var strExtension = strName.substr(7);
			strTarget = strProjectName + strExtension;
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
			case "allayg.rc":
			case "dlgallayg.rc":
				strTarget = strProjectName + "ayg.rc";
				break;
			case "allppc.rc":
			case "dlgallppc.rc":
				strTarget = strProjectName + "ppc.rc";
				break;
			case "allsp.rc":
			case "dlgallsp.rc":
				strTarget = strProjectName + "sp.rc";
				break;
			case "dlgres.h":
			case "resource.h":
				strTarget = "Resource.h";
				break;
			case "dlgresayg.h":
			case "resourceayg.h":
				strTarget = "resourceayg.h";
				break;
			case "dlgresppc.h":
			case "resourceppc.h":
				strTarget = "Resourceppc.h";
				break;
			case "dlgressp.h":
			case "resourcesp.h":
				strTarget = "Resourcesp.h";
				break;
			case "dialog.h":
				strTarget = wizard.FindSymbol("DIALOG_HEADER");
				break;
			case "dialog.cpp":
				strTarget = wizard.FindSymbol("DIALOG_IMPL");
				break;
			case "frame.h":
				strTarget = wizard.FindSymbol("MAIN_FRAME_HEADER");
				break;
			case "frame.cpp":
				strTarget = wizard.FindSymbol("MAIN_FRAME_IMPL");
				break;
			case "doc.h":
				strTarget = wizard.FindSymbol("DOC_HEADER");
				break;
			case "doc.cpp":
				strTarget = wizard.FindSymbol("DOC_IMPL");
				break;
			case "view.h":
				strTarget = wizard.FindSymbol("VIEW_HEADER");
				break;
			case "view.cpp":
				strTarget = wizard.FindSymbol("VIEW_IMPL");
				break;
			case "wndview.h":
				strTarget = wizard.FindSymbol("WND_VIEW_HEADER");
				break;
			case "wndview.cpp":
				strTarget = wizard.FindSymbol("WND_VIEW_IMPL");
				break;
			case "treeview.h":
				strTarget = wizard.FindSymbol("TREE_VIEW_HEADER");
				break;
			case "treeview.cpp":
				strTarget = wizard.FindSymbol("TREE_VIEW_IMPL");
				break;
			case "doc.ico":
				strTarget = strResPath + "\\" + strProjectName + "Doc.ico";
				break;
			default:
				break;
		}

		var strTemp = GetAdditionalPath(strTarget, strResPath);


		strTarget = strTemp;
		return strTarget;
	}
	catch(e)
	{
		throw e;
	}
}

function GetAdditionalPath(strName, strResPath)
{
	try
	{
		var strFullName = strName;
		switch(strName)
		{
			case "tba__.bmp":
			case "tbah_.bmp":
			case "tbnd_.bmp":
			case "tba_rap.bmp":
				strFullName = strResPath + "\\Cmdbar.bmp";
				break;
			default:
				break;
		}
		return strFullName;
	}
	catch(e)
	{
		throw e;
	}
}
