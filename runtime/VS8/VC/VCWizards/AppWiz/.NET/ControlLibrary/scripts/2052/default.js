// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");

		if (!CanUseDrive(strProjectPath))
			return VS_E_WIZARDBACKBUTTONPRESS;

		var proj = CreateManagedProject(strProjectName, strProjectPath);
		AddManagedConfigsForDLL(proj, strProjectName);
		proj.Object.Configurations("Debug").ManagedExtensions = managedAssemblyPure;
		proj.Object.Configurations("Release").ManagedExtensions = managedAssemblyPure;
		AddReferencesForControl(proj);
		AddFilesToNewProjectWithInfFile(proj, strProjectName);
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
		projfile.Object.FileType = eFileTypeCppControl;
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
			strTarget = strProjectName + "Control" + strName.substr(4);
		}
		return strTarget; 
	}
	catch(e)
	{
		throw e;
	}
}

