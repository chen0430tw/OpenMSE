// Copyright (c) Microsoft Corporation. All rights reserved.
function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		if (wizard.FindSymbol("MANAGED") && !IsMCppProject(selProj))
			return VS_E_WIZARDBACKBUTTONPRESS;
	   	if (wizard.FindSymbol("MANAGED") && !PrepareToAddManagedClass(selProj))
	    	return VS_E_WIZARDBACKBUTTONPRESS;

		oCM	= selProj.CodeModel;

		var strProjectName = wizard.FindSymbol("PROJECT_NAME");

		var strClassName = wizard.FindSymbol("CLASS_NAME");
		var strHeader = wizard.FindSymbol("HEADER_FILE");
		var strImpl = wizard.FindSymbol("IMPL_FILE");
		var strBaseName = wizard.FindSymbol("BASE_CLASS_NAME");

		var L_TransactionName_Text = "已添加向导 ";
		// The files we're adding the new class to might have arbitrary content. Make sure CM is synced up to the changes.
		oCM.Synchronize();

		bValid = wizard.ParentObject.ValidateMember(strClassName, vsCMElementClass);
		if (!bValid) {
			var L_ErrorMessageText = "Class '"+strClassName+"' is already implemented in the selected source file.";
			wizard.ReportError(L_ErrorMessageText);
			return;
		}

		oCM.StartTransaction(L_TransactionName_Text + strClassName);

		var newclass = oCM.AddClass(strClassName, strHeader, vsCMAddPositionEnd, "", "", vsCMAccessDefault);
		if(wizard.FindSymbol("MANAGED"))
		{
			try
			{
				newclass.IsManaged = true;
			}
			catch (e)
			{ // above assignment will fail if project is not managed
			}
		}
		if (strBaseName != "")
		{
			var oBase = newclass.AddBase(strBaseName, vsCMAddPositionEnd);
			oBase.Access = wizard.FindSymbol("ACCESS");
		}

		var oConstructor = newclass.AddFunction(strClassName, vsCMFunctionConstructor, "", vsCMAddPositionEnd, vsCMAccessPublic, strImpl);
		var oDestructor;
		if(!wizard.FindSymbol("MANAGED") ||  wizard.FindSymbol("VIRTUAL_DTOR"))
			oDestructor = newclass.AddFunction("~"+strClassName, vsCMFunctionDestructor, "", vsCMAddPositionEnd, vsCMAccessPublic, strImpl);
		// VSWhidbey 383535 Make sure the function bodies are empty -- same as in the nonmanaged case
		if (wizard.FindSymbol("MANAGED"))
		{
			oConstructor.BodyText = "";

		        if(wizard.FindSymbol("VIRTUAL_DTOR")==true)
				oDestructor.BodyText = "";
		}
        if(wizard.FindSymbol("VIRTUAL_DTOR")==true)
		    oDestructor.IsVirtual = true;
		if(strBaseName != "")
		{
			try
			{
				IncludeCodeElementDeclaration(selProj, strBaseName, newclass.Location(vsCMWhereDefault));
			}
			catch(e)
			{
				//don't display the error in case base class was not found: the warning was already displayed in HTML script
				//var L_ErrMsg1_Text = "Unable to find base class definition: ";
				//wizard.ReportError( L_ErrMsg1_Text + e.description);
			}
		}

		oCM.CommitTransaction();

		ShowTextPoint(newclass.StartPoint);
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

