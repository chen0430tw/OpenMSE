// Copyright (c) Microsoft Corporation. All rights reserved.

function OnFinish(selProj, selObj)
{
	try
	{
		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");
		var isManaged = (wizard.FindSymbol("MANAGED") == "TRUE");

		if (!CanUseDrive(strProjectPath))
			return VS_E_WIZARDBACKBUTTONPRESS;
			
		var proj;
		if (isManaged)
		{
			proj = CreateManagedProject(strProjectName, strProjectPath);
			AddManagedConfigsForEXE(proj, strProjectName, null, false);
		} else {
			proj = CreateProject(strProjectName, strProjectPath);
			SetupFilters(proj);
			AddCommonConfig(proj, strProjectName, true /* unicode */, true /* empty project */);
		}

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


