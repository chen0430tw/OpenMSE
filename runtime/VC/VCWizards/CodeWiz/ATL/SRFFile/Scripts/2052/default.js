// Copyright (c) Microsoft Corporation. All rights reserved.
function OnFinish(selProj, selObj)
{   
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;

		var strTemplatePath = wizard.FindSymbol("TEMPLATES_PATH");
		var strFileName = wizard.FindSymbol("ITEM_NAME");

		var L_TRANSACTION_Text = "添加 ";
		oCM.StartTransaction(L_TRANSACTION_Text + strFileName);

		// use the filesystem object to check for file existance
		var oFSO = new ActiveXObject("Scripting.FileSystemObject");
		if (oFSO.FileExists(strFileName))
		{
			var L_Error1_Text = " 已经存在。请选择一个不同的文件名。";
			wizard.ReportError(strFileName + L_Error1_Text);
			return VS_E_WIZCANCEL;
		}
		
		// render the srf file 
		wizard.RenderTemplate(strTemplatePath + "\\" + "rootapp.srf", strFileName);
		
		// add the srf file to the selected folder (could be the root of the project)
		var srffile = selObj.AddFromFile(strFileName);
        if( srffile )
        {
            var window = srffile.Open(vsViewKindPrimary);
            if(window)
                window.visible = true;
        }
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


