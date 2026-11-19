// Copyright (c) Microsoft Corporation. All rights reserved.
function OnFinish(selProj, selObj)
{   
	var oCM;
	try
	{
		if( selProj )
			oCM	= selProj.CodeModel;

		var strTemplatePath = wizard.FindSymbol("TEMPLATES_PATH");
		var strFileName = wizard.FindSymbol("ITEM_PATH") + "\\" + wizard.FindSymbol("ITEM_NAME");
		var L_addFile_Text = "添加文件 ";
		if( oCM )
			oCM.StartTransaction( L_addFile_Text + strFileName);
	
		// use the filesystem object to check for file existance
		var oFSO = new ActiveXObject("Scripting.FileSystemObject");
		if (oFSO.FileExists(strFileName))
		{
			L_thisFileExists_Text = " 已经存在。请选择一个不同的文件名。";
			wizard.ReportError(strFileName + L_thisFileExists_Text);
			return VS_E_WIZCANCEL;
		}
		
		var strShortName = wizard.FindSymbol("ITEM_NAME");
		var index = strShortName.lastIndexOf(".");
		if (index != -1)
			strShortName = strShortName.substring(0, index);
		wizard.AddSymbol("ITEM_SHORT_NAME", strShortName);

		// render the property sheet file 
		wizard.RenderTemplate(strTemplatePath + "\\" + "root.vsprops", strFileName);
		
		if (selProj != null && selObj != null)		
			selProj.Object.AddFile(strFileName);

		if( oCM )
			oCM.CommitTransaction();
	}
	catch(e)
	{
		if (oCM)
			oCM.AbortTransaction();

		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}
