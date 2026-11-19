// Copyright (c) Microsoft Corporation. All rights reserved.
function OnFinish(selProj, selObj)
{
	wizard.AddSymbol("NATIVE_PROJECT",IsNativeProject(selProj));
	try
	{
		if (!IsMCppProject(selProj))
			return VS_E_WIZARDBACKBUTTONPRESS;
		if (!PrepareToAddManagedClass(selProj))
			return VS_E_WIZARDBACKBUTTONPRESS;
		AddFilesToProjectWithInfFile(selProj, wizard.FindSymbol("PROJECT_NAME"), selObj);
		selProj.Object.Save();
	}
	catch (e)
	{
		if (e.description.length > 0)
			SetErrorInfo(e);
		return e.number;
	}
}

function SetFileProperties(oFileItem, strFileName)
{
	if (wizard.FindSymbol("NATIVE_PROJECT")) {
		SetFileLevelClr(oFileItem);
	}
	return false;
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		var strTarget = strName;

		if (strName.substr(0, 7) == "Trigger")
		{
			var strItemName = wizard.FindSymbol("ITEM_NAME");
			var extension = strName.substr(7);
			strTarget = GetFileNameUnique(strItemName, extension);
		}

		return strTarget;
	}
	catch(e)
	{
		throw e;
	}
}

