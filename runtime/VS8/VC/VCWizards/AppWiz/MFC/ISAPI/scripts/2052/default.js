// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
    try
    {
        var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
        var strProjectName = wizard.FindSymbol("PROJECT_NAME");

        selProj = CreateProject(strProjectName, strProjectPath);

        AddCommonConfig(selProj, strProjectName, false); // ISAPI does not support UNICODE
        AddSpecificConfig(selProj, strProjectName);

        selProj.Object.keyword = "MFCISAPIProj";

        SetupFilters(selProj);

        AddFilesToProjectWithInfFile(selProj, strProjectName);
        SetCommonPchSettings(selProj);

		selProj.Object.Save();

        if (wizard.FindSymbol("GENERATE_DEPLOYMENT"))
        {
            var strDplyPath = Solution.TemplatePath(GUID_Deployment_TemplatePath);
            strDplyPath += "\\WebSetup.vdproj";
            
            var strDeployDir = wizard.FindSymbol("PROJECT_PATH");
            var strDeployName = wizard.FindSymbol("PROJECT_NAME")+"Deploy";
            var strFullDeployPath = "";

            var nIndex = strDeployDir.lastIndexOf(wizard.FindSymbol("PROJECT_NAME"));
            if (nIndex == -1)
                nIndex = strDeployDir.length;
            else
                nIndex -= 1;

            var strDeployPath = strDeployDir.substring(0, nIndex);
            strDeployPath+= "\\" + strDeployName;
            var fso = new ActiveXObject("Scripting.FileSystemObject");
            
            try
            {
                fso.CreateFolder(strDeployPath);
            }
            catch (e)
            {
                strDeployPath = strDeployDir.substring(0, nIndex);
                strDeployPath+= "\\" + wizard.GenerateNextDefaultProjectName(strDeployName, strDeployPath);
            }
                
            var oTarget = wizard.FindSymbol("TARGET");
            var dplyproj;

            if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject)  //  
            {
                var prjItem = oTarget.AddFromTemplate(strDplyPath, strDeployPath + "\\" + strDeployName);
                dplyproj = prjItem.SubProject;
            }
            else
            {
                dplyproj = oTarget.AddFromTemplate(strDplyPath, strDeployPath, strDeployName);
            }
    
            var webfldr = wizard.GetDefaultWebTargetFolder(dplyproj.Object);
            if (webfldr == null)
            {
                webfldr = wizard.CreateWebTargetFolder();
                webfldr.Name = "MC++ Server Deployment";
                
                var webfldrkey = "MC_DEPLOY";
                var FolderPlugin = dplyproj.Object.PlugIns.Item("Folder");
    
                FolderPlugin.Items.AddWithKey(webfldrkey, webfldr);
            }
            
            var strAddress = "http://";
            strAddress += wizard.FindSymbol("ISAPI_SERVER");
            strAddress += "/"
            strAddress += wizard.FindSymbol("ISAPI_VROOT");
            webfldr.DefaultAddress = strAddress;
            
            var strRemoteMachine = wizard.FindSymbol("ISAPI_SERVER");
            wizard.SetRemoteMachine(dplyproj, strRemoteMachine);
    
            webfldr.IsApplication = true;   
            webfldr.AlwaysCreate = true;

            var builtOutput = wizard.AddBuiltOutput(dplyproj.Object, selProj);
            builtOutput.Folder = webfldr;
            var deployContent = wizard.AddDeploymentContent(dplyproj.Object, selProj);
            deployContent.Folder = webfldr;
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
		var strTarget = strName;
		if (strName.substr(0, 4) == "root")
		{
			var strlen = strName.length;
   			strTarget = strProjectName + strName.substr(4);
			return strTarget;
		}

		switch (strName)
		{
			case "readme.txt":
				strTarget = "ReadMe.txt";
				break;
			case "all.rc":
				strTarget = strProjectName + ".rc";
				break;
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
		var bDynamicMFC = wizard.FindSymbol("DYNAMIC_MFC");

		var config = proj.Object.Configurations("Debug");
		config.CharacterSet = charSetMBCS;
		config.ConfigurationType = typeDynamicLibrary;

		var CLTool = config.Tools("VCCLCompilerTool");
		var strDefines = CLTool.PreprocessorDefinitions;
		if (strDefines != "") strDefines += ";";
		strDefines += GetPlatformDefine(config);
		if (bDynamicMFC)
		{
			config.UseOfMFC = useMfcDynamic;
			CLTool.RuntimeLibrary = rtMultiThreadedDebugDLL;
		}
		else
		{
			config.UseOfMFC = useMfcStatic;
			CLTool.RuntimeLibrary = rtMultiThreadedDebug;
		}
		strDefines += "_WINDOWS;_DEBUG;_USRDLL";
		CLTool.PreprocessorDefinitions = strDefines;
		CLTool.TreatWChar_tAsBuiltInType = true;
		CLTool.DebugInformationFormat = debugEditAndContinue;

		var MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = "_DEBUG";

		var RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = wizard.FindSymbol("LCID");
		RCTool.PreprocessorDefinitions = "_DEBUG";
    
		var LinkTool = config.Tools("VCLinkerTool");
		LinkTool.LinkIncremental = linkIncrementalYes;
		LinkTool.GenerateDebugInformation = true;
		var strDefFile = ".\\" + strProjectName + ".def";
		LinkTool.ModuleDefinitionFile = strDefFile;
		LinkTool.AdditionalDependencies = "strsafe.lib";

		config = proj.Object.Configurations("Release");
		config.CharacterSet = charSetMBCS;
		config.ConfigurationType = typeDynamicLibrary;

		CLTool = config.Tools("VCCLCompilerTool");
		strDefines = GetPlatformDefine(config);
		if (bDynamicMFC)
		{
			config.UseOfMFC = useMfcDynamic;
			strDefines += "_WINDOWS;NDEBUG;_USRDLL";
			CLTool.PreprocessorDefinitions = strDefines;
			CLTool.RuntimeLibrary = rtMultiThreadedDLL;
		}
		else
		{
			config.UseOfMFC = useMfcStatic;
			strDefines += "_WINDOWS;NDEBUG;_USRDLL";
			CLTool.PreprocessorDefinitions = strDefines;
			CLTool.RuntimeLibrary = rtMultiThreaded;
		}
		CLTool.TreatWChar_tAsBuiltInType = true;
		CLTool.DebugInformationFormat = debugEnabled;

		MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = "NDEBUG";

		RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = wizard.FindSymbol("LCID");
		RCTool.PreprocessorDefinitions = "NDEBUG";

		LinkTool = config.Tools("VCLinkerTool");
		var strDefFile = ".\\" + strProjectName + ".def";
		LinkTool.ModuleDefinitionFile = strDefFile;
		LinkTool.GenerateDebugInformation = true;
		LinkTool.LinkIncremental = linkIncrementalNo;
	}
	catch(e)
	{
		throw e;
	}
}

