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
		AddReferencesForWinForm(selProj);
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
	if (strFileName == "NewForm.h")
   	{
		oFileItem.Object.FileType = eFileTypeCppForm;
	} else if (strFileName == "NewForm.cpp") {
		if (wizard.FindSymbol("NATIVE_PROJECT")) {
			SetFileLevelClr(oFileItem);
		}
	}
	return false;
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		return wizard.FindSymbol("ITEM_NAME") + "." + strName.split(".")[1];
	}
	catch(e)
	{
		throw e;
	}
}

