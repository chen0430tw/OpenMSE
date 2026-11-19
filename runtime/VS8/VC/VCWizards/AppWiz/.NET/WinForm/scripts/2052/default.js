// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");
		
		if (strProjectName == "Form1")
			wizard.AddSymbol("CLASS_NAME_PREFIX", "::Form1::");
		else
			wizard.AddSymbol("CLASS_NAME_PREFIX", "");

		if (!CanUseDrive(strProjectPath))
			return VS_E_WIZARDBACKBUTTONPRESS;

		var proj = CreateManagedProject(strProjectName, strProjectPath);
		AddManagedConfigsForEXE(proj, strProjectName, subSystemWindows, false);
		proj.Object.Configurations("Debug").ManagedExtensions = managedAssemblyPure;
		proj.Object.Configurations("Release").ManagedExtensions = managedAssemblyPure;
		AddReferencesForWinForm(proj);
		AddFilesToNewProjectWithInfFile(proj, strProjectName);
		AddEntryPoint(proj, "main");
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
	if (strName == "root.h")
		projfile.Object.FileType = eFileTypeCppForm;
	return false;
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		var strTarget = strName;

		if (strName == "readme.txt")
		{
			strTarget = "ReadMe.txt";
		}
		else if (strName == "root.cpp")
		{
			strTarget = strProjectName + ".cpp";
		}
		else if (strName.substr(0, 4) == "root")
		{
			strTarget = "Form1" + strName.substr(4);
		}
		return strTarget; 
	}
	catch(e)
	{
		throw e;
	}
}
