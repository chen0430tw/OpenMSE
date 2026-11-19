// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");
		wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));

		selProj = CreateProject(strProjectName, strProjectPath);

		AddCommonConfig(selProj, strProjectName);
		AddSpecificConfig(selProj, strProjectName);
		selProj.Object.keyword = "MFCActiveXProj";

		SetupFilters(selProj);

		if (IsDBCSCharSet(wizard))
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "9"); //VS-supported codedepages that require FontSize 9
		else
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "8"); //all the rest and unsupported codepages: Fontsize 8 

		AddFilesToProjectWithInfFile(selProj, strProjectName);
		SetCommonPchSettings(selProj);

		selProj.Object.Save();
		
		AddHelpBuildSteps(selProj.Object, strProjectName);
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
			if (strName == "root.idl")
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
			case "ctlcore.rtf":
				strTarget = strHelpPath + "\\" + strProjectName + ".rtf";
				break;
			case "bullet.bmp":
				strTarget = strHelpPath + "\\" + "Bullet.bmp";
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
	var oConfigs = proj.Object.Configurations;
	for (var nCntr = 1; nCntr <= oConfigs.Count; nCntr++)
	{
		var config = oConfigs(nCntr);
		var bDebug = false;
		if (-1 != config.Name.indexOf("Debug"))
			bDebug = true;
	
		config.ConfigurationType = typeDynamicLibrary;
		config.UseOfMFC = useMfcDynamic;
		config.CharacterSet = charSetUNICODE;

		var CLTool = config.Tools("VCCLCompilerTool");
		var strDefines = CLTool.PreprocessorDefinitions;
		if (strDefines != "") strDefines += ";";
		strDefines += GetPlatformDefine(config);
		if(bDebug)
		{
			strDefines += "_WINDOWS;_DEBUG;_USRDLL";
			CLTool.DebugInformationFormat = debugEditAndContinue;
		}			
		else
		{
			strDefines += "_WINDOWS;NDEBUG;_USRDLL";	
			CLTool.MinimalRebuild = false;
			CLTool.DebugInformationFormat = debugEnabled;
		}
		CLTool.PreprocessorDefinitions= strDefines;
		CLTool.RuntimeLibrary = (bDebug ? rtMultiThreadedDebugDLL : rtMultiThreadedDLL);

		var MidlTool = config.Tools("VCMidlTool");
		var strIdlName = wizard.FindSymbol("SAFE_IDL_NAME");
		MidlTool.MkTypLibCompatible = false;
		//no_robust
		MidlTool.ValidateParameters = false; 
		MidlTool.PreprocessorDefinitions = (bDebug ? "_DEBUG" : "NDEBUG");
		MidlTool.TypeLibraryName = "$(IntDir)/" + strIdlName + ".tlb";
		MidlTool.HeaderFileName = "$(ProjectName)idl.h";

		var RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = wizard.FindSymbol("LCID");
		RCTool.PreprocessorDefinitions = (bDebug ? "_DEBUG" : "NDEBUG");
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";
		
		var LinkTool = config.Tools("VCLinkerTool");
		LinkTool.LinkIncremental = (bDebug ? linkIncrementalYes : linkIncrementalNo);
		
		LinkTool.GenerateDebugInformation = true;
		var strDefFile = ".\\" + strProjectName + ".def";
		LinkTool.ModuleDefinitionFile = strDefFile;
		LinkTool.OutputFile = "$(OutDir)\\$(ProjectName).ocx";

		LinkTool.RegisterOutput = true;

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

function AddHelpBuildSteps(projObj, strProjectName)
{
	try
	{
		var bHelpFiles = wizard.FindSymbol("HELP_FILES");
		
		if (!bHelpFiles)
			return;
			
		var fileExt;
		var fileTool1 = "";
		var fileTool2 = "";
		var outFileExt;
		
		fileExt = ".hpj";
		fileTool = "makehelp.bat";

		fileTool1 = "start /wait hcw /C /E /M \"$(ProjectName).hpj\"";

		var strCodeTool = new Array();
		strCodeTool[0] = "echo // ";
		var L_CodeFragment1_Text = "生成的帮助映射文件。由 $(ProjectName).HPJ 使用。";
		strCodeTool[0] += L_CodeFragment1_Text + " > \"hlp\\$(ProjectName).hm\"";
		strCodeTool[1] = "echo. >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[2] = "echo // ";
		var L_CodeFragment2_Text = "命令(ID_* 和 IDM_*)";
		strCodeTool[2] += L_CodeFragment2_Text + " >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[3] = "makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 \"$(InputFileName)\" >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[4] = "echo. >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[5] = "echo // ";
		var L_CodeFragment3_Text = "提示(IDP_*)";
		strCodeTool[5] += L_CodeFragment3_Text + " >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[6] = "makehm IDP_,HIDP_,0x30000 \"$(InputFileName)\" >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[7] = "echo. >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[8] = "echo // ";
		var L_CodeFragment4_Text = "资源(IDR_*)";
		strCodeTool[8] += L_CodeFragment4_Text + " >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[9] = "makehm IDR_,HIDR_,0x20000 \"$(InputFileName)\" >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[10] = "echo. >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[11] = "echo // ";
		var L_CodeFragment5_Text = "对话框(IDD_*)";
		strCodeTool[11] += L_CodeFragment5_Text + " >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[12] = "makehm IDD_,HIDD_,0x20000 \"$(InputFileName)\" >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[13] = "echo. >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[14] = "echo // ";
		var L_CodeFragment6_Text = "框架控件(IDW_*)";
		strCodeTool[14] += L_CodeFragment6_Text + " >> \"hlp\\$(ProjectName).hm\"";
		strCodeTool[15] = "makehm IDW_,HIDW_,0x50000 \"$(InputFileName)\" >> \"hlp\\$(ProjectName).hm\"";
		for (var idx=0; idx<16; idx++)
			fileTool2 += strCodeTool[idx] + "\r\n";

		outFileExt = ".hlp";
		
		var fileObj1 = projObj.Files(strProjectName + fileExt);
		var fileObj2 = projObj.Files("resource.h");
		if (fileObj1 != null)
		{
			for (var i=1; i<=fileObj1.FileConfigurations.Count; i++)
			{
				var config = fileObj1.FileConfigurations.Item(i);
				if (config != null)
				{
					var CustomBuildTool = config.Tool;
					CustomBuildTool.AdditionalDependencies = ".\\hlp\\$(ProjectName).hm";
					CustomBuildTool.Outputs = ".\\$(ProjectName)" + outFileExt;
					CustomBuildTool.CommandLine = fileTool1;
					L_ToolDesc1_Text = "正在生成帮助文件...";
					CustomBuildTool.Description = L_ToolDesc1_Text;
				}
			}
		}
		if (fileObj2 != null)
		{
			for (var i=1; i<=fileObj2.FileConfigurations.Count; i++)
			{
				var config = fileObj2.FileConfigurations.Item(i);
				if (config != null)
				{
					var CustomBuildTool = config.Tool;
					CustomBuildTool.Outputs = ".\\hlp\\$(ProjectName).hm";
					CustomBuildTool.CommandLine = fileTool2;
					L_ToolDesc2_Text = "正在生成帮助编译器的映射文件...";
					CustomBuildTool.Description = L_ToolDesc2_Text;
				}
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}
