// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
    try
    {
        var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
        var strProjectName = wizard.FindSymbol("PROJECT_NAME");

        if (!CanUseDrive(strProjectPath))
            return VS_E_WIZARDBACKBUTTONPRESS;

        var proj = CreateManagedSQLProject(strProjectName, strProjectPath);
        AddManagedConfigsForDLL(proj, strProjectName);
        AddSpecificConfig(proj, strProjectName);
        AddReferencesForApp(proj);
        AddFilesToNewProjectWithInfFile(proj, strProjectName);

        var oProjObject = proj.Object;
        oProjObject.AddAssemblyReference("System.Data.dll");
        //oProjObject.AddAssemblyReference("SQLClient.dll");
        wizard.DatabaseConnection(proj);

        proj.Object.Save();
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

        if (strName == "readme.txt")
            strTarget = "ReadMe.txt";

        if (strName.substr(0, 4) == "root")
        {
            strTarget = strProjectName + strName.substr(4);
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
            config.ManagedExtensions = managedAssemblySafe;
            var LinkerTool = config.Tools("VCLinkerTool");
			// /NOENTRY
			LinkerTool.ResourceOnlyDll = true;
        } //for
    } //try
    catch(e)
    {
        throw e;
    }
}

function CreateManagedSQLProject(strProjectName, strProjectPath)
{
	try
	{
		strIdentifierSafeName = CreateIdentifierSafeName(strProjectName);
		wizard.AddSymbol("SAFE_PROJECT_IDENTIFIER_NAME", strIdentifierSafeName);
		var strNamespaceName = CreateCPPName(strIdentifierSafeName);

		var strNamespaces = strNamespaceName.split("::");
		var strNamespaceBegin = "";
		var strNamespaceEnd = "";
		for (var i = 0; i < strNamespaces.length; i++)
		{
			strNamespaceBegin += "namespace " + strNamespaces[i] + " {\r\n";
			strNamespaceEnd += "}";
		}

		wizard.AddSymbol("SAFE_NAMESPACE_NAME", strNamespaceName);
		wizard.AddSymbol("SAFE_NAMESPACE_BEGIN", strNamespaceBegin);
		wizard.AddSymbol("SAFE_NAMESPACE_END", strNamespaceEnd);

		AddAssemblyInfoSymbols();

		var oProject = CreateSQLProject(strProjectName, strProjectPath);
		SetupFilters(oProject);
		return oProject;
	}
	catch(e)
	{
		throw e;
	}
}

function CreateSQLProject(strProjectName, strProjectPath)
{
	try
	{
		var strProjTemplatePath = wizard.FindSymbol("PROJECT_TEMPLATE_PATH");
		var strProjTemplate = strProjTemplatePath + "\\defaultsql.vcproj";

		var Solution = dte.Solution;
		var strSolutionName = "";
		if (wizard.FindSymbol("CLOSE_SOLUTION"))
		{
			Solution.Close();
			strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
			if (strSolutionName.length)
			{
				var strSolutionPath = strProjectPath.substr(0, strProjectPath.length - strProjectName.length);
				Solution.Create(strSolutionPath, strSolutionName);
			}
		}

		var strProjectNameWithExt = strProjectName + ".vcproj";
		var oTarget = wizard.FindSymbol("TARGET");
		var oProj;
		if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject)  // vsWizardAddSubProject
		{
			var prjItem = oTarget.AddFromTemplate(strProjTemplate, strProjectPath + "\\" + strProjectNameWithExt);
			oProj = prjItem.SubProject;
		}
		else
		{
			oProj = oTarget.AddFromTemplate(strProjTemplate, strProjectPath, strProjectNameWithExt);
		}

		return oProj;
	}
	catch(e)
	{
		throw e;
	}
}
