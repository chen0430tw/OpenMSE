// Copyright (c) Microsoft Corporation. All rights reserved.
var filetowrite = "";

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM = selProj.CodeModel;
		var L_TransactionName_Text = "添加 ActiveX 控件中的类";
		oCM.StartTransaction(L_TransactionName_Text);

		oCM.AddInclude("<afxdisp.h>", "stdafx.h", vsCMAddPositionEnd);
		var nInterfaceCount = wizard.FindSymbol("INTERFACE_COUNT");

		for (i = 0; i < nInterfaceCount; i++)
		{
			var strHeader = wizard.FindSymbol("HEADER_FILE_" + i);
			wizard.AddSymbol("HEADER_FILE", strHeader);

			var strImpl = wizard.FindSymbol("IMPL_FILE_" + i);
			wizard.AddSymbol("IMPL_FILE", strImpl);

			try
			{
				var strRefs = wizard.FindSymbol("REF_INTERFACES_" + i.toString());
				wizard.AddSymbol("REF_INTERFACES", strRefs);

				var strRefIncludes = wizard.FindSymbol("REF_INTERFACES_INCLUDES_" + i.toString());
				wizard.AddSymbol("REF_INTERFACES_INCLUDES", strRefIncludes);
			}
			catch(e)
			{
				// no referenced interfaces
				wizard.AddSymbol("REF_INTERFACES", " ");
				wizard.AddSymbol("REF_INTERFACES_INCLUDES", " ");
			}

			var strClassName = wizard.FindSymbol("CLASS_NAME_" + i);
			wizard.AddSymbol("CLASS_NAME", strClassName);

			var strClassText = wizard.FindSymbol("CLASS_TEXT_" + i);
			wizard.AddSymbol("CLASS_TEXT", strClassText);

			var strCLSID = wizard.FindSymbol("CONTROL_CLSID_" + i);
			wizard.AddSymbol("CONTROL_CLSID", strCLSID);

			if (i == 0)
			{
				// CWnd derived class for default interface
				IsUsedFileName(strHeader, true); // This sets the variable filetowrite
				RenderAddTemplate(wizard, "wrapper.h", filetowrite, selObj, true);
				IsUsedFileName(strImpl, true);
				RenderAddTemplate(wizard, "wrapper.cpp", filetowrite, selObj, false);
			}
			else
			{
				// COleDispatchDriver derived classes

				IsUsedFileName(strHeader, true);
				RenderAddTemplate(wizard, "wrapperdisp.h", filetowrite, selObj, false);
				IsUsedFileName(strImpl, true);
				RenderAddTemplate(wizard, "wrapperdisp.cpp", filetowrite, selObj, false);
			}

			var strFileName = GetProjectFile(selProj, "CPP", false, true);
			oCM.AddInclude("\"" + strHeader + "\"", strFileName, vsCMAddPositionEnd);

		}
		oCM.CommitTransaction();
	}
	catch(e)
	{
		if (oCM)
			oCM.AbortTransaction();

		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number;
	}
}

function IsUsedFileName(strFileName, bCheckIfMidlHeader)
{
	filetowrite = strFileName;
	if (bCheckIfMidlHeader)
	{
		var oMidlTool = wizard.ProjectObject.Object.Configurations(1).Tools("VCMidlTool");
		var strHeadFile = wizard.ProjectObject.Object.Configurations(1).Evaluate(oMidlTool.HeaderFileName);
		if (strHeadFile.toLowerCase() == strFileName.toLowerCase())
		{   
			var L_ErrMsg20_Text = " 由 MIDL 生成但无法使用。";
			wizard.ReportError(strFileName + L_ErrMsg20_Text);
			return false;
		}
	}
	if (wizard.DoesFileExist(strFileName))
	{
			var strProjectPath	= wizard.FindSymbol("PROJECT_PATH");
			var UniqueFileName = GetUniqueFileName(strProjectPath, strFileName);
			var L_MergeMsg1_Text = " 已经存在。要将此类合并到同一文件中吗? 如果“否”，则它将被添加为";
			var L_Period_Text = ".";
			var bRet = wizard.YesNoAlert(strFileName + L_MergeMsg1_Text + UniqueFileName + L_Period_Text);
			if (bRet)
				wizard.AddSymbol("MERGE_FILE", true);
			else
				filetowrite = UniqueFileName;
			return bRet;
	}
	else
		return true;
}
